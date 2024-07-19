#define MAXBULLETS 400
#define MAXSIZE 400
#define MAX_PLANE 3
#define MAPDIM 25
#define MAXPARTICLES 20

struct SpotLight{
    vec4 spotlightPosition;            	// postion of the spotlight
    vec4 spotlightDirection;           	// direction of the spotlight/directional light
    vec4 spotlightColor;               	// color of the light
    float spotlightCosIn;
    float spotlightCosOut;
};



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

vec3 spotlightIntensity(vec4 spotlightPosition,
                        vec4 spotlightDirection,
                        vec4 spotlightColor,
                        float spotlightCosIn,
                        float spotlightCosOut,
                        vec4 dirToLight) {

    vec3 dir = dirToLight.xyz;
    vec3 lightDirection = normalize(dir);
    float decay = pow(5 / length(dir), 2);
    vec3 lightColor = spotlightColor.xyz * spotlightColor.w;
    float cosalpha = dot(lightDirection, spotlightDirection.xyz);
    float coneDimming = clamp((cosalpha - spotlightCosOut) / (spotlightCosIn - spotlightCosOut), 0, 1);
    lightColor = lightColor * decay * coneDimming;

    return lightColor;
}

vec3 phongSpecularNonMetals(vec3 objectPos, vec4 lightPos, vec3 cameraPos, vec3 normal, float shineFactor, vec3 reflectionColor)
{
    vec3 dirToLight = normalize(lightPos.xyz - objectPos);
    vec3 cameraDir = normalize(cameraPos - objectPos);

    vec3 reflectionDir = -reflect(dirToLight, normal);
    float reflectionIntensity = pow(clamp(dot(cameraDir, reflectionDir), 0, 1), shineFactor);

    return reflectionColor * reflectionIntensity;
}

vec3 phongSpecularMetals(vec3 objectPos, vec4 lightPos, vec3 cameraPos, vec3 normal, vec3 metalColor, float shineFactor)
{
    vec3 dirToLight = lightPos.xyz - objectPos;
    vec3 cameraDir = cameraPos - objectPos;

    vec3 reflectionDir = -reflect(dirToLight, normal);
    float reflectionIntensity = pow(clamp(dot(cameraDir, reflectionDir), 0, 1), shineFactor);

    return metalColor * reflectionIntensity;
}