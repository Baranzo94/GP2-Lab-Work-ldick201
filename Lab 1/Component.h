#ifndef COMPONENT_H

#define COMPONENT_H

//#include <GL/glew.h>
//#include <SDL_opengl.h>
//#include <SDL_image.h>
//#include <iostream>
#include <string>

class GameObject;

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

		void setParent(GameObject * object);

protected:
		std::string m_Type;
		bool m_Active;
		GameObject * m_Parent;
private:
};



#endif