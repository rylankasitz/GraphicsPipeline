#include "stdafx.h"
#include "resourcemanager.h"
#include "texture.h"
#include "material.h"
#include "tmesh.h"
#include "cubemap.h"

using namespace std;

ResourceManager::ResourceManager() {

	textures = map<string, Texture>();
	materials = map<string, Material>();
	meshes = map<string, TMesh>();
	hasloaded = false;
}

void ResourceManager::Load() {

	if (hasloaded)
		return;

	AddTexture("Marble", "geometry/Textures/WhiteMarble.tif");
	AddTexture("blender", "geometry/Textures/blender.tiff");

	AddCubeMap("skymap", "geometry/Textures/uffizi_cross.tiff");

	AddMaterial("red", Material(CR_RED, false));
	AddMaterial("blue", Material(CR_BLUE, false));
	AddMaterial("green", Material(CR_GREEN, false));
	AddMaterial("yellow", Material(CR_YELLOW, false));
	AddMaterial("white", Material(CR_WHITE, false));
	AddMaterial("reflective", Material(CR_YELLOW, true));
	AddMaterial("marble", Material(CR_WHITE, textures["Marble"]));

	AddMesh("cube1", "geometry/cube.obj");
	AddMesh("cube2", "geometry/cube.obj");
	AddMesh("cube3", "geometry/cube.obj");
	AddMesh("ground", "geometry/plane.obj");
	AddMesh("arealight", "geometry/plane.obj");

	hasloaded = true;
}

#pragma region Loading Resources

void ResourceManager::AddTexture(string key, char* fname) {

	textures.insert(pair<string, Texture>(key, Texture(fname)));
}

void ResourceManager::AddMaterial(string key, Material mat) {

	materials.insert(pair<string, Material>(key, mat));
}

void ResourceManager::AddMesh(string key, char* fname) {

	TMesh mesh = TMesh();
	mesh.LoadObj(fname);
	meshes.insert(pair<string, TMesh>(key, mesh));
}

void ResourceManager::AddCubeMap(string key, char* fname)
{
	Texture tex = Texture(fname);
	CubeMap cubemap = CubeMap();
	cubemap.Load(tex);

	cubemaps.insert(pair<string, CubeMap>(key, cubemap));
}

#pragma endregion
