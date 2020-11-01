#include "stdafx.h"
#include "scene.h"

Scene *scene;

using namespace std;

#include <iostream>
#include <string>
#include <chrono>

using namespace std::chrono;

Scene::Scene() {

	#pragma region Set GUI

	int u0 = 20;
	int v0 = 40;
	int h = 800;
	int w = 1200;

	gui = new GUI();
	gui->show();
	gui->uiw->position(u0 + w + 50, v0);

	#pragma endregion

	plight = new Light(0.25f, 0.5f, this);
	projector = new Projector("geometry/Textures/blender.tiff", this);
	rm = new ResourceManager();

	#pragma region Load PPCs

	worldView = new WorldView(0, u0, v0, w, h, 90.0f, "Main");
	worldView->ppc->SetPose(Vector(0, 50, 50), Vector::ZERO, Vector::YAXIS);
	worldView->ppc->SetFocalLength(800);
	worldView->Show();

	views.push_back(new WorldView(0, u0, v0, w, h, 90.0f, "Render"));
	views[0]->ppc->SetPose(Vector(0, 0, 100), Vector::ZERO, Vector::YAXIS);
	views[0]->ppc->SetFocalLength(800);
	views[0]->renderMode = RenderMode::SceneRender;
		
	#pragma endregion

	InputHandler::Instatiate(worldView);

	rm->Load();

	envmap = new EnvMap(rm->cubemaps["skymap"]);

	Load();
	Render();
}

void Scene::Load() {

	rm->meshes["monkey"].SetCenter(Vector(0, 0, 0));
	rm->meshes["monkey"].SetScale(80);
	rm->meshes["monkey"].material = rm->materials["reflective"];

	plight->SetPosition(Vector(100, 100, 100));
	//projector->SetPostion(Vector(-100, 50, -100), Vector::ZERO);
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

		views[0]->ppc->RotateAround(Vector::ZERO, Vector::YAXIS, 360.0f / 600.0f);
		views[0]->ppc->RotateAround(Vector::ZERO, Vector::XAXIS, 360.0f / -600.0f);
		views[0]->ppc->RotateAround(Vector::ZERO, Vector::ZAXIS, 360.0f / 600.0f);

		Render();

		cerr << "Frame " << i << endl;

		string out = "animations/render_";
		out += to_string(i);
		out += ".tif";
		views[0]->fb->SaveAsTiff((char *) out.c_str());
	}
}