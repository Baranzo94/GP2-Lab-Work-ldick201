#ifndef GAMEOBJECT_H

#define GAMEOBJECT_H

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Component.h"

class GameObject
{
public:
	GameObject();
	~GameObject();
	void init();
	void update();
	void render();
	void destroy();
	const std::string& getName();
	const std::string& setName();
	void addComponent(Component*component);
	
	//void toggleActive();
	//bool isActive();

protected:
	//std::string m_Type;
	//bool m_Active;
private:
	std::vector<Component*> m_Components;
	const std::string m_Name;
	
};



#endif