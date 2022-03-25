#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <glm/glm.hpp> 
#include "glm/ext/vector_float3.hpp"
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <GL\glew.h>
#include "EntityAbstract.h"

class VAO {

	public:
		GLuint indicesLength = 0;
		GLuint vaoId = 0;
		GLuint verticesVBO = 0;
		GLuint textureCoordsVBO = 0;
		GLuint normalsVBO = 0;
		GLuint indicesVBO = 0;
		GLuint texture0 = -1;
		GLuint texture1 = -1;
		GLuint texture2 = -1;
		GLuint texture3 = -1;
		GLuint texture4 = -1;
		std::vector<EntityAbstract*> entities;
		VAO();
		VAO(std::string objFile);
		VAO(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<int> indices, std::vector<float> normals);
		void addEntity(EntityAbstract* entity);
		void setTexture0(GLuint id);
		void setTexture1(GLuint id);
		void setTexture2(GLuint id);
		void setTexture3(GLuint id);
		void setTexture4(GLuint id);
		void bind();
		void unbind();
		void dispose();
	private:
		void loadObjFile(std::string objFile);
		std::vector<std::string> splitString(std::string string, char delimiter);
};

