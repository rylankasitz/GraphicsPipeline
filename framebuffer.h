#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

class PPC;
class Vector;

class FrameBuffer : public Fl_Gl_Window {
public:
	unsigned int *pix; // pixel array
	float* zb;
	int w, h;

	FrameBuffer(int u0, int v0, int _w, int _h, unsigned int _id);

	int handle(int guievent);

	void draw();
	void SetBGR(unsigned int bgr);
	void Set(int u, int v, unsigned int color);
	void ClearZB();

	void Draw2dSegment(Vector p1, Vector p2, Vector c1, Vector c2);
	void Draw2dTriangle(Vector p1, Vector p2, Vector p3, Vector c1, Vector c2, Vector c3);
	void Draw2dRectangle(Vector origin, float width, float height, unsigned int col);
	void Draw2dCircle(Vector center, float radius, unsigned int col);

	void Draw3dSegment(Vector point1, Vector point2, PPC* ppc, Vector color1, Vector color2);
	void Draw3dTriangle(Vector point1, Vector point2, Vector point3, Vector color1, Vector color2, Vector color3, PPC* ppc);
	void Draw3DPoint(Vector point, PPC* ppc, unsigned int color, int psize);

	void LoadTiff(char* fname);
	void SaveAsTiff(char *fname);

private:
	bool checkEdge(Vector p, float a, float b, float c);
	int farther(int u, int v, float currz);
	Vector getInterpCoffs(Vector p1, Vector p2, Vector p3, Vector c);
	float getTriangleArea(Vector p1, Vector p2, Vector p3);
};