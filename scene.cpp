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

	plight = new Light(Vector(100.0f, 150.0f, -100.0f), 0.25f, 0.5f);
	projector = new Projector("geometry/Textures/WhiteMarble.tif", Vector(100, 100, 100), Vector::ZERO);
	rm = new ResourceManager();

	worldView = new WorldView(0, u0, v0, w, h, 90.0f, "Main");
	worldView->ppc->SetPose(Vector(-100, 100, 100), Vector::ZERO, Vector::YAXIS);
	worldView->ppc->SetFocalLength(800);
	worldView->Show();

	views.push_back(new WorldView(0, u0, v0, w, h, 90.0f, "v0"));
	views[0]->ppc->SetPose(Vector(-100, 100, 100), Vector::ZERO, Vector::YAXIS);
	views[0]->ppc->SetFocalLength(800);

	views.push_back(new WorldView(0, u0, v0, w, h, 90.0f, "v1"));
	views[1]->ppc->SetPose(Vector(-100, 100, 100), Vector::ZERO, Vector::YAXIS);
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

	plight->RenderShadowMap(this);
	projector->RenderFB(this);
}

void Scene::Render() {

	Fl::check();

	worldView->Render(this);
	views[0]->Render(this);
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

		//Fl::check();
		Render();

		cerr << "Frame " << i << endl;

		string out = "animations/rotation_";
		out += to_string(i);
		out += ".tif";
		views[0]->fb->SaveAsTiff((char *) out.c_str());
	}
}