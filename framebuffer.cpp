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

bool FrameBuffer::Get(int u, int v, unsigned int& color) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return false;

	color = pix[(h - 1 - v) * w + u];
	return true;
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

		if (Farther(u, v, cp[2]))
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

#pragma endregion

#pragma region 3D Shape Drawing

int FrameBuffer::Farther(int u, int v, float currz) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return 1;

	int uv = (h - 1 - v) * w + u;
	if (currz < zb[uv])
		return 1;

	zb[uv] = currz;
	return 0;
}

bool FrameBuffer::Closer(int u, int v, float currz, float error) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return false;

	int uv = (h - 1 - v) * w + u;
	return currz >= zb[uv] - error;
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
			if (Farther(cu, cv, pointRef[2]))
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

#pragma endregion
