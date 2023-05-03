#version 460 core

uniform bool blending = false;
uniform sampler2D source;
uniform float mixCoefficient = 0.5;

void main() {
    if (blending) {
        float refDepth = texture(source, gl_FragCoord.xy / textureSize(source, 0)).r;
        gl_FragDepth = mix(refDepth, gl_FragCoord.z, mixCoefficient);
    } else gl_FragDepth = gl_FragCoord.z;
}
