#include "../includes/Camera.h"

Camera::Camera() {
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startSpeed, GLfloat startTurnSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	
	moveSpeed = startSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {

	GLfloat velocity = moveSpeed * deltaTime;
	
	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}
	
	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
	}
	
	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}

	if (keys[GLFW_KEY_SPACE]) {
		position += up * velocity;
	}

	if (keys[GLFW_KEY_LEFT_SHIFT]) {
		position -= up * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;
	
	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	update();
}

void Camera::scrollControl(GLfloat scrollX, GLfloat scrollY, GLfloat deltaTime) {

	GLfloat velocity = moveSpeed * deltaTime * 10.0f;

	if (scrollX > 0) {
		position += right * velocity;
	}

	if (scrollX < 0) {
		position -= right * velocity;
	}

	if (scrollY > 0) {
		position += front * velocity;
	}

	if (scrollY < 0) {
		position -= front * velocity;
	}
}

void Camera::setCameraPosition(glm::vec3 position)
{
	this->position = position;
}

void Camera::setCameraOrientation(GLfloat yaw, GLfloat pitch)
{
	this->yaw = yaw;
	this->pitch = pitch;

	update();
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::update() {
	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {

}