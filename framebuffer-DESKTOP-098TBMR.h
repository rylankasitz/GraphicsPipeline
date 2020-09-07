#pragma once

#include "vector.h";
#include "matrix.h";

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

class FrameBuffer : public Fl_Gl_Window {
public:
	unsigned int *pix; // pixel array
	int w, h;
	FrameBuffer(int u0, int v0, int _w, int _h, unsigned int _id);

	void KeyboardHandle();
	int handle(int guievent);

	void draw();
	void SetBGR(unsigned int bgr);
	void Set(int u, int v, unsigned int color);

	void SetSegment(Vector p1, Vector p2, unsigned int col);
	void DrawTriangle(Vector p1, Vector p2, Vector p3);
	void SetRectangle();
	void SetCircle();

	void LoadTiff(char* fname);
	void SaveAsTiff(char *fname);
};