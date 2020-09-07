#include "stdafx.h"

#include "framebuffer.h"
#include "math.h"
#include "scene.h"

#include <tiffio.h>
#include <algorithm>  
#include <iostream>

using namespace std;

FrameBuffer::FrameBuffer(int u0, int v0,
	int _w, int _h, unsigned int _id) : Fl_Gl_Window(u0, v0, _w, _h, 0) {

	w = _w;
	h = _h;
	pix = new unsigned int[w * h];

}

#pragma region Input Handling

int FrameBuffer::handle(int event) {

	switch (event)
	{
	case FL_KEYBOARD: {
		KeyboardHandle();
		return 0;
	}
	default:
		break;
	}
	return 0;
}

void FrameBuffer::KeyboardHandle() {

	int key = Fl::event_key();

	switch (key) {
	case FL_Up: {
		cerr << "INFO: pressed up key";
		break;
	}
	default:
		cerr << "INFO: do not understand keypress" << endl;
	}
}

#pragma endregion

#pragma region Pixel Drawing

void FrameBuffer::draw() {

	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);

}

void FrameBuffer::Set(int u, int v, unsigned int color) {

	pix[(h - 1 - v)*w + u] = color;

}

void FrameBuffer::SetBGR(unsigned int bgr) {

	for (int uv = 0; uv < w*h; uv++)
		pix[uv] = bgr;

}

#pragma endregion

#pragma region 2D Shape Drawing

void FrameBuffer::Set2dSegment(Vector p1, Vector p2, unsigned int col) {

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
		float cx = p1[0] + (p2[0] - p1[0]) / (float)stepsN * (float)i;
		float cy = p1[1] + (p2[1] - p1[1]) / (float)stepsN * (float)i;
		Set((int)cx, (int)cy, col);
	}
}

void FrameBuffer::Set2dRectangle(Vector origin, float width, float height, unsigned int col) {

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Set((int) (i + origin[0]), (int) (j + origin[1]), col);
		}
	}

}

void FrameBuffer::Set2dCircle(Vector center, float radius, unsigned int col) {

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

void FrameBuffer::Set2dTriangle(Vector p1, Vector p2, Vector p3, unsigned int col) {

	float minX = min(p1[0], min(p2[0], p3[0]));
	float minY = min(p1[1], min(p2[1], p3[1]));
	
	float maxX = max(p1[0], max(p2[0], p3[0]));
	float maxY = max(p1[1], max(p2[1], p3[1]));

	float a[3] = { p2[1] - p1[1], p3[1] - p1[1], p3[1] - p2[1] };
	float b[3] = { p2[0] - p1[0], p3[0] - p1[0], p3[0] - p2[0] };
	float c[3] = { p1[0] * p2[1] + -p1[1] * p2[0],
				   p1[0] * p3[1] + -p1[1] * p3[0],
				   p2[0] * p3[1] + -p2[1] * p3[0] };

	for (int i = 0; i < maxX + 1; i++) {
		for (int j = 0; j < maxY + 1; j++) {
			Vector p (i + minX, j + minY, 0);

			if (checkEdge(p, a[0], b[0], c[0]) &&
				checkEdge(p, a[1], b[1], c[1]) &&
				checkEdge(p, a[2], b[2], c[2])) {
				Set((int)p[0], (int)p[1], col);
			}
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
	return (p[0] * a - p[1] * b + c) < 0;
}

#pragma endregion
