#version 430 core

layout (location = 0) in vec3 aPosition;

out vec3 vPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 m_Position = projection * view * model * vec4(aPosition, 1.0);
    gl_Position = m_Position.xyzw;
    vPosition = aPosition.xyz;
}