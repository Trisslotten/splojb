#pragma once

#include "renderer.hpp"
#include "assets.hpp"

class Engine
{
	Renderer renderer;

	Assets assets;
	
	double angle;
public:
	void init();
	void update(double dt);
	void render();
};
