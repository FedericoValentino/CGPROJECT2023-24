#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "utility.glsl"

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in float visibility;
layout(location = 4) in flat int tileType;


layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D floorTex;
layout(binding = 3) uniform sampler2D houseTex;
layout(binding = 4) uniform sampler2D skyscraperTex;

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

layout(binding = 5) uniform FloorBuffer {
    vec4 spotlightPosition[MAXFLOORSPOTLIGHTS];
    vec4 spotlightDirection;
    vec4 spotlightColor;
    float spotLightCosIn;
    float spotLightCosOut;
    int counter;
} floorBuffer;

vec4 skycolor = vec4(0.012f,0.031f,0.11f, 1.0f);

void main()
{
    vec4 color;
    float k;
    float roughness;
    float refraction;
    vec3 specularColor = vec3(0.0f, 0.0f, 0.0f);

    switch(tileType)
    {
        case 0:
            color = texture(floorTex, fragUV);
            k = 0.5f;
            roughness = 0.5f;
            refraction = 1.2f;
            specularColor = vec3(1.0f, 1.0f, 1.0f);
            break;
        case 1:
            color = texture(houseTex, fragUV);
            k = 0.5f;
            roughness = 0.5f;
            refraction = 1.2f;
            specularColor = vec3(1.0f, 1.0f, 1.0f);
            break;
        case 2:
            color = texture(skyscraperTex, fragUV);
            k = 0.3f;
            roughness = 0.1f;
            refraction = 1.5f;
            specularColor = vec3(0.588,0.824,0.89);
            break;
    }

    vec3 specularReflection = {0.0, 0.0, 0.0};
    float g = 1.0f; // decay factor
    float beta = 1.5f; // decay factor
    vec3 cookTorrance;
    vec3 lightDirection = {0, 0, 0};
    vec3 halfVector = {0, 0, 0};
    vec3 cameraDirection = normalize(gubo.eyepos.xyz - fragPos);

    //ambient light
    vec3 diffuseLight = gubo.ambientLight.xyz * gubo.ambientLight.w;
    vec3 surfaceNormal = normalize(fragNorm);
    cookTorrance = color.xyz * diffuseLight;

    //Directional Light
    float direction_diffuse = max(dot(surfaceNormal, normalize(-gubo.moon.direction.xyz)), 0);
    lightDirection = -gubo.moon.direction.xyz;
    halfVector = normalize(lightDirection + cameraDirection);
    cookTorrance += gubo.moon.color.xyz * gubo.moon.color.w * ((k * color.xyz * direction_diffuse ) + cookTorranceSpecular(k, roughness, refraction, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor));

    //Point Lights
    for(int i=0; i<gubo.lightCounter; i++)
    {
        lightDirection = normalize(gubo.lights[i].position.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        float dist = distance(gubo.lights[i].position.xyz,fragPos);
        cookTorrance += gubo.lights[i].color.xyz * gubo.lights[i].color.w * pow(g/dist,beta) *
        ( k * color.xyz * pointLightIntensityBlink(gubo.lights[i].size,
                                                                gubo.lights[i].position,
                                                                gubo.lights[i].color,
                                                                gubo.lights[i].time,
                                                                fragPos,
                                                                surfaceNormal) + cookTorranceSpecular(k, roughness, refraction, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor));

        /*specularReflection += phongSpecularNonMetals(fragPos,
                                            gubo.lights[i].position,
                                            gubo.eyepos.xyz,
                                            surfaceNormal,
                                            160, gubo.lights[i].color.xyz);*/
    }

    for(int i = 0; i < gubo.pointLightsAirplaneCounter; ++i)
    {
        lightDirection = normalize(gubo.pointLightsAirplane[i].position.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        float dist = distance(gubo.pointLightsAirplane[i].position.xyz,fragPos);
        cookTorrance += gubo.pointLightsAirplane[i].color.xyz * gubo.pointLightsAirplane[i].color.w * pow(g/dist,beta) *
                        ((k * color.xyz * pointLightIntensityBlink(gubo.pointLightsAirplane[i].size,
                                            gubo.pointLightsAirplane[i].position,
                                            gubo.pointLightsAirplane[i].color,
                                            gubo.pointLightsAirplane[i].time,
                                            fragPos,
                                            surfaceNormal)) +
                        cookTorranceSpecular(k, roughness, refraction, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor));

        /*specularReflection += phongSpecularNonMetals(fragPos,
                                                     gubo.pointLightsAirplane[i].position,
                                                     gubo.eyepos.xyz,
                                                     surfaceNormal,
                                                     160, gubo.pointLightsAirplane[i].color.xyz);*/
    }

    for(int i = 0; i < gubo.explosionCounter; ++i)
    {
        lightDirection = normalize(gubo.explosions[i].position.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        float dist = distance(gubo.explosions[i].position.xyz,fragPos);
        cookTorrance += gubo.explosions[i].color.xyz * gubo.explosions[i].color.w * pow(100.0f/dist,1.0f) *
        ((k * color.xyz * pointLightIntensity(gubo.explosions[i].size,
                                            gubo.explosions[i].position,
                                            gubo.explosions[i].color,
                                            fragPos,
                                            surfaceNormal)) +
                        cookTorranceSpecular(k, roughness, refraction, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor));

        /*specularReflection += phongSpecularNonMetals(fragPos,
                                                     gubo.explosions[i].position,
                                                     gubo.eyepos.xyz,
                                                     surfaceNormal,
                                                     160,
                                                     gubo.explosions[i].color.xyz);*/
    }


    //Zeppelin spot Light
    if(gubo.spotlight.spotlightColor != vec4(0.0f, 0.0f, 0.0f, 0.0f) && distance(gubo.spotlight.spotlightPosition.xyz,fragPos) < 50.0f)
    {
        lightDirection = normalize(gubo.spotlight.spotlightPosition.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        float dist = distance(gubo.spotlight.spotlightPosition.xyz,fragPos);
        cookTorrance += spotlightIntensity(gubo.spotlight.spotlightPosition,
                                        gubo.spotlight.spotlightDirection,
                                        gubo.spotlight.spotlightColor,
                                        gubo.spotlight.spotlightCosIn,
                                        gubo.spotlight.spotlightCosOut,
                                        vec4(-lightDirection, 1.0f)) * (k * color.xyz * clamp(dot(fragNorm,lightDirection),0.0f,1.0f)
        + cookTorranceSpecular(k, roughness, refraction, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor));
    }

    /*if(gubo.spotlight.spotlightColor != vec4(0.0f, 0.0f, 0.0f, 0.0f))
        specularReflection += phongSpecularNonMetals(fragPos,
                                                 gubo.spotlight.spotlightPosition,
                                                 gubo.eyepos.xyz,
                                                 surfaceNormal,
                                                 160, gubo.spotlight.spotlightColor.xyz);
*/
    // Environment SpotLights
    // Lambert + Phong(Cook Torrence is too expensive)

    for (int i = 0; i < floorBuffer.counter; ++i)
     {
         if(distance(floorBuffer.spotlightPosition[i].xyz,fragPos) < 15.0f)
         {
             lightDirection = normalize(floorBuffer.spotlightPosition[i].xyz - fragPos);
             halfVector = normalize(lightDirection + cameraDirection);
             float dist = distance(floorBuffer.spotlightPosition[i].xyz, fragPos);
             cookTorrance += spotlightIntensity(floorBuffer.spotlightPosition[i],
                                                floorBuffer.spotlightDirection,
                                                floorBuffer.spotlightColor,
                                                floorBuffer.spotLightCosIn,
                                                floorBuffer.spotLightCosOut,
                                                vec4(-lightDirection, 1.0f)) * (color.xyz * color.w * clamp(dot(lightDirection, fragNorm), 0, 1));
         }
     }



    outColor = vec4(cookTorrance, 1.0);
    outColor = mix(skycolor, outColor, visibility);
    outColor = clamp(outColor,0.0f,1.0f);
}