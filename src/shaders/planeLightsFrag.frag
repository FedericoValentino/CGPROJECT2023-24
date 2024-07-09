#version 450
#extension GL_ARB_separate_shader_objects : enable

struct bulletFlicker
{
    vec4 color;
    float time;
    float size;
};

layout(binding = 1) uniform FlickeringObject{
    bulletFlicker flick;
}fo;

layout(location = 0) out vec4 outColor; // Final output color

void main()
{
    float frequency = 1.0;

    // Calculate the flickering intensity based on time and frequency
    float intensity = abs(cos(frequency * fo.flick.time));

    // Define colors
    vec3 color = fo.flick.color.xyz;
    vec3 blackColor = vec3(0.0, 0.0, 0.0); // White color

    // Use the intensity to interpolate between green and white
    vec3 finalColor = mix(blackColor, color, intensity);

    // Output the final color, keeping the original alpha from fo.flick.color
    outColor = vec4(finalColor, fo.flick.color.a);
}