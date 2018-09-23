#include "opengl.h"

#include <limits>

static GLuint current_texture = std::numeric_limits<GLuint>::max();
static GLuint current_shader = std::numeric_limits<GLuint>::max();

void BindTexture(GLuint id)
{
    if (id != current_texture)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        current_texture = id;
    }
}

void BindShader(GLuint id)
{
    if (id != current_shader)
    {
        glUseProgram(id);
        current_shader = id;
    }
}
