#include "texture.hpp"
#include "debug.hpp"
#include "logger.hpp"

void Texture::load(const std::string& file, bool mipmap)
{
	Log::debugln("loading '" + file);
	unsigned error = lodepng::decode(image, width, height, file);
	if (error != 0)
	{
		Log::errorln(std::to_string(error) + ": " + lodepng_error_text(error));
		pauseExit();
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// TODO: check if correct
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	image.clear();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(size_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}