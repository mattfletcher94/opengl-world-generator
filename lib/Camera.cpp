#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>


Camera::Camera(float windowWidth, float windowHeight) {
	this->posX = 0;
	this->posY = 2.0f;
	this->posZ = 10.0f;
	this->rotX = 0.0f;
	this->rotY = 0.0f;
	this->rotZ = 0.0f;
	this->width = windowWidth;
	this->height = windowHeight;
    this->projectionMatrix = glm::perspective(this->FOV, this->width / this->height, this->NEAR_PLANE, this->FAR_PLANE);
}

glm::mat4 Camera::getViewMatrix() {



	if(this->rotX > 70.0f) {
		this->rotX = 70.0f;
	} else if(this->rotX < -5.0f) {
		this->rotX = -5.0f;
	}
	if(this->posY < 10.0f) {
		this->posY = 10.0f;
	}

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::rotate(viewMatrix, glm::radians(this->rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, glm::radians(this->rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, glm::radians(this->rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 negativeCameraPosition = glm::vec3(-this->posX, -this->posY, -this->posZ);
	viewMatrix = glm::translate(viewMatrix, negativeCameraPosition);
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() {
	this->projectionMatrix = glm::perspective(this->FOV, this->width / this->height, this->NEAR_PLANE, this->FAR_PLANE);
	return this->projectionMatrix;
}