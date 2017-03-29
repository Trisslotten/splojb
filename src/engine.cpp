#include "engine.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "logger.hpp"
#include <time.h>
#include "simplexnoise1234.h"

double _dt;
glm::mat4 old;


void Engine::init()
{
	Log::debugln("Init Engine");

	renderer.init();

	//assets.loadPNG("assets/ground.png");
	assets.loadMesh("assets/cone.obj");

	for (auto&& map_elem : assets.meshes)
	{
		map_elem.second->load();
	}
	
	old = glm::mat4();
}
double global_time = 0;
double timer = 0;
void Engine::update(double dt)
{
	timer += dt;
	global_time += dt;
	if (timer > 1)
	{
		timer = 0;
	}
	_dt = dt;

	renderer.update(dt);
}

void Engine::render()
{
	//renderer.draw(stuff) here
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(sin(global_time*3), 0.0*sin(global_time * 2.3),0));;
	model = glm::rotate(model, (GLfloat)sin(global_time*0.6)*15, glm::vec3(sin(global_time), cos(global_time),0));
	//model = glm::rotate(model, (GLfloat)sin(global_time*0.05) * 20, glm::vec3(0, 1, 0));
	for (auto&& map_elem : assets.meshes)
	{
		renderer.draw(map_elem.second, nullptr, {model, old, (float)_dt});
	}
	old = model;


	renderer.render();
}
