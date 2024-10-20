#version 430 core

layout (location = 0) out vec3 Position;
layout (location = 1) out vec4 Normal;
layout (location = 2) out vec4 Albedo;
layout (location = 3) out vec4 MRA;

in vec3 vPosition;
in mat3 vTBN;
in vec2 vTexCoords;
in vec4 vShadowCoord;

uniform sampler2D texture_albedo;
uniform sampler2D texture_normal;
uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;
uniform sampler2D texture_AO;
uniform sampler2DShadow depth_texture;

uniform vec3 albedo_color;
uniform float metallic_color;
uniform float roughness_color;
uniform float ao_color;
uniform vec3 lightDir;
//uniform vec3 camPos;

uniform bool use_default_norm;
vec3 default_normal = vec3(0.5, 0.5, 1.0);

float random(vec3 seed, int i) {
    vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main()
{
    vec3 tangentNormal = vec3(0.0);
    if (use_default_norm)
    {
        tangentNormal = default_normal * 2.0 - 1.0;
    }
    else
    {
        tangentNormal = texture(texture_normal, vTexCoords).xyz * 2.0 - 1.0;
    }
    Position = vPosition;
    Normal.xyz = normalize(vTBN * tangentNormal);

    const float far = 1000.0;
    const float near = 0.3;
    float d = gl_FragCoord.z * 2.0 - 1.0;
    //d = (2.0 * near) / (far + near - d * (far - near));
    Normal.a = d;

    Albedo = vec4(texture(texture_albedo, vTexCoords).rgb + albedo_color, 0.0);
    MRA.r = texture(texture_metallic, vTexCoords).r + metallic_color;
    MRA.g = texture(texture_roughness, vTexCoords).r + roughness_color;
    MRA.b = texture(texture_AO, vTexCoords).r + ao_color;
    //MRA.a = 1.0;
    vec4 projCoords = vShadowCoord * 0.5 + 0.5;
    //vec3 V = normalize(camPos - WorldPos);
    vec3 L = normalize(lightDir);
    //vec3 H = normalize(V + L);
    //float cosTheta = max(dot(H, V), 0.0);
    float Theta = abs(radians(90.0) - acos(dot(L, vec3(0.0, 1.0, 0.0))));
    //float bias = 0.0005;
    //if (Theta < radians(2.0)) {
    //    bias = 0.0046;
    //}
    //else if (Theta < radians(5.7)) {
    //    bias = 0.0017;
    //}
    float blurSize = 1.0 / 400.0;
    /*
    MRA.a = 0.0;
    MRA.a += textureProj(depth_texture, projCoords - 0.3 * blurSize) * 0.0215;
    MRA.a += textureProj(depth_texture, projCoords - 0.2 * blurSize) * 0.136;
    MRA.a += textureProj(depth_texture, projCoords - 0.1 * blurSize) * 0.341;
    MRA.a += textureProj(depth_texture, projCoords) * 0.009;
    MRA.a += textureProj(depth_texture, projCoords + 0.1 * blurSize) * 0.341;
    MRA.a += textureProj(depth_texture, projCoords + 0.2 * blurSize) * 0.136;
    MRA.a += textureProj(depth_texture, projCoords + 0.3 * blurSize) * 0.0215;
    */
    MRA.a = 1.0;
    vec2 poissonDisk[16] = vec2[]( 
        vec2( -0.94201624, -0.39906216 ), 
        vec2( 0.94558609, -0.76890725 ), 
        vec2( -0.094184101, -0.92938870 ), 
        vec2( 0.34495938, 0.29387760 ), 
        vec2( -0.91588581, 0.45771432 ), 
        vec2( -0.81544232, -0.87912464 ), 
        vec2( -0.38277543, 0.27676845 ), 
        vec2( 0.97484398, 0.75648379 ), 
        vec2( 0.44323325, -0.97511554 ), 
        vec2( 0.53742981, -0.47373420 ), 
        vec2( -0.26496911, -0.41893023 ), 
        vec2( 0.79197514, 0.19090188 ), 
        vec2( -0.24188840, 0.99706507 ), 
        vec2( -0.81409955, 0.91437590 ), 
        vec2( 0.19984126, 0.78641367 ), 
        vec2( 0.14383161, -0.14100790 ) 
    );
    for (int i = 0; i < 4; i++) {
        int index = int(16.0 * random(floor(vPosition.xyz * 1000.0), i)) % 16;
        MRA.a -= 0.25 * (1.0 - textureProj(depth_texture, vec4(projCoords.xy + poissonDisk[index] / 700.0, projCoords.z / projCoords.w, projCoords.w)));
    }
    //MRA.a *= textureProj(depth_texture, vec4(projCoords.xy, projCoords.z, projCoords.w));
    //MRA.a = textureProj(depth_texture, projCoords);
}