#include "renderer.h"
#include "constants.h"

#include <glad/glad.h>

Renderer::Renderer()
{
    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT),
        0.0f,
        -1.0f,
        1.0f);

    renderdata_player.shader.use();
    renderdata_player.shader.setMat4("projection", projection);
    renderdata_asteroid.shader.use();
    renderdata_asteroid.shader.setMat4("projection", projection);

    sprite_mesh.vertices = {
        {{0.0f, 1.0f}, {0.0f, 1.0f}}, // top left
        {{0.0f, 0.0f}, {0.0f, 0.0f}}, // bottom left
        {{1.0f, 1.0f}, {1.0f, 1.0f}}, // top right
        {{1.0f, 0.0f}, {1.0f, 0.0f}}  // bottom right
    };

    sprite_mesh.indices = {
        0, 1, 2,
        1, 2, 3
    };

    sprite_mesh.build();
}

void Renderer::drawBackground()
{
    glClearColor(0.3, 0.5, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawPlayer(const Rectangle& rect, float angle)
{
    bindTexture(renderdata_player.texture.id);
    bindShader(renderdata_player.shader.id);
    glBindVertexArray(sprite_mesh.VAO);

    glm::mat4 model = generateModel(rect, angle);
    renderdata_player.shader.setMat4("model", model);

    glDrawElements(GL_TRIANGLES, sprite_mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::drawAsteroid(const Rectangle& rect, float angle)
{
    bindTexture(renderdata_asteroid.texture.id);
    bindShader(renderdata_asteroid.shader.id);
    glBindVertexArray(sprite_mesh.VAO);

    glm::mat4 model = generateModel(rect, angle);
    renderdata_asteroid.shader.setMat4("model", model);

    glDrawElements(GL_TRIANGLES, sprite_mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

glm::mat4 Renderer::generateModel(const Rectangle& rect, float angle)
{
    glm::mat4 model;

    model = glm::translate(model, glm::vec3(rect.x, rect.y, 0));
    model = glm::translate(model, glm::vec3(0.5 * rect.width, 0.5 * rect.height, 0.0));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
    model = glm::translate(model, glm::vec3(-0.5 * rect.width, -0.5 * rect.height, 0.0));
    model = glm::scale(model, glm::vec3(rect.width, rect.height, 0.0));

    return model;
}

void Renderer::bindTexture(GLuint texture)
{
    if (texture != current_texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        current_texture = texture;
    }
}

void Renderer::bindShader(GLuint shader)
{
    if (shader != current_shader)
    {
        glUseProgram(shader);
        current_shader = shader;
    }
}
