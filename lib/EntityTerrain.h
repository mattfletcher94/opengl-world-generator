#pragma once

#include "EntityAbstract.h"

class EntityTerrain : public EntityAbstract {
	public:
		EntityTerrain(glm::vec3 pos, glm::vec3 rot, GLfloat scale): EntityAbstract(pos,rot,scale) {}
		EntityTerrain(GLfloat posX,GLfloat posY,GLfloat posZ,GLfloat scale): EntityAbstract(posX,posY,posZ,scale) {}
		void render(int indicesSize,ShaderProgram shaderProgram) override;
};

