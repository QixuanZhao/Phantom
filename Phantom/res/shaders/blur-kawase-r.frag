#version 460 core

out float FragColor;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D image;
uniform int space = 0;

void main()
{
    ivec2 size = textureSize(image, 0);
    vec2 unit = 1.0 / size;
    vec2 curCoord = gl_FragCoord.xy / size;
    ivec2 FragCoord = ivec2(gl_FragCoord.xy);


    if (space == 0) {
        float A = texelFetch(image, FragCoord + ivec2(-1, -1), 0).r;
        float B = texelFetch(image, FragCoord + ivec2( 0, -1), 0).r;
        float C = texelFetch(image, FragCoord + ivec2( 1, -1), 0).r;
        float D = texelFetch(image, FragCoord + ivec2(-1,  0), 0).r;
        float E = texelFetch(image, FragCoord + ivec2( 0,  0), 0).r;
        float F = texelFetch(image, FragCoord + ivec2( 1,  0), 0).r;
        float G = texelFetch(image, FragCoord + ivec2(-1,  1), 0).r;
        float H = texelFetch(image, FragCoord + ivec2( 0,  1), 0).r;
        float I = texelFetch(image, FragCoord + ivec2( 1,  1), 0).r;

        FragColor += A + 2.0 * B + C;
        FragColor += 2.0 * (D + 2.0 * E + F);
        FragColor += G + 2.0 * H + I;
        FragColor /= 16.0;
    } else {
        vec4 AR = textureGather(image, curCoord + vec2( 1.0,  1.0) * (0.5 + space) * unit, 0);
        vec4 BR = textureGather(image, curCoord + vec2( 1.0, -1.0) * (0.5 + space) * unit, 0);
        vec4 CR = textureGather(image, curCoord + vec2(-1.0,  1.0) * (0.5 + space) * unit, 0);
        vec4 DR = textureGather(image, curCoord + vec2(-1.0, -1.0) * (0.5 + space) * unit, 0);
        vec4 R = (AR + BR + CR + DR);
        float red = R.x + R.y + R.z + R.w;

        FragColor = red / 16.0;
    }
}