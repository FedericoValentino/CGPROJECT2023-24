#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex1;

layout(binding = 2) uniform GlobalUniformBufferObject {
    vec3 lightDir;
    vec4 lightColor;
    vec3 eyePos;
} gubo;

void main()
{
    vec3 Norm = fragNorm;
    vec3 color = vec3(1.0, 0.0, 0.0);

    float intensity = dot(gubo.lightDir, normalize(Norm));

    if(fragPos.z == 0)
    {
        outColor = vec4(1.0);
    }
    else
    {
        color = color*intensity;
        outColor = vec4(color, 1.0);
    }
}