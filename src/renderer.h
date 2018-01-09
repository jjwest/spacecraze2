#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "constants.h"
#include "mesh.h"
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct RenderData
{
    Shader shader;
    Texture texture;
};

struct Rectangle
{
    float x;
    float y;
    float width;
    float height;
};


struct Renderer
{
    RenderData renderdata_player{
        Shader("../shaders/player.vs", "../shaders/player.fs"),
        Texture("../assets/sprites/playership.png")
    };

    RenderData renderdata_asteroid{
        Shader("../shaders/player.vs", "../shaders/player.fs"),
        Texture("../assets/sprites/meteor.png")
    };

    GLuint current_texture = 0;
    GLuint current_shader = 0;
    Mesh sprite_mesh;

    Renderer();

    void drawBackground();
    void drawPlayer(const Rectangle& rect, float angle = 0.0);
    void drawAsteroid(const Rectangle& rect, float angle = 0.0);
    void bindTexture(GLuint texture);
    void bindShader(GLuint shader);
    glm::mat4 generateModel(const Rectangle& rect, float angle);
};

#endif // _RENDERER_H_
