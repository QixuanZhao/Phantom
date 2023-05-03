#version 460 core

out vec3 FragColor;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D image;
uniform bool horizontal;

uniform int boxSize = 11;

void main()
{
    ivec2 FragCoord = ivec2(gl_FragCoord.xy);
    vec3 currentFragmentColour = texelFetch(image, FragCoord, 0).rgb;

    vec3 result = currentFragmentColour; // current fragment's contribution

    for (int i = 1; i <= boxSize / 2; ++i)
    {
        if (horizontal) {
            vec3 left = texelFetch(image, FragCoord + ivec2(i, 0), 0).rgb;
            vec3 right = texelFetch(image, FragCoord - ivec2(i, 0), 0).rgb;
            result += left;
            result += right;
        } else {
            vec3 up = texelFetch(image, FragCoord + ivec2(0, i), 0).rgb;
            vec3 down = texelFetch(image, FragCoord - ivec2(0, i), 0).rgb;
            result += up;
            result += down;
        }
    }

    FragColor = result / boxSize;
}