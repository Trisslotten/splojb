#include "shader.hpp"

#include <fstream>
#include <vector>
#include <iostream>
#include "logger.hpp"

GLint Shader::compileShader(GLenum type, std::string name)
{
	GLuint shader = glCreateShader(type);
	std::ifstream shaderFile(name);
	if (shaderFile.is_open())
	{
		std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		shaderFile.close();
		const char* shaderTextPtr = shaderText.c_str();

		glShaderSource(shader, 1, &shaderTextPtr, nullptr);
		glCompileShader(shader);


		// Check for compile error
		GLint success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint log_size = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
			std::vector<GLchar> error(log_size);
			glGetShaderInfoLog(shader, log_size, &log_size, &error[0]);
			std::string errorstr{ &error[0] };

			Log::errorln("\n" + errorstr);

			glDeleteShader(shader);
			Log::warningln("Shader '" + name + "' could not compile");
			return -1;
		}

		return (GLint)shader;
	}
	return -1;
}

bool Shader::createProgram(const std::string vert, const std::string frag)
{
	vert_path = vert;
	frag_path = frag;

	GLint vs = compileShader(GL_VERTEX_SHADER, vert);
	GLint fs = compileShader(GL_FRAGMENT_SHADER, frag);
	if (vs == -1 || fs == -1)
		return false;

	program = glCreateProgram();
	glAttachShader(program, fs);
	glAttachShader(program, vs);
	glLinkProgram(program);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint log_size = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
		std::vector<GLchar> error(log_size);
		glGetProgramInfoLog(program, log_size, &log_size, &error[0]);
		std::string errorstr{ &error[0] };

		Log::errorln("\n" + errorstr);

		glDeleteProgram(program);
		Log::warningln("Program '" + vert + "' could not link");
		return false;
	}

	return true;
}

bool Shader::createProgram(const std::string vert, const std::string geom, const std::string frag)
{
	vert_path = vert;
	geom_path = geom;
	frag_path = frag;

	GLint vs = compileShader(GL_VERTEX_SHADER, vert);
	GLint gs = compileShader(GL_GEOMETRY_SHADER, geom);
	GLint fs = compileShader(GL_FRAGMENT_SHADER, frag);
	if (vs == -1 || gs == -1 || fs == -1)
		return false;

	program = glCreateProgram();
	glAttachShader(program, fs);
	glAttachShader(program, gs);
	glAttachShader(program, vs);
	glLinkProgram(program);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint log_size = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
		std::vector<GLchar> error(log_size);
		glGetProgramInfoLog(program, log_size, &log_size, &error[0]);
		std::string errorstr{ &error[0] };

		Log::errorln("\n" + errorstr);

		glDeleteProgram(program);
		Log::warningln("Program '" + vert + "' could not link");
		return false;
	}

	return true;
}

void Shader::use()
{
	glUseProgram(program);
}


void Shader::reload()
{
	Log::debugln("Reloading shader: '" + this->vert_path + "', '" + this->frag_path + "'");
	GLuint old_program = program;
	bool success;
	if (geom_path.size() > 0)
	{
		success = createProgram(vert_path, geom_path, frag_path);
	}
	else
	{
		success = createProgram(vert_path, frag_path);
	}
	if (success)
	{
		glDeleteProgram(old_program);
	}
	else
	{
		Log::warningln("Using old shader");
	}
}

Shader::Shader()
{}

Shader::~Shader()
{}