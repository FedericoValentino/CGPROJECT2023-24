#version 450

#define MAPDIM 24

layout(binding = 0) buffer UniformBufferObject {
    mat4 mvpMat[24*24];
    mat4 mMat[24*24];
    mat4 nMat[24*24];
} ubo;


layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 norm;

layout (location = 0) out vec3 fragPos;
layout (location = 1) out vec3 fragNorm;
layout (location = 2) out vec2 fragUV;



void main()
{
    gl_Position = ubo.mvpMat[gl_InstanceIndex] * vec4(pos, 1.0);
    fragPos = (ubo.mMat[gl_InstanceIndex] * vec4(pos, 1.0)).xyz;
    fragNorm = norm;
    fragUV = texCoord;
}