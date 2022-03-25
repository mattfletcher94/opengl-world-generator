#include <string>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp> 
#include "glm/ext/vector_float3.hpp"
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <map>

#ifndef ShaderProgram_H
#define ShaderProgram_H

class ShaderProgram {
	private:
		std::map<std::string, GLuint> uniforms;
		std::stringstream getFile(std::string filePath);
		GLuint createShader(std::string shaderFile, GLenum shaderType);
	public:
		GLuint programId;
		GLuint vertexShaderId;
		GLuint fragmentShaderId;
		ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);
		void start();
		void stop();
		void registerUniform(std::string name);
		void setUniform1f(std::string name, float value);
		void setUniform3f(std::string name, float value1, float value2, float value3);
		void setUniformInt(std::string name,int value);
		void setMatrix4(std::string name,glm::mat4 matrix);
		void dispose();

};

#endif;
