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
        vec4 ambientLight;
        directLight moon;
        int lightCounter;
        int pointLightsAirplaneCounter;
        int explosionCounter;
} gubo;

vec4 skycolor = vec4(0.012f,0.031f,0.11f, 1.0f);

void main()
{
    //ambient light
    vec3 diffuseLight = gubo.ambientLight.xyz * gubo.ambientLight.w;
    vec3 surfaceNormal = normalize(fragNorm);

    //Directional Light
    float direction_diffuse = max(dot(surfaceNormal, normalize(-gubo.moon.direction.xyz)), 0);
    diffuseLight += gubo.moon.color.xyz * gubo.moon.color.w * direction_diffuse;


    //Point Lights
    for(int i=0; i<gubo.lightCounter; i++)
    {
        diffuseLight += pointLightIntensityBlink(gubo.lights[i].size,
                                            gubo.lights[i].position,
                                            gubo.lights[i].color,
                                            gubo.lights[i].time,
                                            fragPos,
                                            surfaceNormal);
    }
    for(int i = 0; i < gubo.pointLightsAirplaneCounter; ++i)
    {
        diffuseLight += pointLightIntensityBlink(gubo.pointLightsAirplane[i].size,
                                            gubo.pointLightsAirplane[i].position,
                                            gubo.pointLightsAirplane[i].color,
                                            gubo.pointLightsAirplane[i].time,
                                            fragPos,
                                            surfaceNormal);
    }
    for(int i = 0; i < gubo.explosionCounter; ++i)
    {
        diffuseLight += pointLightIntensity(gubo.explosions[i].size,
                                            gubo.explosions[i].position,
                                            gubo.explosions[i].color,
                                            fragPos,
                                            surfaceNormal);
    }

    vec4 color = texture(tex1, fragUV);
    outColor = vec4(diffuseLight * color.xyz, 1.0);
    outColor = mix(skycolor, outColor, visibility);
}