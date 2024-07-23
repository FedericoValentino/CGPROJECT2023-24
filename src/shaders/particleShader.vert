#version 450

#define MAXPARTICLES 20
#define PARTICLESPEED 15

layout(binding = 0) uniform particleUniformBufferObject
{
    mat4 ViewProj;
    mat4 Model;
    vec4 directions[MAXPARTICLES];
    float time;
}pubo;


layout(location = 1) out int instanceID;

// Grid position are in clipped space
vec3 particle[24] = vec3[] (
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

    mat4 translation = BuildTranslation(pubo.directions[gl_InstanceIndex].xyz * pubo.time * PARTICLESPEED);
    mat4 newModel = translation * pubo.Model;
    vec4 p = pubo.ViewProj * newModel * vec4(particle[gl_VertexIndex], 1.0f);
    gl_Position = p;
    instanceID = gl_InstanceIndex;
}
