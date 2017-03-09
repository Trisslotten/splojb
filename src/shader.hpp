#pragma once

#include <string>
#include <GL/glew.h>

class Shader
{
	GLuint program;

	std::string vert_path, geom_path, frag_path;

	GLint compileShader(GLenum type, std::string name);
public:
	inline GLuint getProgram();

	bool createProgram(const std::string vert, const std::string frag);
	bool createProgram(const std::string vert, const std::string geom, const std::string frag);

	void use();

	void reload();
	Shader();
	~Shader();
};

inline GLuint Shader::getProgram()
{
	return program;
}