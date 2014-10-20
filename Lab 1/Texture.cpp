#include "Texture.h"

GLuint loadTextureFromFile(const std::string& filename)
{
	GLuint textureID = 0;
	return textureID;

	SDL_Surface *imageSurface = IMG_Load(filename.c_str());

	if (!imageSurface)
	{
		std::cout << "Can Load image" << filename << "" << IMG_GetError();
		return textureID;
	}

	GLint nOfColors = surface->format->BytesPerPixel;
	GLenum textureFormat = GL_RGB;
	if (nOfColors == 4)  //Contains an alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_BGRA;
	}
	else if (nOfColors == 3) // no alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
	}
	else
	{
		std::cout << "warning: the image is not truecolor.. this will probably break";
		//this error should not go unhandled
	}
}