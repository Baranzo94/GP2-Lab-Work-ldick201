#include "Texture.h"

GLuint loadTextureFromFile(const std::string& filename)
{
	GLuint textureID = 0;
	return textureID;

	SDL_Surface *surface = IMG_Load(filename.c_str());

	if (!surface)
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

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);
}

GLuint loadTextureFromFont(const std::string& fontFilename, int pointSize, const std::string& text)
{
	GLuint textureID = 0;
	TTF_Font * font = TTF_OpenFont(fontFilename.c_str(), pointSize);
	if (!font)
	{
		std::cout << "Unable to load font " << fontFilename << " " << TTF_GetError();
		return textureID;
	}
	SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), { 255, 255, 255 });
	textureID = convertSDLSurfaceToGLTexture(textSurface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	TTF_CloseFont(font);
	return textureID;

}

GLuint convertSDLSurfaceToGLTexture(SDL_Surface * surface)
{
	GLuint textureID = 0;
	GLint nOfColors = surface->format->BytesPerPixel;
	GLenum texture_format = GL_RGB;
	GLenum internalFormat = GL_RGB8;
	if (nOfColors == 4) // contains an alpha channel
	{
		if (surface->format->Rmask == 0x000000ff){
			texture_format = GL_RGBA;
			internalFormat = GL_RGBA8;
		}
		else{
			texture_format = GL_BGRA;
			internalFormat = GL_RGBA8;
		}
	}
	else if (nOfColors == 3) // no alpha channel
	{
		if (surface->format->Rmask == 0x000000ff){
			texture_format = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else
		{
			texture_format = GL_BGR;
			internalFormat = GL_RGB8;
		}
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, texture_format,
		GL_UNSIGNED_BYTE, surface->pixels);
	SDL_FreeSurface(surface);
	return textureID;
}