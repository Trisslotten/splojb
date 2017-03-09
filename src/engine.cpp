#include "engine.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "logger.hpp"


void Engine::init()
{
	Log::debugln("Init Engine");

	renderer.init();
	mesh = std::make_shared<Mesh>();
	mesh->load();
}

void Engine::update(double dt)
{
	angle += dt*0.1f;
	


}

void Engine::render()
{
	//renderer.draw(stuff) here

	glm::mat4 model = glm::rotate(glm::mat4(), (GLfloat)angle, glm::vec3(0,1,0));
	renderer.draw(mesh, nullptr, model);

	renderer.render();
}
