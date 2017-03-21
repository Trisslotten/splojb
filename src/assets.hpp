#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "texture.hpp"
#include "mesh.hpp"

class Assets
{
	

	std::shared_ptr<Texture> loadTextureIfNotLoaded(const std::string& filepath);
public:
	std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;


	void loadMesh(const std::string& filepath);

};