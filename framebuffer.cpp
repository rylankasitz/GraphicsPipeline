#include "stdafx.h"

#include "framebuffer.h"
#include "math.h"
#include "scene.h"
#include "vector.h"
#include "matrix.h"

#include <tiffio.h>
#include <algorithm>  
#include <iostream>

using namespace std;

FrameBuffer::FrameBuffer(int u0, int v0,
	int _w, int _h, unsigned int _id) : Fl_Gl_Window(u0, v0, _w, _h, 0) {

	w = _w;
	h = _h;
	pix = new unsigned int[w * h];
	zb = new float[w * h];

}

#pragma region Input Handling

int FrameBuffer::handle(int event) {
	return InputHandler::event_handler(event);
}

#pragma endregion

#pragma region Pixel Drawing

void FrameBuffer::ClearZB() {

	for (int uv = 0; uv < w * h; uv++)
		zb[uv] = 0.0f;

}

void FrameBuffer::draw() {

	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);

}

void FrameBuffer::Set(int u, int v, unsigned int color) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return;

	pix[(h - 1 - v)*w + u] = color;

}

void FrameBuffer::SetBGR(unsigned int bgr) {

	for (int uv = 0; uv < w*h; uv++)
		pix[uv] = bgr;

}

#pragma endregion

#pragma region 2D Shape Drawing

void FrameBuffer::Draw2dSegment(Vector p1, Vector p2, Vector c1, Vector c2) {

	float dx = fabsf(p1[0] - p2[0]);
	float dy = fabsf(p1[1] - p2[1]);

	int stepsN;
	if (dx < dy) {
		stepsN = (int)dy;
	}
	else {
		stepsN = (int)dx;
	}
	for (int i = 0; i <= stepsN; i++) {
		Vector cp, cc;
		cp = p1 + (p2 - p1) * (float)i / (float)stepsN;

		int u = (int)cp[0], v = (int)cp[1];

		if (farther(u, v, cp[2]))
			continue;

		cc = c1 + (c2 - c1) * (float)i / (float)stepsN;

		Set(u, v, cc.GetColor());
	}
}

void FrameBuffer::Draw2dRectangle(Vector origin, float width, float height, unsigned int col) {

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Set((int) (i + origin[0]), (int) (j + origin[1]), col);
		}
	}
}

void FrameBuffer::Draw2dCircle(Vector center, float radius, unsigned int col) {

	float x = center[0] - radius;
	float y = center[1] - radius;
	float d = radius * 2;

	for (int i = 0; i < d + 1; i++) {
		for (int j = 0; j < d + 1; j++) {
			Vector p (i + x, j + y, 0);
			Vector c(center[0], center[1], 0.0f);
			float len = (p - c).Length();

			if (len <= radius) {
				Set((int) p[0], (int) p[1], col);
			}
		}
	}
}

void FrameBuffer::Draw2dTriangle(Vector p1, Vector p2, Vector p3, Vector c1, Vector c2, Vector c3) {

	if (getTriangleArea(p1, p2, p3) < .001f) {
		return;
	}

	float minX = clamp(min(p1[0], min(p2[0], p3[0])), 0, w);
	float minY = clamp(min(p1[1], min(p2[1], p3[1])), 0, h);
	float maxX = clamp(max(p1[0], max(p2[0], p3[0])), 0, w);
	float maxY = clamp(max(p1[1], max(p2[1], p3[1])), 0, h);
	float minZ = min({ p1[2], p2[2], p3[2] });
	float maxZ = max({ p1[2], p2[2], p3[2] });

	Vector rCoff = getInterpCoffs(p1, p2, p3, Vector(c1[0], c2[0], c3[0]));
	Vector gCoff = getInterpCoffs(p1, p2, p3, Vector(c1[1], c2[1], c3[1]));
	Vector bCoff = getInterpCoffs(p1, p2, p3, Vector(c1[2], c2[2], c3[2]));
	Vector zCoff = getInterpCoffs(p1, p2, p3, Vector(p1[2], p2[2], p3[2]));

	Vector mins = Vector(min({ c1[0], c2[0], c3[0] }), min({ c1[1], c2[1], c3[1] }), min({ c1[2], c2[2], c3[2] }));
	Vector maxs = Vector(max({ c1[0], c2[0], c3[0] }), max({ c1[1], c2[1], c3[1] }), max({ c1[2], c2[2], c3[2] }));

	float x[3] = { p1[0], p2[0], p3[0] };
	float y[3] = { p1[1], p2[1], p3[1] };

	float a[3], b[3], c[3];
	for (int i = 0; i < 3; i++) {
		int j = (i + 1) % 3;
		int k = (j + 1) % 3;

		a[i] = y[j] - y[i];
		b[i] = -x[j] + x[i];
		c[i] = y[j] * -x[i] + x[j] * y[i];

		float sideness = a[i] * x[k] + b[i] * y[k] + c[i];

		if (sideness < 0) {
			a[i] = -a[i]; b[i] = -b[i]; c[i] = -c[i];
		}
	}

	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			Vector p (i, j, 1);

			if (checkEdge(p, a[0], b[0], c[0]) &&
				checkEdge(p, a[1], b[1], c[1]) &&
				checkEdge(p, a[2], b[2], c[2])) {

				float z = zCoff * p;
				if (farther(p[0], p[1], clamp(z, minZ, maxZ)))
					continue;

				Vector col = Vector(rCoff * p, gCoff * p, bCoff * p);
				col.Clamp(mins, maxs);

				Set((int)p[0], (int)p[1], col.GetColor());
			}
		}
	}

}

#pragma endregion

#pragma region 3D Shape Drawing

void FrameBuffer::Draw3dTriangle(Vector point1, Vector point2, Vector point3, Vector color1, Vector color2, Vector color3, PPC* ppc) {

	Vector point1Ref, point2Ref, point3Ref;
	if (!ppc->Project(point1, point1Ref))
		return;
	if (!ppc->Project(point2, point2Ref))
		return;
	if (!ppc->Project(point3, point3Ref))
		return;

	Draw2dTriangle(point1Ref, point2Ref, point3Ref, color1, color2, color3);
}

void FrameBuffer::Draw3dSegment(Vector point1, Vector point2, PPC* ppc, Vector color1, Vector color2) {

	Vector point1Ref, point2Ref;
	if (!ppc->Project(point1, point1Ref))
		return;
	if (!ppc->Project(point2, point2Ref))
		return;

	Draw2dSegment(point1Ref, point2Ref, color1, color2);
}

void FrameBuffer::Draw3DPoint(Vector point, PPC* ppc, unsigned int color, int psize) {

	Vector pointRef;
	if (!ppc->Project(point, pointRef))
		return;

	int u = (int)pointRef[0];
	int v = (int)pointRef[1];

	for (int cv = v - psize / 2; cv <= v + psize / 2; cv++) {
		for (int cu = u - psize / 2; cu <= u + psize / 2; cu++) {
			if (farther(cu, cv, pointRef[2]))
				continue;

			Set(cu, cv, color);
		}
	}

}

#pragma endregion

#pragma region Image Loading

void FrameBuffer::LoadTiff(char* fname) {

	TIFF* in = TIFFOpen(fname, "r");
	if (in == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	int width, height;
	TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
	if (w != width || h != height) {
		w = width;
		h = height;
		delete[] pix;
		pix = new unsigned int[w*h];
		size(w, h);
		glFlush();
		glFlush();
	}

	if (TIFFReadRGBAImage(in, w, h, pix, 0) == 0) {
		cerr << "failed to load " << fname << endl;
	}

	TIFFClose(in);
}

void FrameBuffer::SaveAsTiff(char *fname) {

	TIFF* out = TIFFOpen(fname, "w");

	if (out == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 4);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	for (uint32 row = 0; row < (unsigned int)h; row++) {
		TIFFWriteScanline(out, &pix[(h - row - 1) * w], row);
	}

	TIFFClose(out);
}

#pragma endregion

#pragma region Private Methods

bool FrameBuffer::checkEdge(Vector p, float a, float b, float c) {
	return (p[0] * a + p[1] * b + c) >= 0;
}

int FrameBuffer::farther(int u, int v, float currz) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return 1;

	int uv = (h - 1 - v) * w + u;
	if (currz < zb[uv])
		return 1;

	zb[uv] = currz;
	return 0;
}

Vector FrameBuffer::getInterpCoffs(Vector p1, Vector p2, Vector p3, Vector c) {

	Matrix m = Matrix();
	m[0] = Vector(p1[0], p1[1], 1.0f);
	m[1] = Vector(p2[0], p2[1], 1.0f);
	m[2] = Vector(p3[0], p3[1], 1.0f);

	return m.Inverted() * c;
}

float FrameBuffer::getTriangleArea(Vector a, Vector b, Vector c) {
	return fabsf((a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + c[0] * (a[1] - b[1])) / 2.0f);
}

#pragma endregion
