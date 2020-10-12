#pragma once

#define TEXTURE_DIRECTORY "geometry/Textures/"

#pragma region Colors

#define CR_RED		0xFFFF0000
#define CR_BLUE		0xFF0000FF
#define CR_WHITE	0xFFFFFFFF
#define CR_GREEN	0xFF00FF00
#define CR_BLACK	0xFF000000

#pragma endregion

#include <map>
#include <string>

class TMesh;
class Texture;
class Material;

class ResourceManager
{
public:
	std::map<std::string, Texture> textures;
	std::map<std::string, Material> materials;
	std::map<std::string, TMesh> meshes;

	ResourceManager();
	void Load();
	void AddTexture(std::string key, char* fname);
	void AddMaterial(std::string key, Material mat);
	void AddMesh(std::string key, char* fname);

private:

};