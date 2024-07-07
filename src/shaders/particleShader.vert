#version 450

#define MAXPARTICLES 20

layout(binding = 0) uniform particleUniformBufferObject
{
    mat4 ViewProj;
    mat4 Model;
    vec4 directions[MAXPARTICLES];
    float time;
}pubo;

// Grid position are in clipped space
vec3 particle[3] = vec3[] (
    vec3(-10, -10, 0), vec3(10, 10, 0), vec3(10, -10, 0)
);

void main() {
    vec4 p = pubo.ViewProj * pubo.Model * vec4(particle[gl_VertexIndex], 1.0f);
    p = vec4(p.xyz + pubo.directions[gl_InstanceIndex].xyz * pubo.time, 1.0f);
    gl_Position = p;
}
