#include "EntityAbstract.h"


EntityAbstract::EntityAbstract(glm::vec3 pos, glm::vec3 rot, GLfloat scale) {
	this->posX = pos.x;
	this->posY = pos.y;
	this->posZ = pos.z;
	this->rotX = rot.x;
	this->rotY = rot.y;
	this->rotZ = rot.z;
	this->scale = scale;
}

EntityAbstract::EntityAbstract(GLfloat posX,GLfloat posY,GLfloat posZ,GLfloat scale) {
	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;
	this->rotX = 0.0f;
	this->rotY = 0.0f;
	this->rotZ = 0.0f;
	this->scale = scale;
}

glm::mat4 EntityAbstract::getTransformationMatrix() {
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, glm::vec3(this->posX, this->posY, this->posZ));
	matrix = glm::rotate(matrix, glm::radians(this->rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(this->rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(this->rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::scale(matrix, glm::vec3(this->scale, this->scale, this->scale));
	return matrix;
}
