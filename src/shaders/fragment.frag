#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "utility.glsl"

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in float visibility;


layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex1;

struct directLight{
    vec4 color;
    vec4 direction;
};

struct pointLight{
    vec4 color;
    vec4 position;
    float time;
    float size;
};

layout(binding = 2) uniform GlobalUniformBufferObject {
    pointLight lights[MAXBULLETS];
    pointLight pointLightsAirplane[10 * MAX_PLANE];
    pointLight explosions[MAXBULLETS];
    SpotLight spotlight;
    vec4 ambientLight;
    vec4 eyepos;
    directLight moon;
    int lightCounter;
    int pointLightsAirplaneCounter;
    int explosionCounter;
} gubo;

vec4 skycolor = vec4(0.012f,0.031f,0.11f, 1.0f);

void main()
{
    float k = 0.5f;
    float roughness = 0.5f;
    vec3 specularColor = {1.0, 1.0, 1.0};
    vec3 cookTorrance;
    vec4 color = texture(tex1, fragUV);
    vec3 lightDirection = {0, 0, 0};
    vec3 cameraDirection = {0, 0, 0};
    vec3 halfVector = {0, 0, 0};
    //vec3 specularReflection = {0.0, 0.0, 0.0};

    //ambient light
    vec3 diffuseLight = gubo.ambientLight.xyz * gubo.ambientLight.w;
    vec3 surfaceNormal = normalize(fragNorm);
    cookTorrance = diffuseLight;

    //Directional Light
    float direction_diffuse = max(dot(surfaceNormal, normalize(-gubo.moon.direction.xyz)), 0);
    cookTorrance += color.xyz * (gubo.moon.color.xyz * gubo.moon.color.w * direction_diffuse);


    //Point Lights
    for(int i=0; i<gubo.lightCounter; i++)
    {
        lightDirection = normalize(gubo.lights[i].position.xyz - fragPos);
        cameraDirection = normalize(gubo.eyepos.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        cookTorrance += (k * color.xyz * pointLightIntensityBlink(gubo.lights[i].size,
                                            gubo.lights[i].position,
                                            gubo.lights[i].color,
                                            gubo.lights[i].time,
                                            fragPos,
                                            surfaceNormal)) +
                         cookTorranceSpecular(k, roughness, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor);

        /*specularReflection += phongSpecularNonMetals(fragPos,
                                            gubo.lights[i].position,
                                            gubo.eyepos.xyz,
                                            surfaceNormal,
                                            160, gubo.lights[i].color.xyz);*/
    }
    for(int i = 0; i < gubo.pointLightsAirplaneCounter; ++i)
    {
        lightDirection = normalize(gubo.pointLightsAirplane[i].position.xyz - fragPos);
        cameraDirection = normalize(gubo.eyepos.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        cookTorrance += (k * color.xyz * pointLightIntensityBlink(gubo.pointLightsAirplane[i].size,
                                            gubo.pointLightsAirplane[i].position,
                                            gubo.pointLightsAirplane[i].color,
                                            gubo.pointLightsAirplane[i].time,
                                            fragPos,
                                            surfaceNormal)) +
                        cookTorranceSpecular(k, roughness, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor);

        /*specularReflection += phongSpecularNonMetals(fragPos,
                                                     gubo.pointLightsAirplane[i].position,
                                                     gubo.eyepos.xyz,
                                                     surfaceNormal,
                                                     160, gubo.pointLightsAirplane[i].color.xyz);*/
    }
    for(int i = 0; i < gubo.explosionCounter; ++i)
    {
        lightDirection = normalize(gubo.explosions[i].position.xyz - fragPos);
        cameraDirection = normalize(gubo.eyepos.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        cookTorrance += (k * color.xyz * pointLightIntensity(gubo.explosions[i].size,
                                            gubo.explosions[i].position,
                                            gubo.explosions[i].color,
                                            fragPos,
                                            surfaceNormal)) +
                        cookTorranceSpecular(k, roughness, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor);

        /*specularReflection += phongSpecularNonMetals(fragPos,
                                                     gubo.explosions[i].position,
                                                     gubo.eyepos.xyz,
                                                     surfaceNormal,
                                                     160,
                                                     gubo.explosions[i].color.xyz);*/
    }

    //Spot Light
    if(gubo.spotlight.spotlightColor != vec4(0.0f, 0.0f, 0.0f, 0.0f))
    {
        lightDirection = normalize(gubo.spotlight.spotlightPosition.xyz - fragPos);
        cameraDirection = normalize(gubo.eyepos.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        cookTorrance += (k * color.xyz * spotlightIntensity(gubo.spotlight.spotlightPosition,
                                        gubo.spotlight.spotlightDirection,
                                        gubo.spotlight.spotlightColor,
                                        gubo.spotlight.spotlightCosIn,
                                        gubo.spotlight.spotlightCosOut,
                                        vec4(fragPos - gubo.spotlight.spotlightPosition.xyz, 1.0f))) +
                        cookTorranceSpecular(k, roughness, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor);
    }

    /*if(gubo.spotlight.spotlightColor != vec4(0.0f, 0.0f, 0.0f, 0.0f))
        specularReflection += phongSpecularNonMetals(fragPos,
                                                 gubo.spotlight.spotlightPosition,
                                                 gubo.eyepos.xyz,
                                                 surfaceNormal,
                                                 160, gubo.spotlight.spotlightColor.xyz);*/


    outColor = vec4(cookTorrance, 1.0);
    outColor = mix(skycolor, outColor, visibility);
}