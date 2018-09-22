#ifndef _MESH_H_
#define _MESH_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <string>
#include <vector>

#include "common.h"

struct Texture
{
    GLuint id;
    int width;
    int height;

    Texture(const std::string& path);
    ~Texture();

    void bind();
};

struct Vertex
{
    glm::vec2 position;
    glm::vec2 texture_coords;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    u32 VBO{};
    u32 VAO{};

    void build();
    ~Mesh();
};

#endif // _MESH_H_
