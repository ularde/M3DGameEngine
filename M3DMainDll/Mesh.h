#pragma once
#include "AssetManager.h"
#include "Platforms.h"
#include <vector>
#include <string>

#include "glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Class.h"
#include "Global.h"
#include "Texture.h"
#include "GeometricShape.h"

class MUnscriptableObject;
class MBasicPlatform;
class MTexture;
class MRigidStatic;
class MGeometricShape;

struct MMeshVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    //int m_BoneIDs[M3D_MAX_BONE_COUNT];
    //float m_Weights[M3D_MAX_BONE_COUNT];
};

class MTriangleMesh: public MGeometricShape {
public:
    MTriangleMesh(MBasicPlatform* platform, std::vector<MMeshVertex> vertices,
        std::vector<unsigned int> indices, unsigned int materialIndex,
        int surfaceTypeID);
    MTriangleMesh(MBasicPlatform* platform, std::vector<MMeshVertex> vertices,
        std::vector<unsigned int> indices, unsigned int materialIndex,
        int surfaceTypeID, bool enablePhysics);
    ~MTriangleMesh();
    void Render();
    void RenderForDepthMapping();
    bool GetPhysicsStatue() { return this->mTriangleMeshCooked; }
    unsigned int GetMaterialIndex() { return mMaterialIndex; }
    std::vector<glm::vec3> GetVertexPositions();
    physx::PxTriangleMesh* GetTriangleMesh() { return this->pTriangleMesh; }
private:
    std::vector<MMeshVertex> mVertices; 
    std::vector<unsigned int> mIndices; 
    GLuint VAO, VBO, EBO;
    unsigned int mMaterialIndex = 0;
    int mSurfaceTypeID = 0;
    bool mEnablePhysics = true,
        mTriangleMeshCooked = false;

    physx::PxVec3* pMeshPoints = NULL;
    physx::PxU32* pMeshIndices32 = NULL;
    physx::PxTriangleMesh* pTriangleMesh = NULL;

    void CookTriangleMesh();
};