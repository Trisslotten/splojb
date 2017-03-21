#include "mesh.hpp"
#include "logger.hpp"
#include <time.h>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

void Mesh::addPart(const std::string & name, MeshPart part)
{
	parts[name] = part;
}

void Mesh::load()
{
	if (state == NOT_LOADED)
	{
		Log::debugln("Loading mesh");
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
		// Normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(3 * sizeof(GLfloat)));
		// Tex coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(6 * sizeof(GLfloat)));

		for (auto&& map_part : parts)
		{
			MeshPart& mesh_part = map_part.second;
			glGenBuffers(1, &mesh_part.ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_part.ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mesh_part.indices.size(), &mesh_part.indices[0], GL_STATIC_DRAW);
		}
		glBindVertexArray(0);

		state = LOADED;
	}
}

void Mesh::unload()
{	
	Log::debugln("Unloading mesh");
	glDeleteBuffers(1, &vbo);
	for (auto&& map_part : parts)
	{
		glDeleteBuffers(1, &map_part.second.ebo);
	}
	glDeleteVertexArrays(1, &vao);
	state = NOT_LOADED;
}

void Mesh::render()
{
	if (state == NOT_LOADED)
	{
		Log::warningln("Mesh '" + name + "' not loaded to GPU. Skipping");
		return;
	}

	glBindVertexArray(vao);
	for (auto&& map_part : parts)
	{
		MeshPart& mesh_part = map_part.second;
		if(mesh_part.material->albedo)
			mesh_part.material->albedo->bind(4);
		if(mesh_part.material->normal)
			mesh_part.material->normal->bind(5);
		if (mesh_part.material->specular)
			mesh_part.material->specular->bind(6);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_part.ebo);
		glDrawElements(GL_TRIANGLES, mesh_part.indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

Mesh::Mesh()
{
	state = NOT_LOADED;
}


Mesh::~Mesh()
{
	unload();
}