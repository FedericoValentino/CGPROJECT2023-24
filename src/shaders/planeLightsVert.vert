#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "utility.glsl"

layout(location = 0) in vec3 inPos;    // vertex position
layout(location = 1) in vec3 inNorm; // vertex normal



layout(binding = 0) uniform PlaneLightUniformBufferObject
{
    mat4 WVP[10 * MAX_PLANE];
    int counter;
}plubo;

void main()
{
    // Calculate the final position of the vertex in clip space
    gl_Position = plubo.WVP[gl_InstanceIndex] * vec4(inPos, 1.0);
}