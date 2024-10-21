#include "pch.h"
#include "TriangleMesh.h"
#include "RigidStatic.h"
#include "Model.h"
#include "Material.h"
#include "RenderDeferredPipeline.h"
#include "RenderForwardSubPipeline.h"

class MModel;
class MRigidStatic;

MTriangleMesh::MTriangleMesh(MBasicPlatform* platform, std::vector<MMeshVertex> vertices,
	std::vector<unsigned int> indices, unsigned int materialIndex,
	int surfaceTypeID) {
	gPlatform = platform;
	mVertices = vertices;
	mIndices = indices;
	mMaterialIndex = materialIndex;
	mSurfaceTypeID = surfaceTypeID;
	//set up mesh in OpenGL
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(MMeshVertex), &mVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)offsetof(MMeshVertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)offsetof(MMeshVertex, texCoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)offsetof(MMeshVertex, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)offsetof(MMeshVertex, bitangent));

	glBindVertexArray(0);
	//set up mesh in PhysX
	if (mEnablePhysics) {
		CookTriangleMesh();
	}
}

MTriangleMesh::MTriangleMesh(MBasicPlatform* platform, std::vector<MMeshVertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex, int surfaceTypeID, bool enablePhysics) {
	gPlatform = platform;
	mVertices = vertices;
	mIndices = indices;
	mMaterialIndex = materialIndex;
	mSurfaceTypeID = surfaceTypeID;
	//set up mesh in OpenGL
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(MMeshVertex), &mVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)offsetof(MMeshVertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)offsetof(MMeshVertex, texCoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)offsetof(MMeshVertex, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MMeshVertex), (void*)offsetof(MMeshVertex, bitangent));

	glBindVertexArray(0);
	//set up mesh in PhysX
	if (mEnablePhysics) {
		CookTriangleMesh();
	}
}

MTriangleMesh::~MTriangleMesh()
{
}


void MTriangleMesh::Render() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MTriangleMesh::RenderForDepthMapping() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

std::vector<glm::vec3> MTriangleMesh::GetVertexPositions() {
	std::vector<glm::vec3> vps;
	for (unsigned int i = 0; i < mVertices.size(); i++) {
		vps.push_back(mVertices[i].position);
	}
	return vps;
}

void MTriangleMesh::CookTriangleMesh() {
	pMeshPoints = (physx::PxVec3*)malloc(mVertices.size() * sizeof(physx::PxVec3));
	pMeshIndices32 = (physx::PxU32*)malloc(mIndices.size() * sizeof(physx::PxU32));

	for (unsigned int i = 0; i < mVertices.size(); i++) {
		pMeshPoints[i].x = mVertices[i].position.x;
		pMeshPoints[i].y = mVertices[i].position.y;
		pMeshPoints[i].z = mVertices[i].position.z;
	}

	for (unsigned int i = 0; i < mIndices.size(); i++) {
		pMeshIndices32[i] = (physx::PxU32)mIndices[i];
	}

	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = mVertices.size();
	meshDesc.points.stride = sizeof(physx::PxVec3);
	meshDesc.points.data = pMeshPoints;

	meshDesc.triangles.count = mIndices.size() / 3;
	meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
	meshDesc.triangles.data = pMeshIndices32;

#ifdef _DEBUG
	bool res = gPlatform->pCooking->validateTriangleMesh(meshDesc);
	PX_ASSERT(res);
#endif

	if (pTriangleMesh = gPlatform->pCooking->createTriangleMesh(meshDesc,
		gPlatform->pPhysics->getPhysicsInsertionCallback())) {
		mTriangleMeshCooked = true;
	}
}