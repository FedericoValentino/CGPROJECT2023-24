#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPos;    // vertex position
layout(location = 1) in vec3 inNorm; // vertex normal

layout(binding = 0) uniform PlaneLightUniformBufferObject
{
    mat4 translation[2];
    mat4 model;
    mat4 View;
    mat4 Proj;
    mat4 WVP;
}plubo;

void main()
{
    // Calculate the final position of the vertex in clip space
    gl_Position = plubo.Proj * plubo.View * plubo.model * plubo.translation[gl_InstanceIndex] * vec4(inPos, 1.0);
}