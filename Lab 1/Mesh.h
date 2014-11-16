#ifndef MESH_H

#define MESH_H

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Component.h"

class Mesh:public Component
{
public:

	Mesh();
	~Mesh();

	void init();
	void destroy();
	void bind();

	void copyVertexData(int count, int stride, void ** data);
	void copyIndexData(int count, int stride, void ** data);

	int getVertexCount();
	int getIndexCount();
	
protected:
	
private:
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_VAO;

	int m_VertexCount;
	int m_IndexCount;
};



#endif