#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "common.h"
#include "mesh.h"
#include "shader.h"

void bind_texture(GLuint id);
void bind_shader(GLuint id);

struct Renderer {
    Shader sprite_shader{"../shaders/sprite.vert", "../shaders/sprite.frag"};
    GLuint current_texture = 0;
    GLuint current_shader = 0;
    Mesh sprite_mesh;
    glm::mat4 projection;

    Renderer();
    void draw_rect(const Rectangle &rect, const Texture &texture,
                   float angle = 0.0f, Shader *shader = nullptr);
    void draw_background(const Texture &texture);
};

#endif // _RENDERER_H_
