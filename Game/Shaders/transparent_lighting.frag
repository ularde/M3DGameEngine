#version 430 core

layout (location = 0) out vec4 accum;
layout (location = 1) out float reveal;

in vec3 vPosition;
in mat3 vTBN;
in vec2 vTexCoords;
in vec4 vShadowCoord;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 camPos;
uniform mat4 view;

uniform sampler2D texture_albedo;
uniform sampler2D texture_normal;
uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;
uniform sampler2D texture_AO;
uniform sampler2D texture_opacity;
uniform sampler2DShadow depth_texture;

uniform vec3 albedo_color;
uniform float metallic_color;
uniform float roughness_color;
uniform float ao_color;
uniform float opacity_color;

uniform bool use_default_norm;
vec3 default_normal = vec3(0.5, 0.5, 1.0);

const float PI = 3.14159265359;

float random(vec3 seed, int i) {
    vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------

vec3 DoShading(vec3 WorldPos, vec3 N, vec3 albedo, float metallic, float roughness, float ao)
{
    vec3 V = normalize(camPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    //for(int i = 0; i < 4; ++i) 
    //{
        // calculate per-light radiance
        //vec3 L = normalize(lightPositions[i] - WorldPos);
        //vec3 L = normalize(lightDir - WorldPos);
        vec3 L = normalize(lightDir);
        vec3 H = normalize(V + L);
        //float distance = length(lightPositions[i] - WorldPos);
        //float attenuation = 1.0 / (distance * distance);
        //vec3 radiance = lightColors[i] * attenuation;
        vec3 radiance = lightColor * 1.0;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    //}
    
        vec4 projCoords = vShadowCoord * 0.5 + 0.5;
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    float shadow = 1.0;
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
        shadow -= 0.25 * (1.0 - textureProj(depth_texture, vec4(projCoords.xy + poissonDisk[index] / 700.0, projCoords.z / projCoords.w, projCoords.w)));
    }

    /*
    shadow += texture(mraChannel, vTexCoords - 3.0 * blurSize).a * 0.0215;
    shadow += texture(mraChannel, vTexCoords - 2.0 * blurSize).a * 0.136;
    shadow += texture(mraChannel, vTexCoords - 1.0 * blurSize).a * 0.341;
    shadow += texture(mraChannel, vTexCoords).a * 0.09;
    shadow += texture(mraChannel, vTexCoords + 1.0 * blurSize).a * 0.341;
    shadow += texture(mraChannel, vTexCoords + 2.0 * blurSize).a * 0.136;
    shadow += texture(mraChannel, vTexCoords + 3.0 * blurSize).a * 0.0215;
    */

    vec3 color = ambient + shadow * Lo;
    //vec3 color = ambient + Lo;

    // HDR tonemapping
    //color = color / (color + vec3(1.0));
    //color = 1.0 - exp(-1.0 * color);
    // gamma correct
    //color = pow(color, vec3(1.0/2.2)); 

    return color;
}

float MaxInVec3(vec3 v) { return max(max(v.x, v.y), v.z); }

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
    vec3 normal = normalize(vTBN * tangentNormal);

    vec3 fragPos = vPosition;
    vec3 albedo = pow(texture(texture_albedo, vTexCoords).rgb, vec3(2.2));
    float metallic  = texture(texture_metallic, vTexCoords).r;
    float roughness = texture(texture_roughness, vTexCoords).r;
    float ao = texture(texture_AO, vTexCoords).r;
    float opacity = texture(texture_opacity, vTexCoords).r;
    vec3 N = normalize(normal);

    albedo = albedo.rgb + albedo_color;
    metallic = metallic + metallic_color;
    roughness = roughness + roughness_color;
    ao = ao + ao_color;
    opacity = opacity + opacity_color;

    //float SSAO = CaculateSSAO(fragPos, N);
    vec4 worldPos = inverse(view) * vec4(fragPos, 1.0);
    //vec3 shadingResult = DoShading(worldPos.xyz, N, albedo, metallic, roughness, ao * SSAO);
    vec4 shadingResult = vec4(DoShading(worldPos.xyz, N, albedo, metallic, roughness, ao), opacity);
    float weight = max(min(1.0, MaxInVec3(shadingResult.rgb) * shadingResult.a), shadingResult.a) * clamp(0.03 / (1e-5 + pow(1.0 - gl_FragCoord.z * 0.9, 4.0)), 1e-2, 3e3);
    accum = vec4(shadingResult.rgb * shadingResult.a, shadingResult.a) * weight;
    reveal = shadingResult.a;
}