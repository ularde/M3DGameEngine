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

class MNonPythonBoundObject;

struct MMeshVertex {
    float position[3];
    float normal[3];
    float texCoord[2];
    float tangent[3];
    float bitangent[3];
    int m_BoneIDs[M3D_MAX_BONE_COUNT];
    float m_Weights[M3D_MAX_BONE_COUNT];
};

struct MMeshTexture {
    unsigned int texID;
    std::string type;
    std::string path;
};

class MMesh: public MNonPythonBoundObject {
public:
    MMesh(MMeshVertex* vert_data, unsigned int vert_count, unsigned int* indices_data, unsigned int indices_count, MMeshTexture* tex_data, unsigned int tex_count);
    ~MMesh();
    void Render();
private:
    MMeshVertex* verticesPtr = NULL;
    unsigned int* indicesPtr = NULL;
    MMeshTexture* texturesPtr = NULL;
    unsigned int verticesCount, indicesCount, texturesCount;
protected:
};