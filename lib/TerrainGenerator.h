#pragma once
#include <string>
#include "VAO.h"
class TerrainGenerator
{
public:
	std::string heightMapPath;
	int terrainSize;
	int terrainMaxHeight;
	int maxPixelColor = 256 * 256 * 256;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textureCoords;
	std::vector<int> indices;
	TerrainGenerator(std::string heightMapPath, int terrainSize, int terrainMaxHeight);
	VAO generateVAO();
private:
	float calculatePixelHeight(unsigned char* imageData, int imageWidth, int imageHeight, int x, int y);
	glm::vec3 calculateNormal(unsigned char* imageData, int imageWidth, int imageHeight, int x, int y);
};

