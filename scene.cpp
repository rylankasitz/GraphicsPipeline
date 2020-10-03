#include "stdafx.h"
#include "scene.h"

Scene *scene;

using namespace std;

#include <iostream>
#include <string>

Scene::Scene() {

	int u0 = 20;
	int v0 = 40;
	int h = 800;
	int w = 1200;

	meshCount = 5;
	viewCount = 3;

	gui = new GUI();
	gui->show();
	gui->uiw->position(u0 + w + 50, v0);

	plight = new PointLight(Vector(0.0f, 0.0f, 100.0f), 0.25f, 32, 0.5f);

	meshes = new TMesh[meshCount];
	views = new WorldView*[viewCount];

	worldView = new WorldView(0, u0, v0, w, h, 90.0f, "Main");
	worldView->ppc->SetPose(Vector(0, 100, 100), Vector::ZERO, Vector::YAXIS);
	worldView->ppc->SetFocalLength(800);
	worldView->Show();

	InputHandler::Instatiate(worldView);

	views[0] = new WorldView(0, u0 + w + 50, v0 + 200, 720, 405, 90.0f, "Demo");
	views[0]->ppc->SetPose(Vector(100, 100, 0), Vector::ZERO, Vector::YAXIS);
	views[0]->ppc->SetFocalLength(400);

	views[1] = new WorldView(0, u0 + w + 50, v0 + 200, 720, 405, 90.0f, "Interp1");
	views[1]->ppc->SetPose(Vector(100, 100, 0), Vector::ZERO, Vector::YAXIS);
	views[1]->ppc->SetFocalLength(400);

	views[2] = new WorldView(0, u0 + w + 50, v0 + 200, 720, 405, 90.0f, "Interp2");
	views[2]->ppc->SetPose(Vector(0, 100, 100), Vector::ZERO, Vector::YAXIS);
	views[2]->ppc->SetFocalLength(400);

	Load();
	Render();
}

void Scene::Load() {

	TMesh teapot57k = TMesh();
	TMesh teapot1k1 = TMesh();
	TMesh teapot1k2 = TMesh();
	TMesh teapot1k3 = TMesh();
	TMesh bunny = TMesh();

	teapot1k1.LoadBin("geometry/teapot1k.bin");
	teapot57k.LoadBin("geometry/teapot57k.bin");
	teapot1k2.LoadBin("geometry/teapot1k.bin");
	teapot1k3.LoadBin("geometry/teapot1k.bin");
	bunny.LoadBin("geometry/happy4.bin");

	teapot1k1.SetScale(80);
	teapot1k2.SetScale(80);
	teapot1k3.SetScale(80);
	teapot57k.SetScale(80);
	bunny.SetScale(80);

	teapot57k.SetCenter(Vector::ZERO);
	teapot1k1.SetCenter(Vector(50, 0, 50));
	teapot1k2.SetCenter(Vector(-50, 0, 50));
	teapot1k3.SetCenter(Vector(50, 0, -50));
	bunny.SetCenter(Vector(-50, 0, -50));

	meshes[0] = teapot1k1;
	meshes[1] = teapot1k2;
	meshes[2] = teapot1k3;
	meshes[3] = teapot57k;
	meshes[4] = bunny;
}

void Scene::Render() {

	Fl::check();

	worldView->Render(meshes, meshCount, views, viewCount, plight);
	views[0]->Render(meshes, meshCount, views, 0, plight);
}

void Scene::DBG() {
	TMesh mesh = TMesh();

	mesh.LoadBin("geometry/teapot57k.bin");
	mesh.SetCenter(Vector(0, 0, 0));

	for (int i = 0; i < 360; i++) {

		Fl::check();
		Render();
	}
}



void Scene::Demo() {
	int steps = 300;

	views[0]->Show();

	for (int i = 0; i < steps; i++) {
		
		if (i >= ((steps / 2) - 1))
			views[0]->ppc->Interpolate(views[1]->ppc, views[2]->ppc, i - ((steps / 2) - 1), steps / 2);

		meshes[0].Rotate(Vector::ZERO, Vector::YAXIS, 1);
		meshes[1].Rotate(Vector::ZERO, Vector::ZAXIS, 1);
		meshes[2].Rotate(Vector::ZERO, Vector::XAXIS, 1);
		meshes[3].Rotate(Vector::ZERO, Vector::YAXIS, 1);
		meshes[4].Rotate(Vector::ZERO, Vector::YAXIS, 1);

		Fl::check();
		Render();

		cerr << "Frame " << i << endl;

		string out = "animations/rotation_";
		out += to_string(i);
		out += ".tif";
		views[0]->fb->SaveAsTiff((char *) out.c_str());
	}
}