#include "renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <limits>

static GLuint current_texture = std::numeric_limits<GLuint>::max();
static GLuint current_shader = std::numeric_limits<GLuint>::max();

void bind_texture(GLuint id) {
    if (id != current_texture) {
        glBindTexture(GL_TEXTURE_2D, id);
        current_texture = id;
    }
}

void bind_shader(GLuint id) {
    if (id != current_shader) {
        glUseProgram(id);
        current_shader = id;
    }
}

Renderer::Renderer() {
    bind_shader(sprite_shader.id);

    projection = glm::ortho(0.0f,
                            static_cast<float>(g_screen_width),
                            static_cast<float>(g_screen_height),
                            0.0f,
                            -1.0f,
                            1.0f);

    sprite_shader.set_mat4("projection", projection);
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

void Renderer::draw_background(const Texture &texture) {
    glClear(GL_COLOR_BUFFER_BIT);
    bind_texture(texture.id);
    bind_shader(sprite_shader.id);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(g_screen_width, g_screen_height, 0));

    sprite_shader.set_mat4("model", model);

    glBindVertexArray(sprite_mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::draw_rect(const Rectangle &rect, const Texture &texture,
                         float angle, Shader *shader) {
    if (!shader) {
        shader = &sprite_shader;
    }

    bind_shader(shader->id);
    bind_texture(texture.id);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(rect.x, rect.y, 0));
    model = glm::translate(model, glm::vec3(0.5 * rect.width, 0.5 * rect.height, 0.0));
    model = glm::rotate(model, glm::radians(angle + 90.0f), glm::vec3(0.0, 0.0, 1.0));
    model = glm::translate(model, glm::vec3(-0.5 * rect.width, -0.5 * rect.height, 0.0));
    model = glm::scale(model, glm::vec3(rect.width, rect.height, 0.0));

    shader->set_mat4("model", model);

    glBindVertexArray(sprite_mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
