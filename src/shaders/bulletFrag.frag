#version 450
#extension GL_ARB_separate_shader_objects : enable
#define MAXBULLETS 400

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in flat int instanceID;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex1;


struct bulletFlicker
{
    vec4 color;
    float time;
    float size;
};

layout(binding = 2) uniform FlickeringObject{
bulletFlicker flick[MAXBULLETS];
}fo;

void main()
{
    float frequency = 5.0f;
    float intensity = 1;
    if(fo.flick[instanceID].time > 0.3f)
    {
        intensity = abs(cos(frequency * fo.flick[instanceID].time));
    }
    outColor = fo.flick[instanceID].color * intensity;

}