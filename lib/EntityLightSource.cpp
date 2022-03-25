#include "EntityLightSource.h"

void EntityLightSource::render(int indicesSize,ShaderProgram shaderProgram) {
	shaderProgram.start();
	shaderProgram.setMatrix4("model", this->getTransformationMatrix());
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	shaderProgram.stop();
}