#include "glm/glm.hpp"
struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	float textureGroupId = 0;
	int modelId = 0;
	int modelVertexId = 0;
	int order = -1;
	bool isDeleted = false;
};
