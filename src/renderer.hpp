#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "mesh.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "entity.hpp"

class Renderer
{

private:

	std::vector<Entity> to_render;

	GLuint g_buffer, postprocess, depth;
	GLuint texture_position, texture_color, texture_normal, texture_velocity, texture_postprocess;

	GLuint quad_vao, quad_vbo;

	Shader shader_geometry;
	Shader shader_shading;
	Shader shader_postprocess;

	glm::mat4 prev_view_projection;

	Camera camera;
public:
	Renderer();
	~Renderer();

	void init();

	void update(double dt);


	// add to draw list (to_render)
	void draw(Entity& entity);


	// render everything in to_render
	void render(double dt);
};

