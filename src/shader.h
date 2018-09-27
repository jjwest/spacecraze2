#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>

#include "common.h"

struct Shader
{
    GLuint id;

    Shader(const GLchar* vertex_path, const GLchar* fragment_path)
    {
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream vertex_shader_file;
        std::ifstream fragment_shader_file;

        vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vertex_shader_file.open(vertex_path);
            fragment_shader_file.open(fragment_path);
            std::stringstream vert_shader_stream, frag_shader_stream;

            vert_shader_stream << vertex_shader_file.rdbuf();
            frag_shader_stream << fragment_shader_file.rdbuf();

            vertex_shader_file.close();
            fragment_shader_file.close();

            vertex_code = vert_shader_stream.str();
            fragment_code = frag_shader_stream.str();
        }
        catch (std::ifstream::failure e)
        {
            Error("SHADER::FILE_NOT_SUCCESSFULLY_READ");
        }

        const char* vert_shader_code = vertex_code.c_str();
        const char* frag_shader_code = fragment_code.c_str();

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vert_shader_code, NULL);
        glCompileShader(vertex);

        CheckCompileErrors(vertex, "VERTEX");

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &frag_shader_code, NULL);
        glCompileShader(fragment);

        CheckCompileErrors(fragment, "FRAGMENT");

        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);

        CheckCompileErrors(id, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }

    void SetBool(std::string_view name, bool value) const
    {
        glUniform1i(glGetUniformLocation(id, name.data()), static_cast<int>(value));
    }

    void SetInt(std::string_view name, int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.data()), value);
    }

    void SetFloat(std::string_view name, float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.data()), value);
    }

    void SetVec2(std::string_view name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(id, name.data()), 1, &value[0]);
    }

    void SetVec2(std::string_view name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(id, name.data()), x, y);
    }

    void SetVec3(std::string_view name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(id, name.data()), 1, &value[0]);
    }
    void SetVec3(std::string_view name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(id, name.data()), x, y, z);
    }

    void SetVec4(std::string_view name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(id, name.data()), 1, &value[0]);
    }
    void SetVec4(std::string_view name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(id, name.data()), x, y, z, w);
    }

    void SetMat2(std::string_view name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &mat[0][0]);
    }

    void SetMat3(std::string_view name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &mat[0][0]);
    }

    void SetMat4(std::string_view name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void CheckCompileErrors(GLuint shader, std::string_view type)
    {
        GLint success;
        GLchar info_log[1024];

        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
                Error("SHADER::%s::COMPILATION_FAILED\n%s\n", type.data(), info_log);
            }
        }
        else
        {
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
                Error("SHADER::%s::LINKING_FAILED\n%s\n", type.data(), info_log);
            }
        }
    }
};

#endif // _SHADER_H_
