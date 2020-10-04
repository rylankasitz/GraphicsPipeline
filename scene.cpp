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

	meshCount = 4;
	viewCount = 3;

	gui = new GUI();
	gui->show();
	gui->uiw->position(u0 + w + 50, v0);

	plight = new Light(Vector(0.0f, 0.0f, 100.0f), 0.5f, 0.5f);

	meshes = new TMesh[meshCount];
	views = new WorldView*[viewCount];

	worldView = new WorldView(0, u0, v0, w, h, 90.0f, "Main");
	worldView->ppc->SetPose(Vector(-100, 100, 100), Vector::ZERO, Vector::YAXIS);
	worldView->ppc->SetFocalLength(800);
	worldView->Show();

	views[0] = new WorldView(0, u0, v0, w, h, 90.0f, "v0");
	views[0]->ppc->SetPose(Vector(-100, 100, 100), Vector::ZERO, Vector::YAXIS);
	views[0]->ppc->SetFocalLength(800);

	views[1] = new WorldView(0, u0, v0, w, h, 90.0f, "v1");
	views[1]->ppc->SetPose(Vector(-100, 100, 100), Vector::ZERO, Vector::YAXIS);
	views[1]->ppc->SetFocalLength(800);

	views[2] = new WorldView(0, u0, v0, w, h, 90.0f, "v2");
	views[2]->ppc->SetPose(Vector(200, 100, 100), Vector::ZERO, Vector::YAXIS);
	views[2]->ppc->SetFocalLength(800);

	InputHandler::Instatiate(worldView);

	Load();
	Render();
}

void Scene::Load() {

	TMesh table = TMesh(); TMesh floor = TMesh(); TMesh notebook = TMesh(); TMesh mirror = TMesh();

	table.LoadObj("geometry/table.obj", "geometry/Textures/RedOak.tif", true);
	table.SetScale(80);
	table.SetCenter(Vector(0.0f, 40.0f, 0.0f));
	table.Rotate(table.GetCenter(), Vector::YAXIS, 90);

	floor.LoadObj("geometry/floor.obj", "geometry/Textures/WhiteMarble.tif", true);
	floor.SetScale(200);
	floor.SetCenter(Vector::ZERO);

	notebook.LoadObj("geometry/notebook.obj", "geometry/Textures/notebook.tif", true);
	notebook.SetScale(15);
	notebook.SetCenter(Vector(-5.0f, 50.0f, 5.0f));

	mirror.LoadObj("geometry/mirror.obj", "geometry/Textures/reflection.tif", true);
	mirror.SetScale(18);
	mirror.SetCenter(Vector(20.0f, 55.0f, -7.0f));
	mirror.Rotate(mirror.GetCenter(), Vector::YAXIS, 200);
	mirror.Rotate(mirror.GetCenter(), Vector::ZAXIS, 180);

	meshes[0] = table;
	meshes[1] = floor;
	meshes[2] = notebook;
	meshes[3] = mirror;
}

void Scene::Render() {

	Fl::check();
	worldView->Render(meshes, meshCount, views, viewCount, plight);
	views[0]->Render(meshes, meshCount, views, 0, plight);
}

void Scene::DBG() {

}



void Scene::Demo() {
	int steps = 300;

	views[0]->Show();

	for (int i = 0; i < steps; i++) {
		
		views[0]->ppc->Interpolate(views[1]->ppc, views[2]->ppc, i, steps);

		//meshes[0].Rotate(Vector::ZERO, Vector::YAXIS, 1);
		//meshes[1].Rotate(Vector::ZERO, Vector::ZAXIS, 1);
		//meshes[2].Rotate(Vector::ZERO, Vector::XAXIS, 1);
		//meshes[3].Rotate(Vector::ZERO, Vector::YAXIS, 1);
		//meshes[4].Rotate(Vector::ZERO, Vector::YAXIS, 1);

		Fl::check();
		Render();

		cerr << "Frame " << i << endl;

		string out = "animations/rotation_";
		out += to_string(i);
		out += ".tif";
		views[0]->fb->SaveAsTiff((char *) out.c_str());
	}
}