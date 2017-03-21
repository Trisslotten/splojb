#include "engine.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "logger.hpp"


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

}

double timer = 0;
void Engine::update(double dt)
{
	angle += dt*0.1f;
	timer += dt;

	if (timer > 1)
	{
		timer = 0;
	}



	renderer.update();
}

void Engine::render()
{
	//renderer.draw(stuff) here
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3());;
	model = glm::rotate(model, (GLfloat)angle, glm::vec3(0,1,0));
	
	for (auto&& map_elem : assets.meshes)
	{
		renderer.draw(map_elem.second, nullptr, model);
	}

	//renderer.draw(mesh, nullptr, model);

	renderer.render();
}
