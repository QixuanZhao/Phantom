#version 460 core

out vec2 FragColor;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D image;
uniform bool horizontal;

uniform int boxSize = 11;

void main()
{
    ivec2 FragCoord = ivec2(gl_FragCoord.xy);
    vec2 currentFragmentColour = texelFetch(image, FragCoord, 0).rg;

    vec2 result = currentFragmentColour; // current fragment's contribution

    for (int i = 1; i <= boxSize / 2; ++i)
    {
        if (horizontal) {
            vec2 left = texelFetch(image, FragCoord + ivec2(i, 0), 0).rg;
            vec2 right = texelFetch(image, FragCoord - ivec2(i, 0), 0).rg;
            result += left;
            result += right;
        } else {
            vec2 up = texelFetch(image, FragCoord + ivec2(0, i), 0).rg;
            vec2 down = texelFetch(image, FragCoord - ivec2(0, i), 0).rg;
            result += up;
            result += down;
        }
    }

    FragColor = result / boxSize;
}