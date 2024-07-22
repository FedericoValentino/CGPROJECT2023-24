#version 450
#include "utility.glsl"

layout(set = 0, binding = 0) uniform ViewUniforms {
    mat4 view;
    mat4 proj;
    vec3 pos;
} view;

float near = 0.01; //0.01
float far = 150; //100
layout(location = 1) in vec3 nearPoint;
layout(location = 2) in vec3 farPoint;
layout(location = 0) out vec4 outColor;

struct directLight{
    vec4 color;
    vec4 direction;
};

struct pointLight{
    vec4 color;
    vec4 position;
    float time;
    float size;
};


layout(binding = 1) uniform GlobalUniformBufferObject {
    pointLight lights[MAXBULLETS];
    pointLight pointLightsAirplane[10 * MAX_PLANE];
    pointLight explosions[MAXBULLETS];
    SpotLight spotlight;
    vec4 ambientLight;
    vec4 eyepos;
    directLight moon;
    int lightCounter;
    int pointLightsAirplaneCounter;
    int explosionCounter;
} gubo;


float computeDepth(vec3 pos) {
    vec4 clip_space_pos = view.proj * view.view * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

const float density = 0.01;
const float gradient = 1.5;
vec4 skycolor = vec4(0.863,0.761,0.918, 1.0f);

void main() {
    vec3 diffuseLight = gubo.ambientLight.xyz * gubo.ambientLight.w;
    vec3 surfaceNormal = normalize(vec3(0.0, 1.0f, 0.0f));
    float direction_diffuse = max(dot(surfaceNormal, normalize(-gubo.moon.direction.xyz)), 0);

    diffuseLight += direction_diffuse;


    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    vec4 positionToCam = view.view * vec4(fragPos3D, 1.0f);

    float distance = length(positionToCam.xyz);
    float visibility = exp(-pow((distance * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);

    gl_FragDepth = computeDepth(fragPos3D);

    outColor = vec4(0.067f, 0.445, 0.0f, 1.0f); // adding multiple resolution for the grid
    outColor = vec4(diffuseLight * outColor.xyz, 1.0);
    outColor = mix(skycolor, outColor, visibility);
}

