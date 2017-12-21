#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "mesh.h"
#include "shader.h"

struct RenderData
{
    Shader shader;
    Texture texture;
};

struct Renderer
{
    RenderData player{Shader("../shaders/player.vs", "../shaders/player.fs"), Texture("../assets/sprites/playership.png")};
    void draw_background();
    void draw_player(const Mesh& mesh);
};

#endif // _RENDERER_H_
