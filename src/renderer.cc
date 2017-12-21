#include "renderer.h"

#include <glad/glad.h>

void Renderer::draw_background()
{
    glClearColor(0.3, 0.5, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw_player(const Mesh& mesh)
{

}
