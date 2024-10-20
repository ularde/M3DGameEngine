#include "pch.h"
#include "Atmosphere.h"
#include "Scene.h"
#include "RenderAtomspherePipeline.h"
#include "EntityComponent.h"
#include "Camera.h"
#include "TimeOfDay.h"

class MCameraComponent;

#define PI 3.141592
#define iSteps 16
#define jSteps 8

MAtmosphere::MAtmosphere(MScene* parent, const std::string& name_) {
	mParent = parent;
	gPlatform = mParent->GetPlatform();
	mName = name_;
}

MAtmosphere::~MAtmosphere() {

}

void MAtmosphere::SetTimeOfDayObject(MTimeOfDay* tod) {
	mTimeOfDay = tod;
	UpdateParameters();
}

void MAtmosphere::InitializeLuaInstance()
{
}

void MAtmosphere::Update(double dt) {

}

void MAtmosphere::UpdateParameters() {
	gPlatform->gAtomspherePipeline->SelectShader("atmosphere");
	gPlatform->gAtomspherePipeline->GetCurrentAtomsphereShader()->UniformVec3("sunDir", mTimeOfDay->GetSunDirection());
	gPlatform->gAtomspherePipeline->GetCurrentAtomsphereShader()->UniformFloat("g", mTimeOfDay->mAtmosphereParameters.g);
	gPlatform->gAtomspherePipeline->GetCurrentAtomsphereShader()->UniformFloat("iSun", mTimeOfDay->mAtmosphereParameters.iSun);
	gPlatform->gAtomspherePipeline->GetCurrentAtomsphereShader()->UniformFloat("kMie", mTimeOfDay->mAtmosphereParameters.kMie);
	gPlatform->gAtomspherePipeline->GetCurrentAtomsphereShader()->UniformVec3("kRlh", mTimeOfDay->mAtmosphereParameters.kRlh);
	gPlatform->gAtomspherePipeline->GetCurrentAtomsphereShader()->UniformFloat("rAtmos", mTimeOfDay->mAtmosphereParameters.rAtmos);
	gPlatform->gAtomspherePipeline->GetCurrentAtomsphereShader()->UniformFloat("rPlanet", mTimeOfDay->mAtmosphereParameters.rPlanet);
	gPlatform->gAtomspherePipeline->GetCurrentAtomsphereShader()->UniformFloat("shMie", mTimeOfDay->mAtmosphereParameters.shMie);
	gPlatform->gAtomspherePipeline->GetCurrentAtomsphereShader()->UniformFloat("shRlh", mTimeOfDay->mAtmosphereParameters.shRlh);
}

void MAtmosphere::RenderSphere() {
	const unsigned int X_SEGMENTS = 8;
	const unsigned int Y_SEGMENTS = 8;

	static bool initFlag = false;
	static GLuint sVAO, sVBO, sEBO;
	static unsigned int indexCount;

	if (!initFlag) {
		glGenVertexArrays(1, &sVAO);
		glGenBuffers(1, &sVBO);
		glGenBuffers(1, &sEBO);
		std::vector<glm::vec3> positions;
		std::vector<unsigned int> indices;

		for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				positions.push_back(glm::vec3(xPos, yPos, zPos));
			}

			bool oddRow = false;
			for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
				if (!oddRow) {
					for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
						indices.push_back(y * (X_SEGMENTS + 1) + x);
						indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					}
				}
				else {
					for (int x = X_SEGMENTS; x >= 0; --x) {
						indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
						indices.push_back(y * (X_SEGMENTS + 1) + x);
					}
				}
				oddRow = !oddRow;
			}
		}
		indexCount = static_cast<unsigned int>(indices.size());

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); ++i) {
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
		}
		glBindVertexArray(sVAO);
		glBindBuffer(GL_ARRAY_BUFFER, sVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		initFlag = true;
	}
	glDisable(GL_CULL_FACE);
	glBindVertexArray(sVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
	glEnable(GL_CULL_FACE);
}
