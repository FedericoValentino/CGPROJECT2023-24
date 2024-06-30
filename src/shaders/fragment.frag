#version 450
#extension GL_ARB_separate_shader_objects : enable
#define MAXBULLETS 400

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex1;

layout(binding = 2) uniform GlobalUniformBufferObject {
        vec4 pointLightColor[MAXBULLETS];
        vec4 ambientLight;
        vec4 pointLightPosition[MAXBULLETS];
        int lightCounter;
} gubo;

void main()
{
    vec3 diffuseLight = gubo.ambientLight.xyz * gubo.ambientLight.w;
    vec3 surfaceNormal = normalize(fragNorm);

    for(int i=0; i<gubo.lightCounter; i++)
    {
        vec3 directionToLight = gubo.pointLightPosition[i].xyz - fragPos;
        float attenuation = 1.0 / dot(directionToLight, directionToLight);
        float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
        vec3 intensity = gubo.pointLightColor[i].xyz * gubo.pointLightColor[i].w * attenuation;

        diffuseLight += intensity * cosAngIncidence;
    }

    vec4 color = texture(tex1, fragUV);
    outColor = vec4(diffuseLight * color.xyz, 1.0);
}