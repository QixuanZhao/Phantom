#version 460 core

out vec2 FragColor;

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D image;
uniform bool horizontal;

// 4
// uniform float weight[] = float[] (0.2734375, 0.21875, 0.109375, 0.03125, 0.00390625);
// 5
uniform float weight[] = float[] (0.24609375, 0.205078125, 0.1171875, 0.0439453125, 0.009765625, 0.0009765625);
// uniform float weight[] = float[] (1.0 / 11.0, 1.0 / 11.0, 1.0 / 11.0, 1.0 / 11.0, 1.0 / 11.0, 1.0 / 11.0);
// 6
// uniform float weight[] = float[] (0.2255859375, 0.193359375, 0.120849609375, 0.0537109375, 0.01611328125, 0.0029296875, 0.000244140625);
// 7
// uniform float weight[] = float[] (
//     0.20947265625, 0.18328857421875, 0.1221923828125, 0.06109619140625, 0.022216796875, 0.00555419921875, 0.0008544921875, 6.103515625e-05
// );
// 8
// uniform float weight[] = float[] (
//     0.196380615234375, 0.174560546875, 0.1221923828125, 0.066650390625, 0.02777099609375, 0.008544921875, 0.0018310546875, 0.000244140625, 1.52587890625e-05
// );

// 24
// uniform float weight[] = float[] (
//     0.11456650271348678, 0.10998384260494731, 0.09729339922745339, 0.07927610307422128, 0.05945707730566596, 0.041004880900459284, 0.025969757903624213, 0.015079214266620511, 0.008010832579142146, 0.0038840400383719498, 0.0017135470757523308, 0.0006854188303009323, 0.0002475123553864478, 8.027427742263171e-05, 2.3237290832867075e-05, 5.958279700735147e-06, 1.3406129326654082e-06, 2.6158301125178696e-07, 4.359716854196449e-08, 6.083325843064813e-09, 6.912870276210015e-10, 6.144773578853346e-11, 4.007461029686965e-12, 1.7053025658242404e-13, 3.552713678800501e-15
// );

void main()
{
    ivec2 FragCoord = ivec2(gl_FragCoord.xy);
    vec2 currentFragmentColour = texelFetch(image, FragCoord, 0).rg;

    vec2 result = currentFragmentColour * weight[0]; // current fragment's contribution

    for(int i = 1; i < weight.length(); ++i)
    {
        if (horizontal) {
            vec2 left = texelFetch(image, FragCoord + ivec2(i, 0), 0).rg;
            vec2 right = texelFetch(image, FragCoord - ivec2(i, 0), 0).rg;
            result += left * weight[i];
            result += right * weight[i];
        } else {
            vec2 up = texelFetch(image, FragCoord + ivec2(0, i), 0).rg;
            vec2 down = texelFetch(image, FragCoord - ivec2(0, i), 0).rg;
            result += up * weight[i];
            result += down * weight[i];
        }
    }

    FragColor = result;
}