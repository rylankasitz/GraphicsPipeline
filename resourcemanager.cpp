#include "stdafx.h"
#include "resourcemanager.h"
#include "texture.h"
#include "material.h"
#include "tmesh.h"

using namespace std;

ResourceManager::ResourceManager() {

	textures = map<string, Texture>();
	materials = map<string, Material>();
	meshes = map<string, TMesh>();
}

void ResourceManager::Load() {

	AddTexture("Marble", "geometry/Textures/WhiteMarble.tif");

	AddMaterial("red", Material(CR_RED));
	AddMaterial("blue", Material(CR_BLUE));
	AddMaterial("green", Material(CR_GREEN));

	AddMesh("cube", "geometry/cube.obj");
	AddMesh("monkey", "geometry/monkey.obj");
	AddMesh("terrain", "geometry/terrain.obj");
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

#pragma endregion
