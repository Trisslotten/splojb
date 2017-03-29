#pragma once

#include "simplexnoise1234.h"
#include <memory>
#include "mesh.hpp"
#include <time.h>

namespace {
	unsigned int _BLOCKS = 0;
	unsigned int CREATED = 0;
}
template<int x_size, int y_size, int z_size>
bool isFull(char(&raw_data)[x_size][y_size][z_size], int x_start, int x_end, int y_start, int y_end, int z_start, int z_end)
{

	for (int x = x_start; x == x_end; x++)
	{
		for (int y = y_start; y == y_end; y++)
		{
			for (int z = z_start; z == z_end; z++)
			{
				if (raw_data[x][y][z] == 0)
				{
					return false;
				}
			}
		}
	}
	_BLOCKS += glm::pow(x_end - x_start, 3) - 1;
	return true;
}



const int GRID_SIZE = 128;
const double SCALE = 0.03;

class VoxelOctree
{
	struct Node
	{
		Node* children[2][2][2];
		//bool filled = 0;
		//glm::vec3 color;
	};

	Node* root;

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;

public:

	void createFace(std::vector<GLuint>& _indices, GLuint a, GLuint b, GLuint c, GLuint d, GLuint offset)
	{
		_indices.push_back(a + offset);
		_indices.push_back(b + offset);
		_indices.push_back(d + offset);

		_indices.push_back(a + offset);
		_indices.push_back(d + offset);
		_indices.push_back(c + offset);
	}

	int counter = 0;
	void createCube(int x_start, int x_end, int y_start, int y_end, int z_start, int z_end)
	{
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					vertices.push_back(glm::vec3(x_start + x*(x_end - x_start),
									   y_start + y*(y_end - y_start),
									   z_start + z*(z_end - z_start)));
				}
			}
		}
		GLuint offset = 8 * counter;
		counter++;
		createFace(indices, 0, 1, 2, 3, offset); // Front
		createFace(indices, 0, 4, 1, 5, offset); // Top
		createFace(indices, 0, 2, 4, 6, offset); // Left
		createFace(indices, 1, 5, 3, 7, offset); // Right
		createFace(indices, 2, 3, 6, 7, offset); // Bottom
		createFace(indices, 5, 4, 7, 6, offset); // Back

		CREATED++;
	}

	// TODO: rewrite to iterative version
	template<int x_size, int y_size, int z_size>
	void buildTreeRecursive(Node* node, char(&raw_data)[x_size][y_size][z_size], int _x_start, int _x_end, int _y_start, int _y_end, int _z_start, int _z_end)
	{


		// Note: not real center, misleading! rename to offset sometime, maybe never...
		int x_center = (_x_end - _x_start) / 2;
		int y_center = (_y_end - _y_start) / 2;
		int z_center = (_z_end - _z_start) / 2;

		if (_x_start == _x_end - 1)
		{
			if (raw_data[_x_start][_y_start][_z_start] == 1)
				createCube(_x_start, _x_end, _y_start, _y_end, _z_start, _z_end);
			return;
		}

		for (int x = 0; x < 2; x++)
		{
			int x_start = _x_start + x*x_center;
			int x_end = _x_end - (1 - x)*x_center;
			for (int y = 0; y < 2; y++)
			{
				int y_start = _y_start + y*y_center;
				int y_end = _y_end - (1 - y)*y_center;
				for (int z = 0; z < 2; z++)
				{
					int z_start = _z_start + z*z_center;
					int z_end = _z_end - (1 - z)*z_center;
					node->children[x][y][z] = new Node();
					//std::cout << "x: " << x_start << " -> " << x_end << "\n";
					//std::cout << "y: " << y_start << " -> " << y_end << "\n";
					//std::cout << "z: " << z_start << " -> " << z_end << "\n";
					if (!isFull(raw_data, x_start, x_end, y_start, y_end, z_start, z_end))
					{
						//std::cout << "not filled\n\n";
						//node->filled = false;
						buildTreeRecursive(node->children[x][y][z], raw_data, x_start, x_end, y_start, y_end, z_start, z_end);
					}
					else
					{
						//std::cout << "Is filled!!!!!!!!!!!\n\n";
						//node->children[x][y][z]->filled = true;
						createCube(x_start, x_end, y_start, y_end, z_start, z_end);
					}
				}
			}
		}
	}


	void buildTree()
	{
		srand(time(NULL));
		double offset = rand()*0.5;
		counter = 0;
		char raw_data[GRID_SIZE][GRID_SIZE][GRID_SIZE];

		for (int x = 0; x < GRID_SIZE; x++)
		{
			for (int y = 0; y < GRID_SIZE; y++)
			{
				for (int z = 0; z < GRID_SIZE; z++)
				{
					double value = SimplexNoise1234::noise(x*SCALE, y*SCALE, z*SCALE + offset);
					if (value > -0.1)
					{
						raw_data[x][y][z] = 1;
					}
					else
					{
						raw_data[x][y][z] = 0;
					}
				}
			}
		}

		root = new Node();
		buildTreeRecursive(root, raw_data, 0, GRID_SIZE, 0, GRID_SIZE, 0, GRID_SIZE);

		std::cout << "Blocks created: " << CREATED << "\n";
		std::cout << "Blocks saved:   " << _BLOCKS << "\n";
		std::cout << "Blocks can fit: " << GRID_SIZE*GRID_SIZE*GRID_SIZE << "\n";

	}

	std::shared_ptr<Mesh> createMesh()
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		for (auto pos : vertices)
		{
			Vertex v;
			v.position = pos;
			mesh->vertices.push_back(v);
		}
		Mesh::MeshPart part;
		for (auto index : indices)
		{
			part.indices.push_back(index);
		}


		mesh->addPart("mesh1", part);

		return mesh;
	}

};
