#include "stdafx.h"

#include "scene.h"

Scene *scene;

using namespace std;

#include <iostream>
#include <string>

Scene::Scene() {

	gui = new GUI();
	gui->show();

	int u0 = 20;
	int v0 = 20;
	int h = 480;
	int w = 640;

	fb = new FrameBuffer(u0, v0, w, h, 0);
	fb->label("SW frame buffer");
	fb->SetBGR(0xFFFFFFFF);
	fb->show();
	fb->redraw();

	gui->uiw->position(u0, v0 + h + 50);

}


void Scene::DBG() {
	fb->Set2dCircle(Vector(200, 200, 0), 20, 0xFF000000);
	fb->redraw();
}

void Scene::Demo() {

	Vector point(200.0f, 320.0f, 0.0f);
	Vector origin(320.0f, 240.0f, 0.0f);
	Vector axis(0.0f, 1.0f, -1.0f);

	float pRadius = 5;
	float inc = 1;

	int steps = 360;

	for (int i = 0; i < steps; i++) {
		fb->SetBGR(0xFFFFFFFF);
		fb->Set2dCircle(point, pRadius, 0xFF000000);
		point.RotatePoint(origin, axis, inc);
		fb->redraw();
		Fl::check();

		string out = "animations/rotation_";
		out += to_string(i);
		out += ".tif";
		fb->SaveAsTiff((char *) out.c_str());
	}
}


void Scene::Calc() {

	Vector v, u;
	Matrix m1, m2;
	float s;

	cerr << "Enter vector (v): ";
	cin >> v;

	cerr << "Enter vector (u): ";
	cin >> u;

	cerr << "Enter scalar value (s): ";
	cin >> s;

	cerr << "v = " << v << endl;
	cerr << "u = " << u << endl;
	cerr << "s = " << s << endl;
	cerr << "" << endl;
	cerr << "v + u = " << (v + u) << endl;
	cerr << "v - u = " << (v - u) << endl;
	cerr << "v * u = " << (v * u) << endl;
	cerr << "v x u = " << (v ^ u) << endl;
	cerr << "v * s = " << (v * s) << endl;
	cerr << "v / s = " << (v / s) << endl;
	cerr << "" << endl;
	cerr << "Length of v = " << v.Length() << endl;
	cerr << "Normalization of v = " << v.Normalized() << endl;
	cerr << "" << endl;

	cerr << "Enter a 3 by 3 matrix (m1): ";
	cin >> m1;

	cerr << "Enter a 3 by 3 matrix (m2): ";
	cin >> m2;

	cerr << "m = " << m1 << endl;
	cerr << "m = " << m2 << endl;
	cerr << "" << endl;

	cerr << "m1 * v = " << (m1 * v) << endl;
	cerr << "m1 * m2 = " << (m1 * m2) << endl;
	cerr << "" << endl;
	cerr << "Inversion of m1 = " << m1.Inverted() << endl;
	cerr << "Transposition of m1 = " << m1.Transposed() << endl;
}
