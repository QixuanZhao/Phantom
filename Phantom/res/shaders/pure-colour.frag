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

struct SpotLight {
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

vec3 computeSpotLight(
    in SpotLight light, 
    in vec3 normal, 
    in vec3 fragmentPosition, 
    in vec3 viewDirection
);

layout (location = 0) out vec4 outputColour;
// layout (location = 1) out vec4 brightColour;

layout(pixel_center_integer) in vec4 gl_FragCoord;

in vec3 fragmentNormal;
in vec3 fragmentPosition;
in vec2 textureCoordinate;

uniform sampler2D textureBuffer;
uniform bool textureEnabled = false;

uniform vec3 originalColour;
uniform float shininess;

uniform vec3 ambientLight;

uniform PointLight pointLight;
uniform sampler2D pointLightShadowMap;

uniform int parallelLightCount = 0;
uniform ParallelLight parallelLights[MAX_PARALLEL_LIGHT_COUNT];
uniform sampler2D parallelLightShadowMaps[MAX_PARALLEL_LIGHT_COUNT];

uniform SpotLight cameraFlash;
uniform bool cameraFlashOn;
uniform vec3 cameraPosition;

uniform bool shadowEnabled = true;
uniform bool reflectingAtBothSide;
uniform bool blinnPhong = false;

void main() {
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    vec3 normal = normalize(fragmentNormal);
    if (reflectingAtBothSide 
        && dot(viewDirection, normal) < 0) 
    {   normal = -normal; }

    vec3 outputLight = vec3(0.0);

    ivec2 FragCoord = ivec2(gl_FragCoord.xy);
    float pointLightShadow = shadowEnabled ? texelFetch(pointLightShadowMap, FragCoord, 0).r : 0.0;

    outputLight += (1.0 - pointLightShadow) * 
        computePointLight(pointLight, normal, fragmentPosition, viewDirection);

    int pc = min(parallelLightCount, MAX_PARALLEL_LIGHT_COUNT);
    for (int i = 0; i < parallelLightCount; i++) {
        float shadow = shadowEnabled ? texelFetch(parallelLightShadowMaps[i], FragCoord, 0).r : 0.0;
        outputLight += (1.0 - shadow) * computeParallelLight(parallelLights[i], normal, viewDirection);
    }

    if (cameraFlashOn) {
        outputLight += 
            computeSpotLight(
                cameraFlash, normal, 
                fragmentPosition, viewDirection
            );
    }

    outputLight += ambientLight;

    vec3 processedColour = originalColour;
    if (textureEnabled) processedColour = texture(textureBuffer, textureCoordinate).rgb;

    vec3 result = outputLight * processedColour;

    outputColour = vec4(result, 1.0);

    // if (dot(result, vec3(1.0)) > 3.0) {
    //     brightColour = vec4(result, 1.0);
    // } else {
    //     brightColour = vec4(vec3(0.0), 1.0);
    // }
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

vec3 computeSpotLight(
    SpotLight light, 
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
