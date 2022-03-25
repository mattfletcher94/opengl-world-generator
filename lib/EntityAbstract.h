#pragma once
#include <GL\glew.h>
#include "ShaderProgram.h"
#include <iostream>
class EntityAbstract {
	
	public: 
		EntityAbstract(glm::vec3 pos, glm::vec3 rot, GLfloat scale);
		EntityAbstract(GLfloat posX,GLfloat posY,GLfloat posZ,GLfloat scale);
		GLfloat posX = 0.0f;
		GLfloat posY = 0.0f;
		GLfloat posZ = 0.0f;
		GLfloat rotX = 0.0f;
		GLfloat rotY = 0.0f;
		GLfloat rotZ = 0.0f;
		GLfloat scale = 1.0f;
		GLfloat reflectivity = 0.2f;
		GLfloat shineDamper = 1000.0f;
		virtual void render(int indicesSize,ShaderProgram shaderProgram) { std::cout << "Base render called\n"; };
		virtual ~EntityAbstract() {};
		glm::mat4 getTransformationMatrix();
};

