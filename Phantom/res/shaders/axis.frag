#version 460 core

out vec4 outputColour;

in vec3 originalColour;
in vec3 fragmentPosition;

void main() {
    vec3 colour = originalColour;
    float len = length(fragmentPosition);

    if ((int(len) / 100) % 2 == 1)
        colour /= 8;
    if ((int(len) / 1000) % 2 == 1)
        colour /= 64;

    // int lenFloor = (int(len) / 100) * 100;
    // if (abs(min(len - lenFloor, 100 - len + lenFloor)) < 0.5)
    //     colour = mix(vec3(1.0), colour, 0.5);
    
    outputColour = vec4(colour, 0.7);
}

