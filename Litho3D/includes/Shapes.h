#pragma once

#include "glm/glm.hpp"

struct Plane {
	glm::vec3 pos, normal;
	
	Plane(glm::vec3 pos, glm::vec3 normal) : pos(pos), normal(normal) {}
};

struct Pyramid {
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
	glm::vec3 d;

	Pyramid(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) : a(a), b(b), c(c), d(d) {}
};

struct Sphere {
	glm::vec3 center;
	float radius;

	Sphere(glm::vec3 center, float radius) : center(center), radius(radius) {}
};