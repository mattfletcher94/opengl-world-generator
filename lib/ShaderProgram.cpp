#include "ShaderProgram.h"
#include <iostream>
#include <fstream>

ShaderProgram::ShaderProgram(std::string vertexShaderPath,std::string fragmentShaderPath) {
	std::string vertexShaderContents = this->getFile(vertexShaderPath).str();
    std::string fragmentShaderContents = this->getFile(fragmentShaderPath).str();
	this->programId = glCreateProgram();
    this->vertexShaderId = this->createShader(vertexShaderContents,GL_VERTEX_SHADER);
    this->fragmentShaderId = this->createShader(fragmentShaderContents,GL_FRAGMENT_SHADER);
	glAttachShader(this->programId, this->vertexShaderId);
    glAttachShader(this->programId, this->fragmentShaderId);
    glLinkProgram(this->programId);
}

std::stringstream ShaderProgram::getFile(std::string filePath) {
    std::stringstream result;
    std::ifstream stream(filePath);
    std::string line;
    while(getline(stream,line))
        result << line << "\n";
    return result;
}

GLuint ShaderProgram::createShader(std::string shaderFile,GLenum shaderType) {
    GLuint id = glCreateShader(shaderType);
    const char* shader_c_string = shaderFile.c_str();
    glShaderSource(id,1,&shader_c_string,nullptr);
    glCompileShader(id);
    GLint result;
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if(result == GL_FALSE){
        GLint length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        char *message = (char*)alloca(sizeof(char)*length);
        glGetShaderInfoLog(id,length,&length,message);
        std::cout << "Failed to compile "<<(shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader"<<std::endl;
        std::cout << message << std::endl;
    }
    return id;
}

void ShaderProgram::start() {
	glUseProgram(this->programId);
}

void ShaderProgram::stop() {
	glUseProgram(0);
}

void ShaderProgram::registerUniform(std::string name) {
	this->start();
	this->uniforms.insert(std::pair<std::string, GLuint>(name, glGetUniformLocation(this->programId, name.c_str())));
	this->stop();
}

void ShaderProgram::setUniform1f(std::string name,float value) {
	GLuint uniform = glGetUniformLocation(this->programId,name.c_str());
    glUniform1f(uniform, value); 
}

void ShaderProgram::setUniform3f(std::string name,float value1,float value2,float value3) {
	GLuint uniform = glGetUniformLocation(this->programId,name.c_str());
    glUniform3f(uniform, value1, value2, value3); 
}


void ShaderProgram::setUniformInt(std::string name,int value) {
	GLuint uniform = glGetUniformLocation(this->programId,name.c_str());
    glUniform1i(uniform, value); 
}

void ShaderProgram::setMatrix4(std::string name,glm::mat4 matrix) {
	GLuint uniform = glGetUniformLocation(this->programId,name.c_str());
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::dispose() {
	this->stop();
	glDeleteShader(this->vertexShaderId);
	glDeleteShader(this->fragmentShaderId);
	glDeleteProgram(this->programId);
}
