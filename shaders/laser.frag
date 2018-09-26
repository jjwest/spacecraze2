#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D image;
uniform bool doubleDamage;

void main()
{
    if (doubleDamage)
    {
        vec4 sampled = texture(image, TexCoord);
        FragColor = vec4(1.0, 1.0, 0.0, sampled.a);
    }
    else
    {
        FragColor = texture(image, TexCoord);
    }
}
