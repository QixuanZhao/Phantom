#version 460 core
in vec4 FragPos;

uniform vec3 light_position;
uniform float far_plane;
uniform bool blending = false;
uniform samplerCube source;
uniform float mixCoefficient = 0.5;

void main()
{
    float depth = distance(FragPos.xyz, light_position) / far_plane;
    if (blending) {
        float refDepth = texture(source, FragPos.xyz - light_position).r;
        gl_FragDepth = mix(refDepth, depth, mixCoefficient);
    } else gl_FragDepth = depth;
}  