#version 460 core

#define MAX_POINT_LIGHT_COUNT 16
#define MAX_SPOTLIGHT_COUNT 16
#define MAX_PARALLEL_LIGHT_COUNT 16

const float PI = radians(180.0);

struct ParallelLight { 
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 diffuse;
    vec3 specular;
};

struct Spotlight {
    vec3 position;
    vec3 direction;
    
    float constant;
    float linear;
    float quadratic;

    float outerCutOff;
    float innerCutOff;
    
    vec3 diffuse;
    vec3 specular;
};

vec3 computeParallelLight (
    in ParallelLight light, 
    in vec3 normal, 
    in vec3 viewDirection
);

vec3 computePointLight(
    in PointLight light,
    in vec3 normal,
    in vec3 fragmentPosition,
    in vec3 viewDirection
);

vec3 computeSpotlight(
    in Spotlight light, 
    in vec3 normal, 
    in vec3 fragmentPosition, 
    in vec3 viewDirection
);

layout (location = 0) out vec4 outputColour;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D input_position;
uniform sampler2D input_normal;
uniform sampler2D input_albedo;
uniform sampler2D input_depth;

uniform bool ssaoEnabled = true;

uniform sampler2D ambientOcclusionMap;

float shininess = 32.0;

uniform vec3 ambientLight;

uniform int parallelLightCount = 0;
uniform ParallelLight parallelLights[MAX_PARALLEL_LIGHT_COUNT];
uniform sampler2D parallelLightShadowMaps[MAX_PARALLEL_LIGHT_COUNT];

uniform int pointLightCount = 0;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform sampler2D pointLightShadowMaps[MAX_POINT_LIGHT_COUNT];

uniform int spotlightCount = 0;
uniform Spotlight spotlights[MAX_SPOTLIGHT_COUNT];
uniform sampler2D spotlightShadowMaps[MAX_SPOTLIGHT_COUNT];

uniform Spotlight cameraFlash;
uniform bool cameraFlashOn;
uniform vec3 cameraPosition;

uniform bool shadowEnabled = true;
uniform bool reflectingAtBothSide;
uniform bool blinnPhong = false;

void main() {
    ivec2 FragCoord = ivec2(gl_FragCoord.xy);

    vec3 fragmentPosition = texelFetch(input_position, FragCoord, 0).rgb;
    vec3 fragmentNormal = texelFetch(input_normal, FragCoord, 0).rgb;
    vec4 albedo = texelFetch(input_albedo, FragCoord, 0);
    float depth = texelFetch(input_depth, FragCoord, 0).r;
    float occlusion = texelFetch(ambientOcclusionMap, FragCoord, 0).r;
    
    shininess = albedo.a;

    if (depth == 1.0) {
        outputColour = vec4(albedo.rgb, 1.0);
        return;
    }

    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    vec3 normal = normalize(fragmentNormal);
    if (reflectingAtBothSide 
        && dot(viewDirection, normal) < 0) 
    {   normal = -normal; }

    vec3 outputLight = vec3(0.0);
    
    int pc = min(parallelLightCount, MAX_PARALLEL_LIGHT_COUNT);
    for (int i = 0; i < pc; i++) {
        float shadow = shadowEnabled ? texelFetch(parallelLightShadowMaps[i], FragCoord, 0).r : 0.0;
        outputLight += (1.0 - shadow) * computeParallelLight(parallelLights[i], normal, viewDirection);
    }
    
    pc = min(pointLightCount, MAX_POINT_LIGHT_COUNT);
    for (int i = 0; i < pc; i++) {
        float shadow = shadowEnabled ? texelFetch(pointLightShadowMaps[i], FragCoord, 0).r : 0.0;
        outputLight += (1.0 - shadow) * computePointLight(pointLights[i], normal, fragmentPosition, viewDirection);
    }

    pc = min(spotlightCount, MAX_SPOTLIGHT_COUNT);
    for (int i = 0; i < pc; i++) {
        float shadow = shadowEnabled ? texelFetch(spotlightShadowMaps[i], FragCoord, 0).r : 0.0;
        outputLight += (1.0 - shadow) * computeSpotlight(spotlights[i], normal, fragmentPosition, viewDirection);
    }

    if (cameraFlashOn) {
        outputLight += computeSpotlight(
            cameraFlash, normal, 
            fragmentPosition, viewDirection
        );
    }

    if (ssaoEnabled) outputLight += ambientLight * occlusion;
    else outputLight += ambientLight;

    vec3 processedColour = albedo.rgb;

    vec3 result = outputLight * processedColour;
    
    outputColour = vec4(result, 1.0);
}

vec3 computeParallelLight(
    in ParallelLight light, 
    in vec3 normal, 
    in vec3 viewDirection
) {
    vec3 diffuse, specular;

    vec3 lightDirection = normalize(-light.direction);
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);

    float specularFactor;
    if (blinnPhong) {
        vec3 halfwayDirection = normalize(viewDirection + lightDirection);
        specularFactor = 
            pow(max(dot(normal, halfwayDirection), 0.0), shininess);
    } else {
        vec3 reflectDirection = reflect(-lightDirection, normal);
        specularFactor = 
            pow(max(dot(reflectDirection, viewDirection), 0.0), shininess);
    }
    
    diffuse = light.diffuse * diffuseFactor;
    specular = light.specular * specularFactor;
    
    return (diffuse + specular);
}

vec3 computePointLight(
    in PointLight light,
    in vec3 normal,
    in vec3 fragmentPosition,
    in vec3 viewDirection
) {
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    float diffuseFactor;
    float specularFactor;

    vec3 lightOffset = light.position - fragmentPosition;
    vec3 lightDirection = normalize(lightOffset);

    float lightDistance = length(lightOffset);
    float attenuation = 
        1.0 / (light.constant + light.linear * lightDistance + 
        light.quadratic * lightDistance * lightDistance);

    diffuseFactor = max(dot(normal, lightDirection), 0.0) * attenuation;
    diffuse = light.diffuse * diffuseFactor;

    if (blinnPhong) {
        vec3 halfwayDirection = normalize(viewDirection + lightDirection);
        specularFactor = 
            pow(max(dot(normal, halfwayDirection), 0.0), shininess) * attenuation;
    } else {
        vec3 reflectDirection = reflect(-lightDirection, normal);
        specularFactor = 
            pow(max(dot(reflectDirection, viewDirection), 0.0), shininess) * attenuation;
    }
    specular = light.specular * specularFactor;
    
    return (diffuse + specular);
}

vec3 computeSpotlight(
    Spotlight light, 
    vec3 normal, 
    vec3 fragmentPosition, 
    vec3 viewDirection
) {
    float diffuseFactor, specularFactor;
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 lightOffset = light.position - fragmentPosition;
    vec3 lightDirection = normalize(lightOffset);

    float cosineTheta = dot(lightDirection, normalize(-light.direction));
    if (cosineTheta > cos(light.outerCutOff)) {
        float lightDistance = length(lightOffset);
        float attenuation = 
            1 / (light.constant + light.linear * lightDistance + 
            light.quadratic * lightDistance * lightDistance);

        diffuseFactor = max(dot(normal, lightDirection), 0.0) * attenuation;
        diffuse = light.diffuse * diffuseFactor;


        if (blinnPhong) {
            vec3 halfwayDirection = normalize(viewDirection + lightDirection);
            specularFactor = 
                pow(max(dot(normal, halfwayDirection), 0.0), shininess) * attenuation;
        } else {
            vec3 reflectDirection = reflect(-lightDirection, normal);
            specularFactor = 
                pow(max(dot(reflectDirection, viewDirection), 0.0), shininess) * attenuation;
        }

        specular = light.specular * specularFactor;
        if (cosineTheta < cos(light.innerCutOff)) {
            float epsilon = cos(light.innerCutOff) - cos(light.outerCutOff);
            float intensity = clamp((cosineTheta - cos(light.outerCutOff)) / epsilon, 
                0.0, 1.0);
            
            diffuse *= intensity;
            specular *= intensity;
        }
    }

    return (diffuse + specular);
}
