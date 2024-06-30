#version 450
#extension GL_ARB_separate_shader_objects : enable
#define MAXBULLETS 400

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex1;

struct pointLight{
    vec4 color;
    vec4 position;
    float time;
    float size;
};

layout(binding = 2) uniform GlobalUniformBufferObject {
        pointLight lights[MAXBULLETS];
        vec4 ambientLight;
        int lightCounter;
} gubo;

void main()
{
    vec3 diffuseLight = gubo.ambientLight.xyz * gubo.ambientLight.w;
    vec3 surfaceNormal = normalize(fragNorm);


    for(int i=0; i<gubo.lightCounter; i++)
    {
        float frequency = gubo.lights[i].size;
        vec3 directionToLight = gubo.lights[i].position.xyz - fragPos;
        float attenuation = 1.0 / dot(directionToLight, directionToLight);
        float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
        vec3 intensity = gubo.lights[i].color.xyz *
        gubo.lights[i].color.w *
        attenuation;
        if(gubo.lights[i].time > 0.3f)
        {
            intensity *= abs(cos(frequency * gubo.lights[i].time));
        }

        diffuseLight += intensity * cosAngIncidence;
    }

    vec4 color = texture(tex1, fragUV);
    outColor = vec4(diffuseLight * color.xyz, 1.0);
}