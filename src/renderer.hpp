#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "mesh.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "camera.hpp"

class Renderer
{

public:
	struct Transforms
	{
		glm::mat4 transform;
		glm::mat4 old;
		float dt;
	};

private:
	struct RenderObject
	{
		Transforms transforms;
		std::shared_ptr<Material> material;
		std::shared_ptr<Mesh> mesh;
	};

	std::vector<RenderObject> to_render;

	GLuint g_buffer, postprocess, depth;
	GLuint texture_position, texture_color, texture_normal, texture_velocity, texture_postprocess;

	GLuint quad_vao, quad_vbo;

	Shader shader_geometry;
	Shader shader_shading;
	Shader shader_postprocess;

	Camera camera;
public:
	Renderer();
	~Renderer();

	void init();

	void update(double dt);


	// add to draw list (to_render)
	void draw(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, Transforms transforms);


	// render everything in to_render
	void render();
};

