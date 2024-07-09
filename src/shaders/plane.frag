#version 450
#extension GL_ARB_separate_shader_objects : enable
#define MAXBULLETS 400
#define MAX_PLANE 3

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
    vec4 ambientLight;
    directLight moon;
    int lightCounter;
    int pointLightsAirplaneCounter;
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
        float frequency = gubo.lights[i].size;
        vec3 directionToLight = gubo.lights[i].position.xyz - fragPos;
        float attenuation = 1.0 / dot(directionToLight, directionToLight);
        float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
        vec3 intensity = gubo.lights[i].color.xyz *
        gubo.lights[i].color.w *
        attenuation;
        if(gubo.lights[i].time > 0.3f)
        {
            intensity *= abs(cos(frequency * gubo.lights[i].time));
        }

        diffuseLight += intensity * cosAngIncidence;
    }

    for(int i = 0;i < gubo.pointLightsAirplaneCounter;++i)
    {
        float frequency = gubo.pointLightsAirplane[i].size;
        vec3 directionToLight = gubo.pointLightsAirplane[i].position.xyz - fragPos;
        float attenuation = 1.0 / dot(directionToLight, directionToLight);
        float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
        vec3 intensity = 5.0f * gubo.pointLightsAirplane[i].color.xyz *
        gubo.pointLightsAirplane[i].color.w *
        attenuation;
        if(gubo.pointLightsAirplane[i].time > 0.3f)
        {
            intensity *= abs(cos(frequency * gubo.pointLightsAirplane[i].time));
        }

        diffuseLight += intensity * cosAngIncidence;
    }

    vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    outColor = vec4(diffuseLight * color.xyz, 1.0);
    outColor = mix(skycolor, outColor, visibility);
}