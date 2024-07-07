#version 450
#define MAXPARTICLES 20


layout(location = 0) out vec4 outColor;
layout(location = 1) in flat int instanceID;

layout(binding = 0) uniform particleUniformBufferObject
{
    mat4 ViewProj;
    mat4 Model;
    vec4 directions[MAXPARTICLES];
    float time;
}pubo;

void main() {

    float fireVal = cos(pubo.time + instanceID);
    if(fireVal < -0.5)
    {
        outColor = vec4(vec3(0.643,0.055,0.016), 1.0);
    }
    if(fireVal > 0.5)
    {
        outColor = vec4(vec3(0.984,0.463,0.016), 1.0);
    }
    else
    {
        outColor = vec4(vec3(0.796,0.125,0.016), 1.0);
    }
}
