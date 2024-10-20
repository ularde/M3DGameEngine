#version 430 core

out vec4 fragColor;
in vec2 vTexCoords;

uniform sampler2D atomsphereResult;
uniform sampler2D lightingResult;
uniform sampler2D indirectLightingResult;
uniform sampler2D albedoChannel;
uniform sampler2D mraChannel;
uniform sampler2D forwardResult;

void main()
{
    vec4 forwardColor = texture(forwardResult, vTexCoords);

    vec3 albedo = pow(texture(albedoChannel, vTexCoords).rgb, vec3(2.2));
    float shadow = texture(mraChannel, vTexCoords).a;
    vec2 texelSize = 1.0 / vec2(textureSize(indirectLightingResult, 0));
    vec2 texelSizeShadow = 1.0 / vec2(textureSize(mraChannel, 0));

    vec4 indirectLightingSampled = vec4(0.0);
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            indirectLightingSampled += texture(indirectLightingResult, vTexCoords + offset).rgba;
        }
    }
    indirectLightingSampled = indirectLightingSampled / (4.0 * 4.0);

    float ssaoColor = indirectLightingSampled.a;
    vec3 ambient = vec3(0.03) * (albedo + indirectLightingSampled.rgb * (1.2 - shadow * ssaoColor)) * (texture(mraChannel, vTexCoords).b * ssaoColor);
    vec3 atomsphereColor = texture(atomsphereResult, vTexCoords).rgb;
    vec3 lightingColor = texture(lightingResult, vTexCoords).rgb;
    float lightingAlpha = texture(lightingResult, vTexCoords).a;
    lightingColor = ambient + shadow * lightingColor;
    lightingColor = lightingColor * (1.0 - forwardColor.a) + forwardColor.rgb * forwardColor.a;
    // HDR tonemapping
    lightingColor = 1.0 - exp(-1.0 * lightingColor);
    // gamma correct
    lightingColor = pow(lightingColor, vec3(1.0/2.2)); 
    fragColor = vec4(lightingColor + atomsphereColor * lightingAlpha * (1.0 - forwardColor.a), 1.0);
}