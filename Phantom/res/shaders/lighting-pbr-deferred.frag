#version 460 core

#define MAX_POINT_LIGHT_COUNT 16
#define MAX_SPOTLIGHT_COUNT 16
#define MAX_PARALLEL_LIGHT_COUNT 16

const float PI = radians(180.0);

struct ParallelLight { 
    vec3 direction;
    vec3 flux;
};

struct PointLight {
    vec3 position;

    vec3 flux;
};

struct Spotlight {
    vec3 position;
    vec3 direction;

    float outerCutOff;
    float innerCutOff;

    vec3 flux;
};

float computeAttenuation(vec3 fragPos, vec3 lightPos);
vec3 FresnelSchlick (float cosine, vec3 R0);

vec3 computeParallelLight (
    in ParallelLight light, 
    in vec3 N, 
    in vec3 V,
    vec3 albedo
);

vec3 computePointLight(
    in PointLight light,
    in vec3 N,
    in vec3 fragmentPosition,
    in vec3 V,
    vec3 albedo
);

vec3 computeSpotlight(
    in Spotlight light, 
    in vec3 N, 
    in vec3 fragmentPosition, 
    in vec3 V,
    vec3 albedo
);

layout (location = 0) out vec4 outputColour;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D input_position;
uniform sampler2D input_normal;
uniform sampler2D input_albedo;
uniform sampler2D input_depth;
uniform sampler2D input_metallic_roughness;
uniform sampler2D input_distinction_coefficient;

uniform bool ssaoEnabled = true;

uniform sampler2D ambientOcclusionMap;

float metallic = 0.7;
float roughness = 0.6;

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

vec3 R0 = vec3(0.04);
vec3 R_IT;
vec3 K;

void main() {
    ivec2 FragCoord = ivec2(gl_FragCoord.xy);

    float depth = texelFetch(input_depth, FragCoord, 0).r;
    vec3 albedo = texelFetch(input_albedo, FragCoord, 0).rgb;
    if (depth == 1.0) {
        outputColour = vec4(albedo, 1.0);
        return;
    }

    vec3 fragmentPosition = texelFetch(input_position, FragCoord, 0).rgb;
    vec3 fragmentNormal = texelFetch(input_normal, FragCoord, 0).rgb;
    float occlusion = texelFetch(ambientOcclusionMap, FragCoord, 0).r;

    vec2 mrk = texelFetch(input_metallic_roughness, FragCoord, 0).rg;
    metallic = mrk.x * (1.0 - 1e-4);
    roughness = mrk.y;

    vec3 dc = texelFetch(input_distinction_coefficient, FragCoord, 0).rgb;

    vec3 V = normalize(cameraPosition - fragmentPosition);
    vec3 N = normalize(fragmentNormal);
    if (reflectingAtBothSide 
        && dot(V, N) < 0) 
    {   N = -N; }

    R0 = mix(R0, albedo, metallic);
    vec3 R0_sqrt = sqrt(R0);
    R_IT = (1.0 - R0_sqrt) / (1.0 + R0_sqrt);
    // K = R0 * (mrk.z);
    K = dc;

    vec3 outputLight = vec3(0.0);

    int pc = min(parallelLightCount, MAX_PARALLEL_LIGHT_COUNT);
    for (int i = 0; i < pc; i++) {
        float shadow = shadowEnabled ? texelFetch(parallelLightShadowMaps[i], FragCoord, 0).r : 0.0;
        outputLight += (1.0 - shadow) * computeParallelLight(parallelLights[i], N, V, albedo);
    }

    pc = min(pointLightCount, MAX_POINT_LIGHT_COUNT);
    for (int i = 0; i < pc; i++) {
        float shadow = shadowEnabled ? texelFetch(pointLightShadowMaps[i], FragCoord, 0).r : 0.0;
        outputLight += (1.0 - shadow) * computePointLight(pointLights[i], N, fragmentPosition, V, albedo);
    }

    pc = min(spotlightCount, MAX_SPOTLIGHT_COUNT);
    for (int i = 0; i < pc; i++) {
        float shadow = shadowEnabled ? texelFetch(spotlightShadowMaps[i], FragCoord, 0).r : 0.0;
        outputLight += (1.0 - shadow) * computeSpotlight(spotlights[i], N, fragmentPosition, V, albedo);
    }

    if (cameraFlashOn) {
        outputLight += computeSpotlight(
            cameraFlash, N, 
            fragmentPosition, V,
            albedo
        );
    }

    if (ssaoEnabled) outputLight += ambientLight * albedo * occlusion;
    else outputLight += ambientLight * albedo;
    
    outputColour = vec4(outputLight, 1.0);
}

float computeAttenuation(vec3 fragPos, vec3 lightPos) {
    float dist =  distance(fragPos, lightPos);
    return 1.0 / (dist * dist);
}

vec3 Fresnel (float cosine, vec3 R_IT, vec3 K) {
    /// RGB : 700, 546.1, 435.8 (nm)
    float cosineI = cosine;
    float sineI = sqrt(1.0 - cosineI * cosineI);
    vec3 sineT = R_IT * sineI;
    vec3 determinator = clamp(1.0 - sineT * sineT, 0.0, 1.0);
    vec3 cosineT = sqrt(determinator);
    
    vec3 RT = 1.0 / R_IT;

    vec3 A = cosineI - RT * cosineT;
    vec3 B = K * cosineT;
    vec3 C = cosineI + RT * cosineT;
    vec3 D = -B;
 
    vec3 CD = C * C + D * D;
    vec3 R = (A * C + B * D) / CD;
    vec3 I = (B * C - A * D) / CD;
 
    vec3 Rs = R * R + I * I;
 
 
    A = cosineT - RT * cosineI;
    B = K * cosineI;
    C = cosineT + RT * cosineI;
    D = -B;

    CD = C * C + D * D;
    R = (A * C + B * D) / CD;
    I = (B * C - A * D) / CD;

    vec3 Rp = R * R + I * I;

    return (Rs + Rp) / 2.0;
}

vec3 FresnelSchlick (float cosine, vec3 R0) {
    return R0 + (1.0 - R0) * pow(clamp(1.0 - cosine, 0.0, 1.0), 5.0);
}

// vec3 FresnelLazaniy(float cosine, vec3 R0) {

// }

float NormalDistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NH = max(dot(N, H), 0.0);
    float NH2 = NH * NH;

    float num = a2;
    float denom = (NH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    float num = NV;
    float denom = NV * (1.0 - k) + k;
    
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NV = max(dot(N, V), 0.0);
    float NL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NV, roughness);
    float ggx1 = GeometrySchlickGGX(NL, roughness);

    return ggx2 * ggx1;
}

vec3 computeParallelLight(
    in ParallelLight light, 
    in vec3 N, 
    in vec3 V,
    vec3 albedo
) {
    vec3 L = normalize(-light.direction);
    vec3 radiance = light.flux;

    vec3 H = normalize(L + V);

    float NL = max(dot(N, L), 0.0);
    float HV = max(dot(H, V), 0.0);
    // vec3 F = FresnelSchlick(HV, R0);
    vec3 F = Fresnel(HV, R_IT, K);
    float NDF = NormalDistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);


    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * NL * max(dot(N, V), 0.0) + 1e-4;

    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = 1.0 - kS;

    kD *= 1.0 - metallic;

    return (kD * albedo / PI + specular) * radiance * NL;
}

vec3 computePointLight(
    in PointLight light,
    in vec3 N,
    in vec3 fragmentPosition,
    in vec3 V,
    vec3 albedo
) {
    vec3 lightOffset = light.position - fragmentPosition;
    vec3 L = normalize(lightOffset);

    float lightDistance = length(lightOffset);
    float attenuation = 1.0 / (lightDistance * lightDistance);
    vec3 radiance = light.flux * attenuation;

    vec3 H = normalize(L + V);

    float NL = max(dot(N, L), 0.0);
    float HV = max(dot(H, V), 0.0);
    // vec3 F = FresnelSchlick(max(dot(H, V), 0.0), R0);
    vec3 F = Fresnel(HV, R_IT, K);
    float NDF = NormalDistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * NL * max(dot(N, V), 0.0) + 1e-4;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = 1.0 - kS;

    kD *= 1.0 - metallic;
    return (kD * albedo / PI + specular) * radiance * NL;
}

vec3 computeSpotlight(
    Spotlight light, 
    vec3 N, 
    vec3 fragmentPosition, 
    vec3 V,
    vec3 albedo
) {
    vec3 outputLight = vec3(0.0);
    vec3 lightOffset = light.position - fragmentPosition;
    vec3 L = normalize(lightOffset);

    float cosineTheta = dot(L, normalize(-light.direction));
    if (cosineTheta > cos(light.outerCutOff)) {
        float lightDistance = length(lightOffset);
        float attenuation = 1.0 / (lightDistance * lightDistance);
        vec3 radiance = light.flux * attenuation;

        vec3 H = normalize(L + V);

        float NL = max(dot(N, L), 0.0);
        float HV = max(dot(H, V), 0.0);

        // vec3 F = FresnelSchlick(max(dot(H, V), 0.0), R0);
        vec3 F = Fresnel(HV, R_IT, K);
        float NDF = NormalDistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * NL * max(dot(N, V), 0.0) + 1e-4;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = 1.0 - kS;
        kD *= 1.0 - metallic;

        outputLight = (kD * albedo / PI + specular) * radiance * NL;

        if (cosineTheta < cos(light.innerCutOff)) {
            float epsilon = cos(light.innerCutOff) - cos(light.outerCutOff);
            float intensity = clamp((cosineTheta - cos(light.outerCutOff)) / epsilon, 
                0.0, 1.0);
            
            outputLight *= intensity;
        }
    }

    return outputLight;
}
