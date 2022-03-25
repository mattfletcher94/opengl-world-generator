#pragma once

#include "EntityAbstract.h"

class EntityLightSource : public EntityAbstract {
	public:
		EntityLightSource(glm::vec3 pos, glm::vec3 rot, GLfloat scale): EntityAbstract(pos,rot,scale) {}
		EntityLightSource(GLfloat posX,GLfloat posY,GLfloat posZ,GLfloat scale): EntityAbstract(posX,posY,posZ,scale) {}
		void render(int indicesSize,ShaderProgram shaderProgram) override;
};

