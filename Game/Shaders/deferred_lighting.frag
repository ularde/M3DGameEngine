#version 430 core

out vec4 fragColor;

in vec2 vTexCoords;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 camPos;
uniform mat4 view;
uniform sampler2D positionChannel;
uniform sampler2D normalChannel;
uniform sampler2D albedoChannel;
uniform sampler2D mraChannel;

const float PI = 3.14159265359;

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
    
       
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    float shadow = 0.0;
    //float blurSize = 1.0 / 400.0;

    shadow = texture(mraChannel, vTexCoords).a;
    /*
    shadow += texture(mraChannel, vTexCoords - 3.0 * blurSize).a * 0.0215;
    shadow += texture(mraChannel, vTexCoords - 2.0 * blurSize).a * 0.136;
    shadow += texture(mraChannel, vTexCoords - 1.0 * blurSize).a * 0.341;
    shadow += texture(mraChannel, vTexCoords).a * 0.09;
    shadow += texture(mraChannel, vTexCoords + 1.0 * blurSize).a * 0.341;
    shadow += texture(mraChannel, vTexCoords + 2.0 * blurSize).a * 0.136;
    shadow += texture(mraChannel, vTexCoords + 3.0 * blurSize).a * 0.0215;
    */

    //vec3 color = ambient + shadow * Lo;
    vec3 color = Lo;

    // HDR tonemapping
    //color = color / (color + vec3(1.0));
    //color = 1.0 - exp(-1.0 * color);
    // gamma correct
    //color = pow(color, vec3(1.0/2.2)); 

    return color;
}

void main()
{
    float albedoAlpha = texture(albedoChannel, vTexCoords).a;

    //vec4 atomspherColor = texture(atomsphereResult, vTexCoords).rgba;

    vec3 fragPos = texture(positionChannel, vTexCoords).rgb;
    vec3 albedo = pow(texture(albedoChannel, vTexCoords).rgb, vec3(2.2));
    float metallic  = texture(mraChannel, vTexCoords).r;
    float roughness = texture(mraChannel, vTexCoords).g;
    float ao = texture(mraChannel, vTexCoords).b;
    vec3 N = normalize(texture(normalChannel, vTexCoords).rgb);

    //float SSAO = CaculateSSAO(fragPos, N);
    vec4 worldPos = inverse(view) * vec4(fragPos, 1.0);
    //vec3 shadingResult = DoShading(worldPos.xyz, N, albedo, metallic, roughness, ao * SSAO);
    vec3 shadingResult = DoShading(worldPos.xyz, N, albedo, metallic, roughness, ao);
    // HDR tonemapping
    //shadingResult = 1.0 - exp(-1.0 * shadingResult);
    // gamma correct
    //shadingResult = pow(shadingResult, vec3(1.0/2.2)); 

    //fragColor = atomspherColor * albedoAlpha;
    fragColor = vec4(shadingResult, albedoAlpha);
    //fragColor = vec4(ao * SSAO, ao * SSAO, ao * SSAO, 1.0);
}
