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

class MUnscriptableObject;
class MBasicPlatform;
class MTexture;

struct MMeshVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    //int m_BoneIDs[M3D_MAX_BONE_COUNT];
    //float m_Weights[M3D_MAX_BONE_COUNT];
};

class MMesh: public MUnscriptableObject {
public:
    MMesh(MBasicPlatform* platform_, std::vector<MMeshVertex> vertices_,
        std::vector<unsigned int> indices_, unsigned int material_index,
        int surface_type_ID);
    ~MMesh();
    void Render();
    void RenderForDepthMapping();
    bool GetPhysicsStatue() { return this->mTriangleMeshCooked; }
    unsigned int GetMaterialIndex() { return mMaterialIndex; }
    std::vector<glm::vec3> GetVertexPositions();
    physx::PxTriangleMesh* GetTriangleMesh() { return this->pTriangleMesh; }
private:
    MBasicPlatform* platform = NULL;
    std::vector<MMeshVertex> vertices; 
    std::vector<unsigned int> indices; 
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