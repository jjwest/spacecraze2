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
    // if (doAnimation)
    // {
    //     float accentuationPoint = animationDuration * 0.10;
    //     float timeRemaining = animationDuration - animationTimeElapsed;
    //     float elapsedDistanceToAccentuation = abs(accentuationPoint - animationTimeElapsed);
    //     float remainingDistanceToAccentuation = abs(accentuationPoint - timeRemaining);

    //     vec4 animatedSampled = vec4(animationColor, texture(text, TexCoords).r);
    //     float shortestDistance = min(elapsedDistanceToAccentuation, remainingDistanceToAccentuation);
    //     float intensity = (shortestDistance * 2) / animationDuration;
    //     FragColor = mix(animatedSampled, normalColor, intensity);
    // }
    else
    {
        FragColor = normalColor;
    }
}
