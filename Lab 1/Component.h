#ifndef COMPONENT_H

#define COMPONENT_H

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

class Component
{
public:
		Component();
		virtual ~Component();
		virtual void init();
		virtual void update();
		virtual void render();
		virtual void destroy();
		const std::string& getType();
		void toggleActive();
		bool isActive();

protected:
		std::string m_Type;
		bool m_Active;
private:
};



#endif