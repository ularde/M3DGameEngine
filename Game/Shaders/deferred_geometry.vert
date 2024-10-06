#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 vPosition;
out mat3 vTBN;
out vec2 vTexCoords;
out vec4 vShadowCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 B = normalize(normalMatrix * aBitangent);
	vec3 N = normalize(normalMatrix * aNormal);
	vTBN = mat3(T, B, N);
    vec3 fragPos = vec3(view * model * vec4(aPosition, 1.0));
	vShadowCoord = lightSpaceMatrix * model * vec4(aPosition, 1.0);
    vPosition = fragPos;
    vTexCoords = aTexCoords;
    gl_Position = projection * vec4(fragPos, 1.0);
}