#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include "lodepng.h"

class Texture
{
	GLuint texture;
	std::vector<unsigned char> image;
	unsigned width, height;
public:
	void load(const std::string& file, bool mipmap = false);

	void bind(size_t slot);
};