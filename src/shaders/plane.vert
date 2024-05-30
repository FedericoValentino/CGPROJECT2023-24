#version 450


layout(binding = 0) uniform UniformBufferObject {
    mat4 mvpMat;
    mat4 mMat;
    mat4 nMat;
} ubo;


layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;

layout (location = 0) out vec3 fragPos;
layout (location = 1) out vec3 fragNorm;
layout (location = 2) out vec2 fragUV;



void main()
{
    gl_Position = ubo.mvpMat * vec4(pos, 1.0);
    fragPos = (ubo.mMat * vec4(pos, 1.0)).xyz;
    fragNorm = norm;
    fragUV = texCoord;
}