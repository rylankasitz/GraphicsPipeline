#include "stdafx.h"
#include "scene.h"

Scene *scene;

using namespace std;

#include <iostream>
#include <string>
#include <chrono>

using namespace std::chrono;

Scene::Scene() {

	int u0 = 20;
	int v0 = 40;
	int h = 800;
	int w = 1200;

	gui = new GUI();
	gui->show();
	gui->uiw->position(u0 + w + 50, v0);

	plight = new Light(0.25f, 0.5f, this);
	projector = new Projector("geometry/Textures/blender.tiff", this);
	rm = new ResourceManager();

	worldView = new WorldView(0, u0, v0, w, h, 90.0f, "Main");
	worldView->ppc->SetPose(Vector(-100, 100, 100), Vector::ZERO, Vector::YAXIS);
	worldView->ppc->SetFocalLength(800);
	worldView->Show();

	views.push_back(new WorldView(0, u0, v0, w, h, 90.0f, "v0"));
	views[0]->ppc->SetPose(Vector(0, 100, 100), Vector::ZERO, Vector::YAXIS);
	views[0]->ppc->SetFocalLength(800);

	views.push_back(new WorldView(0, u0, v0, w, h, 90.0f, "v1"));
	views[1]->ppc->SetPose(Vector(0, 100, 100), Vector::ZERO, Vector::YAXIS);
	views[1]->ppc->SetFocalLength(800);

	views.push_back(new WorldView(0, u0, v0, w, h, 90.0f, "v2"));
	views[2]->ppc->SetPose(Vector(200, 100, 100), Vector::ZERO, Vector::YAXIS);
	views[2]->ppc->SetFocalLength(800);

	InputHandler::Instatiate(worldView);

	rm->Load();
	Load();
	Render();
}

void Scene::Load() {

	rm->meshes["terrain"].SetCenter(Vector::ZERO);
	rm->meshes["terrain"].SetScale(400);
	rm->meshes["terrain"].material = rm->materials["green"];

	rm->meshes["monkey"].SetCenter(Vector(0, 50, 0));
	rm->meshes["monkey"].SetScale(40);
	rm->meshes["monkey"].material = rm->materials["blue"];

	rm->meshes["cube"].SetCenter(Vector(50, 50, 0));
	rm->meshes["cube"].SetScale(40);
	rm->meshes["cube"].material = rm->materials["red"];

	rm->meshes["plane"].SetCenter(Vector(-50, 20, 0));
	rm->meshes["plane"].SetScale(100);
	rm->meshes["plane"].material = rm->materials["yellow"];

	plight->SetPosition(Vector(100, 100, 100));
	projector->SetPostion(Vector(-100, 50, -100), Vector::ZERO);
}

void Scene::Render() {

	Fl::check();

	worldView->Render(this);
	views[0]->Render(this);
}

void Scene::DBG() {

}

void Scene::Demo() {

	int steps = 600;
	views[0]->Show();
	for (int i = 0; i < steps; i++) {	

		if (i < 200)
			plight->Rotate(Vector::ZERO, Vector::YAXIS, 360 / (float) 200);
		else if (i < 400)
			projector->Rotate(Vector::ZERO, Vector::YAXIS, 360 / (float)200);
		else
			views[0]->ppc->Interpolate(views[1]->ppc, views[2]->ppc, i - 400, 200);

		Render();

		cerr << "Frame " << i << endl;

		string out = "animations/rotation_";
		out += to_string(i);
		out += ".tif";
		views[0]->fb->SaveAsTiff((char *) out.c_str());
	}
}