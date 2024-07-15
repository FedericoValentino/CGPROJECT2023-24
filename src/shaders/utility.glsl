#define MAXBULLETS 400
#define MAXSIZE 400
#define MAX_PLANE 3
#define MAPDIM 25
#define MAXPARTICLES 20


vec3 pointLightIntensityBlink(float frequencyIn, vec4 position, vec4 color, float time, vec3 pos, vec3 normal)
{
    float frequency = frequencyIn;
    vec3 directionToLight = position.xyz - pos;
    float attenuation = 1.0 / dot(directionToLight, directionToLight);
    float cosAngIncidence = max(dot(normal, normalize(directionToLight)), 0);
    vec3 intensity = color.xyz * color.w * attenuation;
    if(time > 0.3f)
    {
        intensity *= abs(cos(frequency * time));
    }

    return intensity * cosAngIncidence;
}

vec3 pointLightIntensity(float frequencyIn, vec4 position, vec4 color, vec3 pos, vec3 normal)
{
    float frequency = frequencyIn;
    vec3 directionToLight = position.xyz - pos;
    float attenuation = 1.0 / dot(directionToLight, directionToLight);
    float cosAngIncidence = max(dot(normal, normalize(directionToLight)), 0);
    vec3 intensity = color.xyz * color.w * attenuation;


    return intensity * cosAngIncidence;
}