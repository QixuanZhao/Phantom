#version 460 core

out vec2 FragColor;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D image;

void main()
{
    ivec2 FragCoord = ivec2(gl_FragCoord.xy);
    FragColor = texelFetch(image, FragCoord, 0).rg;
}