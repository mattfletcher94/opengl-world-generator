#pragma once
#include <GL\glew.h>
class FBO
{
	public:
		GLuint fboId = 0;
		GLuint texture0Id = 0;
		GLuint texture1Id = 0;
		GLuint rboId = 0;
		GLuint quadVaoId = 0;
		GLuint quadVboId = 0;
		GLfloat width = 0;
		GLfloat height = 0;
		FBO(float width, float height);
		void generateQuad();
		void bind();
		void unbind();
		void render();
		void dispose();
};

