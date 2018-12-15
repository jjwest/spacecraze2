#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform vec2 world_dimensions;

void main()
{
    gl_Position = projection * model * vec4(aPosition, 0.0, 1.0);
    TexCoord = aTexCoord;
}
