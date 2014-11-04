#ifndef GAMEOBJECT_H

#define GAMEOBJECT_H

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
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

	void addComponent(Component*component);
	void setName(const std::string& name);
	const std::string& getName();
	


protected:

private:
	std::vector<Component*> m_Components;
	std::string m_Name;
	
};



#endif