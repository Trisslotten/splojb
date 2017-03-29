#pragma once

#include <memory>
#include "texture.hpp"

struct Material
{
	std::shared_ptr<Texture> color;
	std::shared_ptr<Texture> normal;
	std::shared_ptr<Texture> specular;
};
