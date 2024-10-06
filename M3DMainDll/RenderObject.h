#pragma once
#include "Class.h"
#include <vector>
#include <glm/glm.hpp>

class MRenderObject :public MUnscriptableObject {
public:
	glm::vec3 mPosition;
	void UpdateModelMatrix(const glm::mat4& model) { mModelMatrix = model; }
	virtual void Render() = 0;
protected:
	glm::mat4 mModelMatrix = glm::mat4(1.0f);
};