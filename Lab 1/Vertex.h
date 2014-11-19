#ifndef VERTEX_H

#define VERTEX_H

#include <glm/glm.hpp>
using glm::vec4;
using glm::vec3;
using glm::vec2;

struct Vertex
{
	vec3 position;
	vec2 texCoords;
	vec4 colours;
	vec3 normal;
};

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#endif