#ifndef VERTEX_H
#define VERTEX_H

#include "glm/glm.hpp"
struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	float miniShaderId = 0;
	glm::vec2 texCoords;
	float textureGroupId = -1;
	int modelId = 0;
	int modelVertexId = 0;
	int order = -1;
	bool isDeleted = false;
};

#endif