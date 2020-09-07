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

	void Set2dSegment(Vector p1, Vector p2, unsigned int col);
	void Set2dTriangle(Vector p1, Vector p2, Vector p3, unsigned int col);
	void Set2dRectangle(Vector origin, float width, float height, unsigned int col);
	void Set2dCircle(Vector center, float radius, unsigned int col);

	void LoadTiff(char* fname);
	void SaveAsTiff(char *fname);

private:
	bool checkEdge(Vector p, float a, float b, float c);
};