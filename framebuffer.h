#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

class PPC;
class Vector;
class WorldView;

class FrameBuffer : public Fl_Gl_Window {
public:
	unsigned int *pix; // pixel array
	float* zb;
	int w, h;

	WorldView* wv;

	FrameBuffer(int u0, int v0, int _w, int _h, unsigned int _id);

	int handle(int guievent);

	void draw();
	void SetBGR(unsigned int bgr);
	void Set(int u, int v, unsigned int color);
	bool Get(int u, int v, unsigned int& color);
	void ClearZB();

	void Draw2dSegment(Vector p1, Vector p2, Vector c1, Vector c2);
	void Draw2dRectangle(Vector origin, float width, float height, unsigned int col);
	void Draw2dCircle(Vector center, float radius, unsigned int col);

	void Draw3dSegment(Vector point1, Vector point2, PPC* ppc, Vector color1, Vector color2);
	void Draw3DPoint(Vector point, PPC* ppc, unsigned int color, int psize);

	int Farther(int u, int v, float currz);
	bool Closer(int u, int v, float currz, float error);

	void LoadTiff(char* fname);
	void SaveAsTiff(char *fname);

private:
};