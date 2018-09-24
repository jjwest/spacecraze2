#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "mesh.h"
#include "shader.h"
#include "common.h"

void BindTexture(GLuint id);
void BindShader(GLuint id);

struct Renderer
{
    Shader sprite_shader{"../shaders/sprite.vert", "../shaders/sprite.frag"};
    GLuint current_texture = 0;
    GLuint current_shader = 0;
    Mesh sprite_mesh;
    glm::mat4 projection;


    Renderer();
    void DrawRect(const Rectangle& rect, const Texture& texture, float angle = 0.0f);
    void DrawBackground(const Texture& texture);
};

#endif // _RENDERER_H_
