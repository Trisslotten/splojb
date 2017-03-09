#pragma once

#include "renderer.hpp"

class Engine
{
	Renderer renderer;

	std::shared_ptr<Mesh> mesh;
	double angle;
public:
	void init();
	void update(double dt);
	void render();
};
