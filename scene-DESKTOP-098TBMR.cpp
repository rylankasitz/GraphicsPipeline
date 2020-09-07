#include "stdafx.h"

#include "scene.h"

Scene *scene;

using namespace std;

#include <iostream>

Scene::Scene() {

	gui = new GUI();
	gui->show();

	int u0 = 20;
	int v0 = 20;
	int h = 480;
	int w = 640;

	fb = new FrameBuffer(u0, v0, w, h, 0);
	fb->label("SW frame buffer");
	fb->SetBGR(0xFF00FFFF);
	fb->show();
	fb->redraw();

	gui->uiw->position(u0, v0 + h + 50);

}


void Scene::DBG() {

	cerr << "INFO: pressed DBG Button" << endl;

	Vector v = Vector(0, 0, 0);
	Vector u = Vector(1, 0, 0);
	Vector w = Vector(7, 8, 9);

	Matrix m = Matrix(v, u, w);

	w.RotatePoint(v, u, 30);

	cerr << "Rotated: " << w << endl;

	//fb->Set2dSegment(v, u, 0xFF0000);
	fb->redraw();
}


void Scene::NewButton() {
	cerr << "INFO: pressed New Button" << endl;
}
