#include "renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Renderer::Renderer()
{
    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT),
        0.0f,
        -1.0f,
        1.0f);

    BindShader(sprite_shader);
    sprite_shader.SetMat4("projection", projection);

    sprite_mesh.vertices = {
        // Position    // Tex coords
        {{0.0f, 0.0f}, {0.0f, 1.0f}}, // top left
        {{0.0f, 1.0f}, {0.0f, 0.0f}}, // bottom left
        {{1.0f, 0.0f}, {1.0f, 1.0f}}, // top right
        {{1.0f, 1.0f}, {1.0f, 0.0f}}  // bottom right
    };

    sprite_mesh.indices = {
        0, 1, 2,
        1, 2, 3
    };

    sprite_mesh.build();
}


void Renderer::DrawBackground()
{
    glClear(GL_COLOR_BUFFER_BIT);

    BindTexture(background.texture.id);
    BindShader(*background.shader);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::scale(model, glm::vec3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0));

    background.shader->SetMat4("model", model);

    glBindVertexArray(sprite_mesh.VAO);
    glDrawElements(GL_TRIANGLES, sprite_mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPlayer(const Rectangle& rect, float angle)
{
    Draw(rect, player, angle);
}

void Renderer::DrawAsteroid(const Rectangle& rect, float angle)
{
    Draw(rect, asteroid, angle);
}

void Renderer::DrawPlayerLaser(const Rectangle& rect, float angle)
{
    Draw(rect, player_laser, angle);
}

void Renderer::DrawEnemyLaser(const Rectangle& rect, float angle)
{
    Draw(rect, enemy_laser, angle);
}

void Renderer::DrawBlaster(const Rectangle& rect, float angle)
{
    Draw(rect, blaster, angle);
}

void Renderer::DrawDrone(const Rectangle& rect, float angle)
{
    Draw(rect, drone, angle);
}

void Renderer::Draw(const Rectangle& rect, const RenderData& target, float angle)
{
    BindTexture(target.texture.id);
    BindShader(*target.shader);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(rect.x, rect.y, 0));
    model = glm::translate(model, glm::vec3(0.5 * rect.width, 0.5 * rect.height, 0.0));
    model = glm::rotate(model, glm::radians(angle + 90.0f), glm::vec3(0.0, 0.0, 1.0));
    model = glm::translate(model, glm::vec3(-0.5 * rect.width, -0.5 * rect.height, 0.0));
    model = glm::scale(model, glm::vec3(rect.width, rect.height, 0.0));

    target.shader->SetMat4("model", model);

    glBindVertexArray(sprite_mesh.VAO);
    glDrawElements(GL_TRIANGLES, sprite_mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::BindTexture(GLuint texture)
{
    if (texture != current_texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        current_texture = texture;
    }
}

void Renderer::BindShader(const Shader& shader)
{
    if (shader.id != current_shader)
    {
        shader.Use();
        current_shader = shader.id;
    }
}
