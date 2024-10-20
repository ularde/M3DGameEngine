#version 430 core
in vec2 TexCoords;
out vec4 fragColor;

uniform sampler2D glyph;
uniform vec3 color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(glyph, TexCoords).r);
    fragColor = vec4(color, 1.0) * sampled;
}