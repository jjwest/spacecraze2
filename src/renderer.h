#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "mesh.h"
#include "shader.h"
#include "platform.h"


struct Renderer
{
    struct RenderData
    {
        Shader* shader;
        Texture texture;
    };

    Shader sprite_shader{"../shaders/sprite.vs", "../shaders/sprite.fs"};

    RenderData renderdata_player{&sprite_shader, Texture("../assets/sprites/playership.png")};
    RenderData renderdata_asteroid{&sprite_shader, Texture("../assets/sprites/meteor.png")};
    RenderData renderdata_blaster{&sprite_shader, Texture("../assets/sprites/blaster.png")};
    RenderData renderdata_drone{&sprite_shader, Texture("../assets/sprites/drone.png")};
    RenderData renderdata_player_laser{&sprite_shader, Texture("../assets/sprites/playerlaser.png")};

    GLuint current_texture = 0;
    GLuint current_shader = 0;
    Mesh sprite_mesh;

    Renderer();
    void DrawBackground();
    void DrawPlayer(const Rectangle& rect, float angle = 0.0);
    void DrawAsteroid(const Rectangle& rect, float angle = 0.0);
    void DrawBlaster(const Rectangle& rect, float angle);
    void DrawDrone(const Rectangle& rect, float angle);
    void DrawPlayerLaser(const Rectangle& rect, float angle = 0.0);
    void BindTexture(GLuint texture);
    void BindShader(const Shader& shader);
    glm::mat4 GenerateModel(const Rectangle& rect, float angle);

private:
    void Draw(const Rectangle& rect, const RenderData& render, float angle);
};

#endif // _RENDERER_H_
