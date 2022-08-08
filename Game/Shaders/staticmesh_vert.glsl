#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out gl_PerVertex {
	vec4 gl_Position;
};
out vec2 texCoord__;
out vec3 normal__;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main() {
	gl_Position = projectionMat * viewMat * modelMat * vec4(pos, 1.0f);
	texCoord__ = vec2(texCoord.x, texCoord.y);
}