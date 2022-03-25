#include "TerrainGenerator.h"
#include "../stb_image.h"

TerrainGenerator::TerrainGenerator(std::string heightMapPath, int terrainSize, int terrainMaxHeight)
{
	this->heightMapPath = heightMapPath;
	this->terrainSize = terrainSize;
	this->terrainMaxHeight = terrainMaxHeight;
}

VAO TerrainGenerator::generateVAO()
{

	// Load the height map
	int imageWidth, imageHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(this->heightMapPath.c_str(), &imageWidth, &imageHeight, &nrChannels, STBI_rgb_alpha);
	if (!imageData) {
		std::cout << "Failed to load heightmap:" << std::endl;
		std::cout << this->heightMapPath << std::endl;
		exit(0);
	}

	// Vertex count will be image height
	int vertexCount = imageHeight;

	// Array size
	int arraySize = vertexCount * vertexCount;

	// Setup arrays
	this->vertices.resize(arraySize * 3.0);
	this->normals.resize(arraySize * 3.0);
	this->textureCoords.resize(arraySize * 2.0);
	this->indices.resize(6.0 * (vertexCount - 1.0) * (vertexCount - 1.0));

	// Generate vertices, normals and texture coords
	int vertexPointer = 0;
	for (int i = 0; i < vertexCount; i++) {
		for (int j = 0; j < vertexCount; j++) {
			glm::vec3 normal = this->calculateNormal(imageData, imageWidth, imageHeight, i, j);
			float height = this->calculatePixelHeight(imageData, imageWidth, imageHeight, i, j);
			this->vertices[vertexPointer * 3.0] = (float)j / ((float)vertexCount - 1) * this->terrainSize;
			this->vertices[vertexPointer * 3.0 + 1.0] = height;
			this->vertices[vertexPointer * 3.0 + 2.0] = (float)i / ((float)vertexCount - 1) * this->terrainSize;
			this->normals[vertexPointer * 3.0] = normal.x;
			this->normals[vertexPointer * 3.0 + 1.0] = normal.y;
			this->normals[vertexPointer * 3.0 + 2.0] = normal.z;
			this->textureCoords[vertexPointer * 2.0] = (float)j / ((float)vertexCount - 1);
			this->textureCoords[vertexPointer * 2.0 + 1.0] = (float)i / ((float)vertexCount - 1);
			vertexPointer++;
		}
	}

	// Generate indices
	int pointer = 0;
	for (int gz = 0; gz < vertexCount - 1; gz++) {
		for (int gx = 0; gx < vertexCount - 1; gx++) {
			int topLeft = (gz * vertexCount) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * vertexCount) + gx;
			int bottomRight = bottomLeft + 1;
			this->indices[pointer++] = topLeft;
			this->indices[pointer++] = bottomLeft;
			this->indices[pointer++] = topRight;
			this->indices[pointer++] = topRight;
			this->indices[pointer++] = bottomLeft;
			this->indices[pointer++] = bottomRight;
		}
	}

	// Remove image from memory
	stbi_image_free(imageData);

	// Return a VAO
	return VAO(this->vertices, this->textureCoords, this->indices, this->normals);
}


float TerrainGenerator::calculatePixelHeight(unsigned char* imageData, int imageWidth, int imageHeight, int x, int y) {
	if (x < 0.0 || x >= imageHeight || y < 0.0 || y >= imageHeight) {
		return 0.0f;
	}
	unsigned char* pixelOffset = imageData + (y + imageHeight * x) * STBI_rgb_alpha;
	float height = (float)(pixelOffset[0] * pixelOffset[1] * pixelOffset[2]);
	height = height / this->maxPixelColor;
	height = height * this->terrainMaxHeight;
	return height;
}

glm::vec3 TerrainGenerator::calculateNormal(unsigned char* imageData, int imageWidth, int imageHeight, int x, int y) {
	float left = this->calculatePixelHeight(imageData, imageWidth, imageHeight, x - 1, y);
	float right = this->calculatePixelHeight(imageData, imageWidth, imageHeight, x + 1, y);
	float down = this->calculatePixelHeight(imageData, imageWidth, imageHeight, x, y - 1);
	float up = this->calculatePixelHeight(imageData, imageWidth, imageHeight, x, y + 1);
	glm::vec3 normal = glm::vec3(left - right, 2.0f, down - up);
	normal = glm::normalize(normal);
	return normal;
}