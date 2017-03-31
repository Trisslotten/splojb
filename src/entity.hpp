#pragma once

#include <memory>
#include "mesh.hpp"
#include "glm/glm.hpp"

struct Entity
{
	std::shared_ptr<Mesh> mesh;
	glm::mat4 model, prev_model;

};