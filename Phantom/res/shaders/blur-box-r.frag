#version 460 core

out float FragColor;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D image;
uniform bool horizontal;

uniform int boxSize = 11;

void main()
{
    ivec2 FragCoord = ivec2(gl_FragCoord.xy);
    float currentFragmentColour = texelFetch(image, FragCoord, 0).r;

    float result = currentFragmentColour; // current fragment's contribution

    for (int i = 1; i <= boxSize / 2; ++i)
    {
        if (horizontal) {
            float left = texelFetch(image, FragCoord + ivec2(i, 0), 0).r;
            float right = texelFetch(image, FragCoord - ivec2(i, 0), 0).r;
            result += left;
            result += right;
        } else {
            float up = texelFetch(image, FragCoord + ivec2(0, i), 0).r;
            float down = texelFetch(image, FragCoord - ivec2(0, i), 0).r;
            result += up;
            result += down;
        }
    }

    FragColor = result / boxSize;
}