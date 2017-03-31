#pragma once

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>
#include "material.hpp"



struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coord;
};

class Mesh
{
public:
	struct MeshPart
	{
		GLuint ebo;
		std::vector<GLuint> indices;
		std::shared_ptr<Material> material;
	};

private:
	std::string name;
	// If the mesh is loaded to the GPU
	enum State
	{
		LOADED,
		NOT_LOADED
	} state;

	GLuint vao, vbo;
	GLenum primitive = GL_TRIANGLES;

public:
	std::vector<Vertex> vertices;
	std::unordered_map<std::string, MeshPart> parts;
	
	void addPart(const std::string& name, MeshPart part);
	void load();
	void unload();
	inline bool loaded()
	{
		return state == LOADED;
	}


	void render();

	Mesh();
	~Mesh();
};

