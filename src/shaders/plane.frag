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
} gubo;

void main()
{
    vec4 color = vec4(0.0, 1.0, 0.0, 1.0);

    outColor = color;
}