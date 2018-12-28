#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "common.h"

struct Shader {
    GLuint id;

    Shader(const GLchar *vertex_path, const GLchar *fragment_path) {
        char *vertex_code = read_file(vertex_path);
        if (!vertex_code) {
            error("Could not open file '%s'", vertex_path);
            return;
        }

        char *fragment_code = read_file(fragment_path);
        if (!fragment_code) {
            error("Could not open file '%s'", fragment_path);
            free(vertex_code);
            return;
        }

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertex_code, NULL);
        glCompileShader(vertex);

        check_compile_errors(vertex, "VERTEX");

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragment_code, NULL);
        glCompileShader(fragment);

        check_compile_errors(fragment, "FRAGMENT");

        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);

        check_compile_errors(id, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        free(vertex_code);
        free(fragment_code);
    }

    void set_bool(const char *name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name), static_cast<int>(value));
    }

    void set_int(const char *name, int value) const {
        glUniform1i(glGetUniformLocation(id, name), value);
    }

    void set_float(const char *name, float value) const {
        glUniform1f(glGetUniformLocation(id, name), value);
    }

    void set_vec2(const char *name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(id, name), 1, &value[0]);
    }

    void set_vec2(const char *name, float x, float y) const {
        glUniform2f(glGetUniformLocation(id, name), x, y);
    }

    void set_vec3(const char *name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(id, name), 1, &value[0]);
    }

    void set_vec3(const char *name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(id, name), x, y, z);
    }

    void set_vec4(const char *name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(id, name), 1, &value[0]);
    }

    void set_vec4(const char *name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
    }

    void set_mat2(const char *name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(id, name), 1, GL_FALSE, &mat[0][0]);
    }

    void set_mat3(const char *name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE,
                           &mat[0][0]);
    }

    void set_mat4(const char *name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, &mat[0][0]);
    }

private:
    char *read_file(const char *filename) {
        FILE *file = fopen(filename, "rb");
        if (!file) {
            return nullptr;
        }

        fseek(file, 0, SEEK_END);
        size_t len = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *content = (char *)malloc(len + 1);

        if (fread(content, len, 1, file) != 1) {
            fclose(file);
            free(content);
            return nullptr;
        }

        content[len] = 0;

        return content;
    }


    void check_compile_errors(GLuint shader, std::string_view type) {
        GLint success;
        GLchar info_log[1024];

        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
                error("SHADER::%s::COMPILATION_FAILED\n%s\n", type, info_log);
            }
        } else {
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
                error("SHADER::%s::LINKING_FAILED\n%s\n", type, info_log);
            }
        }
    }
};

#endif // _SHADER_H_
