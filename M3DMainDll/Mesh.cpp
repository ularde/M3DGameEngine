#include "pch.h"
#include "Mesh.h"

MMesh::MMesh(MBasicPlatform* platform_, std::vector<MMeshVertex> vertices_,
	std::vector<unsigned int> indices_, unsigned int material_index,
	int surface_type_ID) {
	gPlatform = platform_;
	vertices = vertices_;
	indices = indices_;
	mMaterialIndex = material_index;
	mSurfaceTypeID = surface_type_ID;
	//set up mesh in OpenGL
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MMeshVertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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

MMesh::~MMesh()
{
}

void MMesh::Render() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MMesh::RenderForDepthMapping() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

std::vector<glm::vec3> MMesh::GetVertexPositions() {
	std::vector<glm::vec3> vps;
	for (unsigned int i = 0; i < vertices.size(); i++) {
		vps.push_back(vertices[i].position);
	}
	return vps;
}

void MMesh::CookTriangleMesh() {
	pMeshPoints = (physx::PxVec3*)malloc(vertices.size() * sizeof(physx::PxVec3));
	pMeshIndices32 = (physx::PxU32*)malloc(indices.size() * sizeof(physx::PxU32));

	for (unsigned int i = 0; i < vertices.size(); i++) {
		pMeshPoints[i].x = vertices[i].position.x;
		pMeshPoints[i].y = vertices[i].position.y;
		pMeshPoints[i].z = vertices[i].position.z;
	}

	for (unsigned int i = 0; i < indices.size(); i++) {
		pMeshIndices32[i] = (physx::PxU32)indices[i];
	}

	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = vertices.size();
	meshDesc.points.stride = sizeof(physx::PxVec3);
	meshDesc.points.data = pMeshPoints;

	meshDesc.triangles.count = indices.size() / 3;
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