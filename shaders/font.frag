#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;
uniform bool doAnimation;
uniform float animationDuration;
uniform float animationTimeElapsed;
uniform vec3 animationColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    vec4 normalColor = sampled * vec4(textColor, 1.0);

    if (doAnimation)
    {
        vec4 animatedSampled = vec4(animationColor, texture(text, TexCoords).r);
        float intensity = (animationDuration - animationTimeElapsed) / animationTimeElapsed;
        FragColor = mix(normalColor, animatedSampled, intensity);
    }
    else
    {
        FragColor = normalColor;
    }
}
