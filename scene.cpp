#include "stdafx.h"

#include "scene.h"

Scene *scene;

using namespace std;

#include <iostream>
#include <string>

Scene::Scene() {

	int u0 = 20;
	int v0 = 20;
	int h = 480;
	int w = 640;

	meshCount = 5;
	cameraCount = 3;

	gui = new GUI();
	gui->show();
	gui->uiw->position(u0, v0 + h + 50);

	fb = new FrameBuffer(u0, v0, w, h, 0);
	fb->label("SW frame buffer");
	fb->SetBGR(0xFFFFFFFF);
	fb->show();
	fb->redraw();

	meshes = new TMesh[meshCount];
	cameras = new PPC*[cameraCount];

	mainCamera = new PPC(90.0f, fb->w, fb->h);
	cameras[0] = mainCamera;

	Load();
	Render();
}

void Scene::Load() {

	TMesh teapot57k = TMesh();
	TMesh teapot1k1 = TMesh();
	TMesh teapot1k2 = TMesh();
	TMesh teapot1k3 = TMesh();
	TMesh bunny = TMesh();

	PPC* camera1 = new PPC(90.0f, fb->w, fb->h);
	PPC* camera2 = new PPC(90.0f, fb->w, fb->h);

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

	cameras[1] = camera1;
	cameras[2] = camera2;

	mainCamera->SetPose(Vector(0, 100, 100), Vector::ZERO, Vector::YAXIS);
	camera1->SetPose(Vector(0, 100, 100), Vector::ZERO, Vector::YAXIS);
	camera2->SetPose(Vector(50, 100, 50), Vector::ZERO, Vector::YAXIS);
}

void Scene::DBG() {
	TMesh mesh = TMesh();

	mesh.LoadBin("geometry/teapot57k.bin");
	mesh.SetCenter(Vector(0, 0, 0));
	//mesh.SetScale(400);
	mesh.DrawMode = DrawMode::Filled;
	mainCamera->SetPose(Vector(0, 100, 100), Vector::ZERO, Vector::YAXIS);
	PPC* camera1 = new PPC(90.0f, fb->w, fb->h);
	PPC* camera2 = new PPC(90.0f, fb->w, fb->h);
	camera2->SetPose(Vector(100, 100, 100), Vector::ZERO, Vector::YAXIS);
	//mesh.SetToCube(Vector(0, 0, -100), 100, 0xFFFF0000, 0xFF000000);

	meshes[0] = mesh;
	//camera1->SetPose(Vector(100, 100, 0), mesh.GetCenter(), Vector::YAXIS);

	for (int i = 0; i < 360; i++) {
		//camera->RollLeft(1.0f);
		//meshes[0].Translate(Vector::XAXIS);
		//meshes[0].Rotate(meshes[0].GetCenter(), Vector::YAXIS, 1);
		mainCamera->Interpolate(camera1, camera2, i, 360);
		Fl::check();
		Render();
	}
}

void Scene::Render() {
	fb->SetBGR(0xFFFFFFFF);
	fb->ClearZB();

	for (int i = 0; i < meshCount; i++) {
		meshes[i].DrawMesh(fb, mainCamera);
	}

	fb->redraw();
}


void Scene::Demo() {
	int steps = 300;

	for (int i = 0; i < steps; i++) {
		
		if (i >= (steps / 2 - 1))
			mainCamera->Interpolate(cameras[1], cameras[2], i - (steps / 2 - 1), steps / 2);

		meshes[0].Rotate(meshes[0].GetCenter(), Vector(1, 1, 0), 1);
		meshes[1].Rotate(Vector::ZERO, Vector::YAXIS, 1);
		meshes[2].Rotate(Vector::ZERO, Vector::ZAXIS, 1.5f);
		meshes[3].Rotate(Vector::ZERO, Vector::XAXIS, 2.0f);
		meshes[4].Rotate(meshes[4].GetCenter(), Vector::YAXIS, 1);

		Fl::check();
		Render();

		cerr << "Frame " << i << endl;

		string out = "animations/rotation_";
		out += to_string(i);
		out += ".tif";
		fb->SaveAsTiff((char *) out.c_str());
	}
}