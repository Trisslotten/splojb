#pragma once

#include "renderer.hpp"
#include "assets.hpp"
#include "voxeloctree.hpp"
#include "camera.hpp"

class Engine
{
	Renderer renderer;

	Assets assets;

public:
	void init();
	void update(double dt);
	void render();
};
