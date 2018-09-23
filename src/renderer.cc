#include "renderer.h"
#include "opengl.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Renderer::Renderer()
{
    BindShader(sprite_shader.id);

    projection = glm::ortho(
        0.0f,
        static_cast<float>(g_screen_width),
        static_cast<float>(g_screen_height),
        0.0f,
        -1.0f,
        1.0f);

    sprite_shader.SetMat4("projection", projection);
    sprite_mesh.vertices = {
        // Position    // Tex coords
        {{0.0f, 0.0f}, {0.0f, 1.0f}}, // top left
        {{0.0f, 1.0f}, {0.0f, 0.0f}}, // bottom left
        {{1.0f, 0.0f}, {1.0f, 1.0f}}, // top right
        {{0.0f, 1.0f}, {0.0f, 0.0f}}, // bottom left
        {{1.0f, 0.0f}, {1.0f, 1.0f}}, // top right
        {{1.0f, 1.0f}, {1.0f, 0.0f}}  // bottom right
    };

    sprite_mesh.build();
}


void Renderer::DrawBackground(const Texture& texture)
{
    glClear(GL_COLOR_BUFFER_BIT);
    BindTexture(texture.id);
    BindShader(sprite_shader.id);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(g_screen_width, g_screen_height, 0));

    sprite_shader.SetMat4("model", model);

    glBindVertexArray(sprite_mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawRect(const Rectangle& rect, const Texture& texture, float angle)
{
    BindTexture(texture.id);
    BindShader(sprite_shader.id);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(rect.x, rect.y, 0));
    model = glm::translate(model, glm::vec3(0.5 * rect.width, 0.5 * rect.height, 0.0));
    model = glm::rotate(model, glm::radians(angle + 90.0f), glm::vec3(0.0, 0.0, 1.0));
    model = glm::translate(model, glm::vec3(-0.5 * rect.width, -0.5 * rect.height, 0.0));
    model = glm::scale(model, glm::vec3(rect.width, rect.height, 0.0));

    sprite_shader.SetMat4("model", model);

    glBindVertexArray(sprite_mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
