#include "engine.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "logger.hpp"
#include <time.h>
#include "simplexnoise1234.h"

void Engine::init()
{
	Log::debugln("Init Engine");

	renderer.init();

	//assets.loadPNG("assets/ground.png");
	assets.loadMesh("assets/cone.obj");
	assets.loadMesh("assets/plane/plane.obj");

	for (auto&& map_elem : assets.meshes)
	{
		map_elem.second->load();
	}
	entities.resize(2000);



	for (auto&& e : entities)
	{
		e.model = glm::mat4();
		e.prev_model = glm::mat4();
		e.mesh = assets.meshes["assets/cone.obj"];
	}
	entities[0].mesh = assets.meshes["assets/plane/plane.obj"];
}
void Engine::update(double dt)
{
	timer += dt;
	gt += dt;
	if (timer > 1)
	{
		timer = 0;
	}

	renderer.update(dt);
}

void Engine::render(double dt)
{

	float x = 0, y = 0;
	for (auto&& e : entities)
	{
		glm::mat4 model = glm::translate(glm::mat4(), 3.f*glm::vec3(x, 0, y));
		e.model = model;//glm::rotate(model, (float)glm::sin(global_time + x)*3, glm::vec3(sin(global_time + x), cos(global_time+y), 0));
		renderer.draw(e);
		e.prev_model = e.model;
		x += 1;
		if (x > 10)
		{
			x = 0;
			y += 1;
		}
	}


	renderer.render(dt);
}
