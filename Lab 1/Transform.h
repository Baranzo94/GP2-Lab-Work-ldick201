#ifndef TRANSFORM_H

#define TRANSFORM_H

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

#include "Component.h"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform : public Component
{
public:
	Transform();
	~Transform();

	void update();

	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	vec3& getPosition();
	vec3& getRotation();
	vec3& getScale();
	
	mat4& getModel();

protected:

private:

	vec3 m_Position;
	vec3 m_Scale;
	vec3 m_Rotation;
	mat4 m_model;

};



#endif