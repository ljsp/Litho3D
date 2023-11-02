#include "../includes/Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

Mesh::Mesh(Sphere& sphere, int nSubdivision)
{
	//TODO: Corriger l'affichage des triangles aux poles, 
	//		les normales et les coordonnées uv 
	
	float r = sphere.radius;
	glm::vec3 c = sphere.center;
	double PI = 3.14159265358;

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	int horizontalStep = nSubdivision;
	int verticalStep = nSubdivision;
	GLfloat x, y, z;

	// Ajout des 2 poles
	vertices.insert(vertices.end(), { c.x, c.y, c.z + r, 0.0f, 0.0f, c.x, c.y, c.z + r });
	//normals.push_back(glm::normalize(vertices.back()));
	
	vertices.insert(vertices.end(), { c.x, c.y, c.z - r, 1.0f, 1.0f, c.x, c.y, c.z - r });
	//normals.push_back(glm::normalize(vertices.back()));

	// h = 1 et h < horizontalStep - 1 car on a des ajouté les poles à la main avant
	// pour eviter d'avoir v meme point
	for (int h = 1; h < horizontalStep; h++) {
		for (int v = 0; v < verticalStep; v++) {

			x = sin(PI * (double)h / (double)horizontalStep) * cos(2 * PI * (double)v / (double)verticalStep) * r + c[0];
			y = sin(PI * (double)h / (double)horizontalStep) * sin(2 * PI * (double)v / (double)verticalStep) * r + c[1];
			z = cos(PI * (double)h / (double)horizontalStep) * r + c[2];

			vertices.insert(vertices.end(), { x, y, z, cos(x), sin(y), x, y, z});
			//normals.push_back(glm::normalize(vertices.back()));
		}
	}

	// Triangles des poles
	for (unsigned int v = 0; v < verticalStep - 1; v++) {
		indices.insert(indices.end(), { 0, v + 2, v + 3 });
		
		//AddSmoothTriangle(1, 1, Vertexes() - v - 1, Vertexes() - v - 1, Vertexes() - v - 2, Vertexes() - v - 2);
		indices.insert(indices.end(), { 1,  v + 1, v + 2 });
	}

	indices.insert(indices.end(), { 2, 0, (unsigned int)(verticalStep + 1) });

	//AddSmoothTriangle(1, 1, Vertexes() - verticalStep, Vertexes() - verticalStep, Vertexes() - 1, Vertexes() - 1);


	for (unsigned int h = 0; h < horizontalStep - 2; h++) {
		for (unsigned int v = 0; v < verticalStep; v++) {
			unsigned int v1 = h * verticalStep + v + 2;
			unsigned int v4 = h * verticalStep + (v + 1) % verticalStep + 2;
			unsigned int v2 = (h + 1) * verticalStep + v + 2;
			unsigned int v3 = (h + 1) * verticalStep + (v + 1) % verticalStep + 2;

			indices.insert(indices.end(), { v1, v2, v3 });
			indices.insert(indices.end(), { v4, v1, v3 });
		}
	}

	this->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
}

Mesh::Mesh(Plane& p)
{
	unsigned int floorIndices[]{
	0, 2, 1,
	1, 2, 3
	};

	GLfloat floorVertices[] = {
		//          x           y            z			    u     v			      nx	      ny	     nz
			p.pos.x - 10.0f, p.pos.y, p.pos.z - 10.0f,     0.0f, 0.0f,        p.normal.x, p.normal.y, p.normal.z,
			p.pos.x + 10.0f, p.pos.y, p.pos.z - 10.0f,     10.0f, 0.0f,       p.normal.x, p.normal.y, p.normal.z,
			p.pos.x - 10.0f, p.pos.y, p.pos.z + 10.0f,     0.0f, 10.0f,       p.normal.x, p.normal.y, p.normal.z,
			p.pos.x + 10.0f, p.pos.y, p.pos.z + 10.0f,     10.0f, 10.0f,      p.normal.x, p.normal.y, p.normal.z
	};

	this->CreateMesh(floorVertices, floorIndices, 32, 6);
}

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
	for (size_t i = 0; i < indiceCount; i += 3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] = normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] = normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] = normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

Mesh::Mesh(Pyramid& p)
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//    x      y      z        u     v       nx    ny    nz
			p.a.x, p.a.y, p.a.z,	0.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
			p.b.x, p.b.y, p.b.z,	0.5f, 0.0f,   0.0f, 0.0f, 0.0f,
			p.c.x, p.c.y, p.c.z,	1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
			p.d.x, p.d.y, p.d.z,	0.5f, 1.0f,   0.0f, 0.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	this->CreateMesh(vertices, indices, 32, 12);	
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

void Mesh::RenderMesh() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::ClearMesh() {
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}