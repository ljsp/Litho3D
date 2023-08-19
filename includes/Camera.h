#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch,GLfloat startSpeed, GLfloat startTurnSpeed);
	
	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	void scrollControl(GLfloat scrollX, GLfloat scrollY, GLfloat deltaTime);
	void setCameraPosition(glm::vec3 position);
	void setCameraOrientation(GLfloat yaw, GLfloat pitch);

	glm::vec3 getCameraPosition();
	
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;
	
	void update();
};

