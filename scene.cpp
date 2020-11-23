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

	projector = new Projector("geometry/Textures/blender.tiff", this);
	rm = new ResourceManager();
	plight = new Light(0.25f, 0.5f, this);
	p1 = .5f;

	#pragma region Load PPCs

	worldView = new WorldView(0, u0, v0, w, h, 90.0f, "Main", this);
	worldView->ppc->SetPose(Vector(0, 100, -100), Vector::ZERO, Vector::YAXIS);
	worldView->ppc->SetFocalLength(800);
	worldView->drawMode = DrawMode::GPU;
	worldView->Show();
		
	#pragma endregion

	InputHandler::Instatiate(worldView);

	envmap = new EnvMap(rm->cubemaps["skymap"]);

	Fl::check();

	rm->Load();
	Load();
	Render();
}

void Scene::Load() {

	rm->meshes["cube1"].SetCenter(Vector(0, 0, 0));
	rm->meshes["cube1"].SetScale(40);
	rm->meshes["cube1"].SetMaterial(rm->materials["blue"]);
	rm->meshes["cube1"].DrawMode = MeshMode::Filled;

	rm->meshes["cube2"].SetCenter(Vector(-50, 0, 0));
	rm->meshes["cube2"].SetScale(40);
	rm->meshes["cube2"].SetMaterial(rm->materials["green"]);
	rm->meshes["cube2"].DrawMode = MeshMode::Filled;

	rm->meshes["cube3"].SetCenter(Vector(50, 0, 0));
	rm->meshes["cube3"].SetScale(40);
	rm->meshes["cube3"].SetMaterial(rm->materials["red"]);
	rm->meshes["cube3"].DrawMode = MeshMode::Filled;

	rm->meshes["ground"].SetCenter(Vector(0, -20, 0));
	rm->meshes["ground"].SetScale(300);
	rm->meshes["ground"].SetMaterial(rm->materials["white"]);
	rm->meshes["ground"].DrawMode = MeshMode::Filled;

	rm->meshes["arealight"].SetCenter(Vector(0, 50, 100));
	rm->meshes["arealight"].Rotate(rm->meshes["arealight"].GetCenter(), Vector::XAXIS, 90.0f);
	rm->meshes["arealight"].SetScale(100);
	rm->meshes["arealight"].SetMaterial(rm->materials["yellow"]);
	rm->meshes["arealight"].DrawMode = MeshMode::WireFrame;
}

void Scene::Render() {
	
	worldView->Render(this);
	Fl::check();
}

void Scene::DBG() {
	Render();
}

void Scene::Demo() {

	shadowDemo();
	//hDemo();
}

void Scene::shadowDemo()
{
	int steps = 900;

	InputHandler::SetEnabled(false);
	worldView->ppc->SetPose(Vector(0, 100, -100), Vector::ZERO, Vector::YAXIS);
	for (int i = 0; i < steps; i++) {

		if (i < 200)
		{
			rm->meshes["cube2"].Translate(Vector(1, 0, 1) * .25f);
			rm->meshes["cube3"].Translate(Vector(-1, 0, -1) * .25f);
		}
		else if (i < 400)
		{
			worldView->ppc->RotateAround(Vector::ZERO, Vector::YAXIS, 270.0f / 200.0f);
		}
		else if (i < 800)
		{
			rm->meshes["arealight"].Rotate(Vector::ZERO, Vector::YAXIS, 360.0f / 400.0f);
		}

		Render();

		//cerr << "Frame " << i << endl;

		string out = "animations/render_";
		out += to_string(i);
		out += ".tif";
		//worldView->fb->SaveAsTiff((char*)out.c_str());
	}
}

void Scene::hDemo()
{
	int steps = 900;

	InputHandler::SetEnabled(false);
	worldView->ppc->SetPose(Vector(0, 100, -100), Vector::ZERO, Vector::YAXIS);
	worldView->drawMode = DrawMode::HardWare;

	rm->meshes["cube1"].SetMaterial(rm->materials["marble"]);
	rm->meshes["cube2"].DrawMode = MeshMode::WireFrame;

	Vector c;
	c.SetFromColor(CR_GREEN);
	rm->meshes["cube3"].colorsArr[0] = c;
	rm->meshes["cube3"].colorsArr[1] = c;
	rm->meshes["cube3"].colorsArr[2] = c;

	for (int i = 0; i < steps; i++) {

		if (i < 200)
		{
			rm->meshes["cube2"].Translate(Vector(1, 0, 1) * .25f);
			rm->meshes["cube3"].Translate(Vector(-1, 0, -1) * .25f);
		}
		else if (i < 400)
		{
			worldView->ppc->RotateAround(Vector::ZERO, Vector::YAXIS, 270.0f / 200.0f);
		}
		else if (i < 800)
		{
			rm->meshes["arealight"].Rotate(Vector::ZERO, Vector::YAXIS, 360.0f / 400.0f);
		}

		Render();

		cerr << "Frame " << i << endl;

		string out = "animations/render_";
		out += to_string(i);
		out += ".tif";
		worldView->fb->SaveAsTiff((char*)out.c_str());
	}
}