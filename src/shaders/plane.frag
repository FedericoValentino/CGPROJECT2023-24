#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "utility.glsl"


layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in float visibility;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D playerTexture;
layout(binding = 3) uniform sampler2D enemyTexture;
layout(binding = 4) uniform sampler2D bossTexture;

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

layout( push_constant ) uniform constants
{
    int planeType;
} plane;

vec4 skycolor = vec4(0.012f,0.031f,0.11f, 1.0f);

void main()
{
    vec4 color;
    switch(plane.planeType)
    {
            case 0:
                color = texture(playerTexture, fragUV);
                break;
            case 1:
                color = texture(enemyTexture, fragUV);
                break;
            case 2:
                color = texture(bossTexture, fragUV);
                break;
    }

    vec3 specularReflection = {0.0, 0.0, 0.0};
    float k = 0.5f;
    float g = 1.0f; // decay factor
    float beta = 1.0f; // decay factor
    float roughness = 0.5f;
    vec3 specularColor = {1.0, 1.0, 1.0};
    vec3 cookTorrance;
    vec3 lightDirection = {0, 0, 0};
    vec3 halfVector = {0, 0, 0};
    vec3 cameraDirection = normalize(gubo.eyepos.xyz - fragPos);

    vec3 eps = {0.1f,0.1f,0.1f}; // make the colour of the texture a little brighter
    color.xyz = color.xyz + eps;

    //ambient light
    vec3 ambientLight = gubo.ambientLight.xyz * gubo.ambientLight.w;
    vec3 surfaceNormal = normalize(fragNorm);
    vec3 diffuseLight = color.xyz * ambientLight;


    //Directional Light
    float direction_diffuse = max(dot(surfaceNormal, normalize(-gubo.moon.direction.xyz)), 0);
    lightDirection = -gubo.moon.direction.xyz;
    halfVector = normalize(lightDirection + cameraDirection);
    diffuseLight += gubo.moon.color.xyz * gubo.moon.color.w * ((k * color.xyz * direction_diffuse ) + cookTorranceSpecular(k, roughness, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor));

    //Point Lights
    for(int i=0; i<gubo.lightCounter; i++)
    {
        lightDirection = normalize(gubo.lights[i].position.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        float dist = distance(gubo.lights[i].position.xyz,fragPos);
        diffuseLight += gubo.lights[i].color.xyz * gubo.lights[i].color.w * pow(g/dist,beta) *
        ( k * color.xyz * pointLightIntensityBlink(gubo.lights[i].size,
                                                    gubo.lights[i].position,
                                                    gubo.lights[i].color,
                                                    gubo.lights[i].time,
                                                    fragPos,
                                                    surfaceNormal) + cookTorranceSpecular(k, roughness, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor));
    }


    for(int i = 0;i < gubo.explosionCounter;++i)
    {
        lightDirection = normalize(gubo.explosions[i].position.xyz - fragPos);
        halfVector = normalize(lightDirection + cameraDirection);
        float dist = distance(gubo.explosions[i].position.xyz,fragPos);
        diffuseLight += gubo.explosions[i].color.xyz * gubo.explosions[i].color.w * pow(g/dist,beta) *
        ( k * color.xyz * pointLightIntensity(gubo.explosions[i].size,
                                            gubo.explosions[i].position,
                                            gubo.explosions[i].color,
                                            fragPos,
                                            surfaceNormal) + cookTorranceSpecular(k, roughness, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor));
    }

    lightDirection = normalize(gubo.spotlight.spotlightPosition.xyz - fragPos);
    halfVector = normalize(lightDirection + cameraDirection);
    float dist = distance(gubo.spotlight.spotlightPosition.xyz,fragPos);
    diffuseLight += spotlightIntensity(gubo.spotlight.spotlightPosition,
                                        gubo.spotlight.spotlightDirection,
                                        gubo.spotlight.spotlightColor,
                                        gubo.spotlight.spotlightCosIn,
                                        gubo.spotlight.spotlightCosOut,
                                        vec4(-lightDirection, 1.0f))
    * (k * color.xyz * clamp(dot(fragNorm,lightDirection),0.0f,1.0f) + cookTorranceSpecular(k, roughness, halfVector, surfaceNormal, cameraDirection, lightDirection, specularColor));

    outColor = vec4(diffuseLight, 1.0);
    outColor = mix(skycolor, outColor, visibility);
    outColor = clamp(outColor,0.0f,1.0f);
}