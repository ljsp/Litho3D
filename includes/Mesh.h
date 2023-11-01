#pragma once

#include <vector>

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "Shapes.h"

class Mesh
{
public:
	Mesh();
	Mesh(Plane& plane);
	Mesh(Pyramid& pyramid);
	Mesh(Sphere& sphere, int nSubdivision);
	
	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);

	void RenderMesh();
	void ClearMesh();
	
	~Mesh();
	
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

