#include "EntityTerrain.h"


void EntityTerrain::render(int indicesSize,ShaderProgram shaderProgram) {
	this->shineDamper = 1000.0f;
	this->reflectivity = 0.01f;
	shaderProgram.start();
	shaderProgram.setMatrix4("model", this->getTransformationMatrix());
	//shaderProgram.setUniform1f("uniformShineDamper", this->shineDamper);
	//shaderProgram.setUniform1f("uniformReflectivity", this->reflectivity);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	shaderProgram.stop();
}

