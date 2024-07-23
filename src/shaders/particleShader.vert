#version 450

#define MAXPARTICLES 20
#define PARTICLESPEED 15

layout(binding = 0) uniform particleUniformBufferObject
{
    mat4 ViewProj;
    mat4 Model[MAXPARTICLES];
    vec4 directions[MAXPARTICLES*MAXPARTICLES];
    float time[MAXPARTICLES];
}pubo;


layout(location = 1) out int instanceID;

// Grid position are in clipped space
vec3 particles[24] = vec3[] (
    vec3(0, 0, 0), vec3(0, 0.1, 0), vec3(0.04, 0.04, 0),
    vec3(0, 0, 0), vec3(0.1, 0, 0), vec3(0.04, 0.04, 0),
    vec3(0, 0, 0), vec3(0.1, 0, 0), vec3(0.04, -0.04, 0),
    vec3(0, 0, 0), vec3(0, -0.1, 0), vec3(0.04, -0.04, 0),
    vec3(0, 0, 0), vec3(0, -0.1, 0), vec3(-0.04, -0.04, 0),
    vec3(0, 0, 0), vec3(-0.1, 0, 0), vec3(-0.04, -0.04, 0),
    vec3(0, 0, 0), vec3(-0.1, 0, 0), vec3(-0.04, 0.04, 0),
    vec3(0, 0, 0), vec3(0, 0.1, 0), vec3(-0.04, 0.04, 0)
);

mat4 BuildTranslation(vec3 delta)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(delta, 1.0));
}

void main() {

    int particleIndex = int(floor(gl_InstanceIndex/MAXPARTICLES));
    mat4 translation = BuildTranslation(pubo.directions[gl_InstanceIndex].xyz * pubo.time[particleIndex] * PARTICLESPEED);
    mat4 newModel = translation * pubo.Model[particleIndex];
    vec4 p = pubo.ViewProj * newModel * vec4(particles[gl_VertexIndex], 1.0f);
    gl_Position = p;
    instanceID = gl_InstanceIndex;
}
