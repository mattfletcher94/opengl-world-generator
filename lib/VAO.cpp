#include "VAO.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "../Obj_Loader.h"

VAO::VAO() {

}

VAO::VAO(std::string objFile) {
	glGenVertexArrays(1, &this->vaoId);
	glGenBuffers(1, &this->verticesVBO);
    glGenBuffers(1, &this->textureCoordsVBO);
    glGenBuffers(1, &this->normalsVBO);
    glGenBuffers(1, &this->indicesVBO);
	this->loadObjFile(objFile);
}

VAO::VAO(std::vector<float> vertices,std::vector<float> textureCoords,std::vector<int> indices,std::vector<float> normals) {
	
	glGenVertexArrays(1, &this->vaoId);
	glGenBuffers(1, &this->verticesVBO);
    glGenBuffers(1, &this->textureCoordsVBO);
    glGenBuffers(1, &this->normalsVBO);
    glGenBuffers(1, &this->indicesVBO);

	// Set the vertices VBO
	glBindVertexArray(this->vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Set the texture coords VBO
	glBindVertexArray(this->vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureCoords.size(), textureCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Set the indices VBO
	this->indicesLength = (int)indices.size();
	glBindVertexArray(this->vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	// Set the normals VBO
	glBindVertexArray(this->vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void VAO::addEntity(EntityAbstract* entity) {
	this->entities.push_back(entity);
}

void VAO::setTexture0(GLuint id)
{
	this->texture0 = id;
}

void VAO::setTexture1(GLuint id)
{
	this->texture1 = id;
}

void VAO::setTexture2(GLuint id)
{
	this->texture2 = id;
}

void VAO::setTexture3(GLuint id)
{
	this->texture3 = id;
}

void VAO::setTexture4(GLuint id)
{
	this->texture4 = id;
}

void VAO::bind() {
	glBindVertexArray(this->vaoId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
	if (this->texture0 > 0.0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->texture0);
	} else {

	}
	if (this->texture1 > 0.0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->texture1);
	}
	if (this->texture2 > 0.0) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->texture2);
	}
	if (this->texture3 > 0.0) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, this->texture3);
	}
	if (this->texture4 > 0.0) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, this->texture4);
	}
}

void VAO::unbind() {
	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void VAO::dispose() {
	glDeleteVertexArrays(1, &this->vaoId);
    glDeleteBuffers(1, &this->verticesVBO);
    glDeleteBuffers(1, &this->textureCoordsVBO);
	glDeleteBuffers(1, &this->normalsVBO);
    glDeleteBuffers(1, &this->indicesVBO);
	for (EntityAbstract* entity : this->entities) {
		delete entity;
	}
	this->entities.clear();
}

void VAO::loadObjFile(std::string objFile) {

	// Init loader
	objl::Loader Loader;

	// Load .obj File
	bool loadout = Loader.LoadFile(objFile);

	// Check to see if it loaded

	// If so continue
	if(loadout) {

		// Get first mesh
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		
		// Create arrays
		std::vector<float> verticesArray;
		std::vector<float> normalsArray;
		std::vector<float> texturesArray;
		std::vector<unsigned int> indicesArray = curMesh.Indices;

		// Go through each vertex and print its number,
		//  position, normal, and texture coordinate
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			verticesArray.push_back(curMesh.Vertices[j].Position.X);
			verticesArray.push_back(curMesh.Vertices[j].Position.Y);
			verticesArray.push_back(curMesh.Vertices[j].Position.Z);

			normalsArray.push_back(curMesh.Vertices[j].Normal.X);
			normalsArray.push_back(curMesh.Vertices[j].Normal.Y);
			normalsArray.push_back(curMesh.Vertices[j].Normal.Z);

			texturesArray.push_back(curMesh.Vertices[j].TextureCoordinate.X);
			texturesArray.push_back(curMesh.Vertices[j].TextureCoordinate.Y);

		}


		// Set the vertices VBO
		glBindVertexArray(this->vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesArray.size(), verticesArray.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		// Set the texture coords VBO
		glBindVertexArray(this->vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texturesArray.size(), texturesArray.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		// Set the indices VBO
		this->indicesLength = (int)indicesArray.size();
		glBindVertexArray(this->vaoId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(int) * indicesArray.size(), indicesArray.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);

		// Set the normals VBO
		glBindVertexArray(this->vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalsArray.size(), normalsArray.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);

	} else {
		std::cout << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
	}

}

std::vector<std::string> VAO::splitString(std::string string,char delimiter) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = string.find(delimiter, start)) != std::string::npos) {
		tokens.push_back(string.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(string.substr(start));
	return tokens;
}
