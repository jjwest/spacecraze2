#ifndef _MESH_H_
#define _MESH_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

struct Texture
{
    unsigned int id;
    unsigned int width;
    unsigned int height;

    Texture(const std::string& path);
    ~Texture();
};

struct Vertex
{
    glm::vec2 position;
    glm::vec2 texture_mapping;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();
};

#endif // _MESH_H_
