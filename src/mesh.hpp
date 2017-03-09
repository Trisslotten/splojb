#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coord;
};

class Mesh
{
	std::string name;
	// If the mesh is loaded to the GPU
	enum State
	{
		LOADED,
		NOT_LOADED
	} state;

	GLuint vao, vbo, ebo;

	GLenum topology = GL_TRIANGLES;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
public:

	void load();
	void unload();

	void render();

	Mesh();
	~Mesh();
};

