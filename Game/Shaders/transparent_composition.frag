#version 430 core

layout (location = 0) out vec4 fragCoord;

in vec2 vTexCoords;

uniform sampler2D accumChannel;
uniform sampler2D revealChannel;

const float EP = 1e-5;

bool IsNearlyEqual(float a, float b) { return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EP; }

float MaxInVec3(vec3 v) { return max(max(v.x, v.y), v.z); }

void main()
{
    ivec2 coords = ivec2(gl_FragCoord.xy);
    float revealage = texelFetch(revealChannel, coords, 0).r;
    if (IsNearlyEqual(revealage, 1.0)) { discard; }
    vec4 accum = texelFetch(accumChannel, coords, 0);
    if (isinf(MaxInVec3(abs(accum.rgb)))) { accum.rgb = vec3(accum.a); }
    vec3 averageColor = accum.rgb / max(accum.a, EP);
    fragCoord = vec4(averageColor, 1.0 - revealage);
}