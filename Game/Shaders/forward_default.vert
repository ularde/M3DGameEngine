#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 vWorldPos;
out mat3 vTBN;
out vec2 vTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec3 fragPos = vec3(model * vec4(aPosition, 1.0));
    vTBN = mat3(normalize(aTangent),
                normalize(aBitangent),
                normalize(aNormal));
    vWorldPos = fragPos;
    vTexCoords = aTexCoords;
    vec4 m_Position = projection * view * vec4(fragPos, 1.0);
    gl_Position = m_Position.xyww;
}