#ifndef MATERIAL_H
#define MATERIAL_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

using glm::vec4;

#include "Component.h"

class Material :public Component
{
public:
	Material();
	~Material();
	void destroy();
	void bind();
	bool loadShader(const std::string& vsFilename, const std::string& fsFilename);
	GLint getUniformLocation(const std::string& name);

	vec4& getAmbientColour();
	void setAmbientColour(float r, float g, float b, float a);

	vec4& getDiffuseColour();
	void setDiffuseColour(float r, float g, float b, float a);

	vec4& getSpecularColour();
	void setSpecularColour(float r, float g, float b, float a);

	float getSpecularPower();
	void setSpecularPower(float power);
protected:

private:
	GLuint m_ShaderProgram;
	vec4 m_AmbientColour;
	vec4 m_DiffuseColour;
	vec4 m_SpecularColour;
	float m_SpecularPower;
};
#endif