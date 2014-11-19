#include "Transform.h"
#include "GameObject.h"

Transform::Transform()
{
	m_Position = vec3(0.0f, 0.0f, 0.0f);
	m_Rotation = vec3(0.0f, 0.0f, 0.0f);
	m_Scale = vec3(1.0f, 1.0f, 1.0f);
	m_model = mat4();
}

Transform::~Transform()
{

}

void Transform::update()
{
	mat4 translate = glm::translate(translate, m_Position);
	mat4 scale = glm::scale(scale, m_Scale);

	mat4 rotationX = glm::rotate(rotationX, m_Rotation.x, vec3(1.0f, 0.0f, 0.0f));
	mat4 rotationY = glm::rotate(rotationY, m_Rotation.y, vec3(0.0f, 1.0f, 0.0f));
	mat4 rotationZ = glm::rotate(rotationZ, m_Rotation.z, vec3(0.0f, 0.0f, 1.0f));
	mat4 rotation = rotationX*rotationY*rotationZ;

	m_model = translate*rotation*scale;
	if (m_Parent->getParent())
	{
		m_model = m_Parent->getParent()->getTransform()->getModel()*m_model;
	}
}

void Transform::setPosition(float x, float y, float z)
{
	m_Position = vec3(x, y, z);
}
void Transform::setRotation(float x, float y, float z)
{
	m_Rotation = vec3(x, y, z);
}
void Transform::setScale(float x, float y, float z)
{
	m_Scale = vec3(x, y, z);
}
vec3& Transform::getPosition()
{
	return m_Position;
}
vec3& Transform::getScale()
{
	return m_Scale;
}
vec3& Transform::getRotation()
{
	return m_Rotation;
}
mat4& Transform::getModel()
{
	return m_model;
}