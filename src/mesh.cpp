#include "mesh.hpp"
#include "logger.hpp"
#include <time.h>


#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

void Mesh::load()
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(3*sizeof(GLfloat)));
	// Tex coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(6*sizeof(GLfloat)));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	state = LOADED;
}

void Mesh::unload()
{
	Log::debugln("Unloading mesh");
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
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
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

float r()
{
	return ((rand() % 2000) / 1000.0-1);
}

Mesh::Mesh()
{
	state = NOT_LOADED;

	Log::debugln("Creating random mesh");


	srand(time(NULL));
	int size = 3*10;// rand() % 100 + 10;
	for (int i = 0; i < size; i++)
	{
		Vertex v;
		v.position = glm::vec3(5*r(), 2*r(), 2*r());
		vertices.push_back(v);

		indices.push_back((GLuint)(rand() % size));
		//std::cout << v.position.x << "\n";
		//std::cout << indices[i * 3] << " " << indices[i * 3+1] << " " << indices[i * 3+2] << "\n";
	}

	for (int i = 0; i < size-3; i+=3)
	{
		glm::vec3 p1 = vertices[indices[i]].position;
		glm::vec3 p2 = vertices[indices[i +1]].position;
		glm::vec3 p3 = vertices[indices[i +2]].position;

		glm::vec3 v1 = p2 - p1;
		glm::vec3 v2 = p3 - p1;
		glm::vec3 normal = cross(v1, v2);
		vertices[indices[i ]].normal = normal;
		vertices[indices[i +1]].normal = normal;
		vertices[indices[i +2]].normal = normal;
	}
}


Mesh::~Mesh()
{


	if (state == LOADED)
	{
		unload();
	}
}
