#pragma once
#include <glm/glm.hpp> 
#include "glm/ext/vector_float3.hpp"
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <GL\glew.h>

class Camera
{
private: 
	const float FOV = 70;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;
	glm::mat4 projectionMatrix;
public:
	GLfloat posX;
	GLfloat posY;
	GLfloat posZ;
	GLfloat rotX;
	GLfloat rotY;
	GLfloat rotZ;
	float width = 0.0f;
	float height = 0.0f;
	Camera(float windowWidth, float windowHeight);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
};

