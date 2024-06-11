#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex1;

layout(binding = 2) uniform GlobalUniformBufferObject {
    vec3 pointLightPosition;
    vec4 pointLightColor;
    vec4 ambientLight;
} gubo;

void main()
{
    vec3 diffuseLight = gubo.ambientLight.xyz * gubo.ambientLight.w;
    vec3 surfaceNormal = normalize(fragNorm);

    vec3 directionToLight = gubo.pointLightPosition - fragPos;
    float attenuation = 1.0 / dot(directionToLight, directionToLight);
    float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
    vec3 intensity = gubo.pointLightColor.xyz * gubo.pointLightColor.w * attenuation;

    diffuseLight += intensity * cosAngIncidence;

    vec4 color = texture(tex1, fragUV);
    outColor = vec4(diffuseLight * color.xyz, 1.0);
}