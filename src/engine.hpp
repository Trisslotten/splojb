#pragma once

#include "renderer.hpp"
#include "assets.hpp"
#include "voxeloctree.hpp"
#include "camera.hpp"
#include "entity.hpp"

class Engine
{
	Renderer renderer;

	Assets assets;

	std::vector<Entity> entities;


	double gt = 0;
	double timer = 0;

public:
	void init();
	void update(double dt);
	void render(double dt);
};
