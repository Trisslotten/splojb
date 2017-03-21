#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "mesh.hpp"
#include "material.hpp"
#include "shader.hpp"

class Renderer
{
	struct RenderObject
	{
		glm::mat4 transform;
		std::shared_ptr<Material> material;
		std::shared_ptr<Mesh> mesh;
	};

	std::vector<RenderObject> to_render;

	Shader shader;
	Shader depth_shader;

public:
	Renderer();
	~Renderer();

	void init();

	void update();

	// add to draw list (to_render)
	void draw(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::mat4 transform);


	// render everything in to_render
	void render();
};

