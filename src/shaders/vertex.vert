#version 450

#include "utility.glsl"

layout(binding = 0) buffer UniformBufferObject {
    mat4 proj;
    mat4 view;
    mat4 mMat[MAPDIM*MAPDIM];
    mat4 nMat[MAPDIM*MAPDIM];
} ubo;


layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 norm;

layout (location = 0) out vec3 fragPos;
layout (location = 1) out vec3 fragNorm;
layout (location = 2) out vec2 fragUV;
layout (location = 3) out float visibility;
layout(location = 4) out int tileType;

const float density = 0.01;
const float gradient = 1.5;

layout( push_constant ) uniform constants
{
    mat4 view;
    int tileType;
} tile;

void main()
{
    vec4 worldPosition = ubo.mMat[gl_InstanceIndex] * vec4(pos, 1.0);
    vec4 positionToCam = tile.view * worldPosition;
    gl_Position = ubo.proj * tile.view * ubo.mMat[gl_InstanceIndex] * vec4(pos, 1.0);
    fragPos = (ubo.mMat[gl_InstanceIndex] * vec4(pos, 1.0)).xyz;
    fragNorm = norm;
    fragUV = texCoord;

    float distance = length(positionToCam.xyz);
    visibility = exp(-pow((distance * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);
    tileType = tile.tileType;
}