#version 430 core

out vec4 fragColor;

in vec2 vTexCoords;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 camPos;
uniform sampler2D positionChannel;
uniform sampler2D normalChannel;
uniform sampler2D albedoChannel;
uniform sampler2D mraChannel;
uniform sampler2D atomsphereResult;

// for SSAO
uniform sampler2D noiseTex;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 framebufferSize;
const vec3 samples[64] = {
vec3(0.0497709, -0.0447092, 0.0499634),
vec3(0.0145746, 0.0165311, 0.00223862),
vec3(-0.0406477, -0.0193748, 0.0319336),
vec3(0.0137781, -0.091582, 0.0409242),
vec3(0.055989, 0.0597915, 0.0576589),
vec3(0.0922659, 0.0442787, 0.0154511),
vec3(-0.00203926, -0.054402, 0.066735),
vec3(-0.00033053, -0.000187337, 0.000369319),
vec3(0.0500445, -0.0466499, 0.0253849),
vec3(0.0381279, 0.0314015, 0.032868),
vec3(-0.0318827, 0.0204588, 0.0225149),
vec3(0.0557025, -0.0369742, 0.0544923),
vec3(0.0573717, -0.0225403, 0.0755416),
vec3(-0.0160901, -0.00376843, 0.0554733),
vec3(-0.0250329, -0.024829, 0.0249512),
vec3(-0.0336879, 0.0213913, 0.0254024),
vec3(-0.0175298, 0.0143856, 0.00534829),
vec3(0.0733586, 0.112052, 0.0110145),
vec3(-0.0440559, -0.0902836, 0.083683),
vec3(-0.0832772, -0.00168341, 0.0849867),
vec3(-0.0104057, -0.0328669, 0.019273),
vec3(0.00321131, -0.00488206, 0.00416381),
vec3(-0.00738321, -0.0658346, 0.067398),
vec3(0.0941413, -0.00799846, 0.14335),
vec3(0.0768329, 0.126968, 0.106999),
vec3(0.000392719, 0.000449695, 0.00030161),
vec3(-0.104793, 0.0654448, 0.101737),
vec3(-0.00445152, -0.119638, 0.161901),
vec3(-0.0745526, 0.0344493, 0.224138),
vec3(-0.0027583, 0.00307776, 0.00292255),
vec3(-0.108512, 0.142337, 0.166435),
vec3(0.046882, 0.103636, 0.0595757),
vec3(0.134569, -0.0225121, 0.130514),
vec3(-0.16449, -0.155644, 0.12454),
vec3(-0.187666, -0.208834, 0.0577699),
vec3(-0.043722, 0.0869255, 0.0747969),
vec3(-0.00256364, -0.00200082, 0.00406967),
vec3(-0.0966957, -0.182259, 0.299487),
vec3(-0.225767, 0.316061, 0.089156),
vec3(-0.0275051, 0.287187, 0.317177),
vec3(0.207216, -0.270839, 0.110132),
vec3(0.0549017, 0.104345, 0.323106),
vec3(-0.13086, 0.119294, 0.280219),
vec3(0.154035, -0.0653706, 0.229842),
vec3(0.0529379, -0.227866, 0.148478),
vec3(-0.187305, -0.0402247, 0.0159264),
vec3(0.141843, 0.0471631, 0.134847),
vec3(-0.0442676, 0.0556155, 0.0558594),
vec3(-0.0235835, -0.0809697, 0.21913),
vec3(-0.142147, 0.198069, 0.00519361),
vec3(0.158646, 0.230457, 0.0437154),
vec3(0.03004, 0.381832, 0.163825),
vec3(0.083006, -0.309661, 0.0674131),
vec3(0.226953, -0.23535, 0.193673),
vec3(0.381287, 0.332041, 0.529492),
vec3(-0.556272, 0.294715, 0.301101),
vec3(0.42449, 0.00564689, 0.117578),
vec3(0.3665, 0.00358836, 0.0857023),
vec3(0.329018, 0.0308981, 0.178504),
vec3(-0.0829377, 0.512848, 0.0565553),
vec3(0.867363, -0.00273376, 0.100138),
vec3(0.455745, -0.772006, 0.0038413),
vec3(0.417291, -0.154846, 0.462514),
vec3(-0.442722, -0.679282, 0.186503)};
const float radius = 1.0;
const float bias = 0.025;

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

    vec3 color = ambient + shadow * Lo;

    // HDR tonemapping
    //color = color / (color + vec3(1.0));
    //color = 1.0 - exp(-1.0 * color);
    // gamma correct
    //color = pow(color, vec3(1.0/2.2)); 

    return color;
}

float CaculateSSAO(vec3 fragPos, vec3 N)
{
    vec2 noiseScale = framebufferSize / 4.0;
    vec3 randomVec = normalize(texture(noiseTex, vTexCoords * noiseScale).xyz);
    vec3 tangent = normalize(randomVec - N * dot(randomVec, N));
    vec3 bitangent = cross(N, tangent);
    mat3 TBN = mat3(tangent, bitangent, N);
    float occlusion = 0.0;
    for(int i = 0; i < 16; ++i)
    {
        // get sample position
        vec3 samplePos = TBN * samples[i]; // from tangent to view-space
        samplePos = fragPos + samplePos * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture(positionChannel, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / 16);
    return occlusion;
}

void main()
{
    float albedoAlpha = texture(albedoChannel, vTexCoords).a;

    vec4 atomspherColor = texture(atomsphereResult, vTexCoords).rgba;

    vec3 fragPos = texture(positionChannel, vTexCoords).rgb;
    vec3 albedo = pow(texture(albedoChannel, vTexCoords).rgb, vec3(2.2));
    float metallic  = texture(mraChannel, vTexCoords).r;
    float roughness = texture(mraChannel, vTexCoords).g;
    float ao = texture(mraChannel, vTexCoords).b;
    vec3 N = normalize(texture(normalChannel, vTexCoords).rgb);

    float SSAO = CaculateSSAO(fragPos, N);
    vec4 worldPos = inverse(view) * vec4(fragPos, 1.0);
    vec3 shadingResult = DoShading(worldPos.xyz, N, albedo, metallic, roughness, ao * SSAO);
    // HDR tonemapping
    shadingResult = 1.0 - exp(-1.0 * shadingResult);
    // gamma correct
    shadingResult = pow(shadingResult, vec3(1.0/2.2)); 

    fragColor = atomspherColor * albedoAlpha;
    fragColor += vec4(shadingResult, 1.0);
    //fragColor = vec4(ao * SSAO, ao * SSAO, ao * SSAO, 1.0);
}
