#include "font.h"

#include "shader.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include <unordered_map>

struct Character
{
    GLuint texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};

std::unordered_map<GLchar, Character> characters;
GLuint VAO;
GLuint VBO;

Shader* font_shader;

void LoadFont(std::string_view filename, int size, FT_Library library)
{
    FT_Face face;
    if (FT_New_Face(library, filename.data(), 0, &face))
    {
        Error("Unable to load font '%s'", filename);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Error("FONTS::Failed to load Glyph for character '%c'\n", c);
            continue;
        }

        FT_GlyphSlot glyph = face->glyph;

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     glyph->bitmap.width,
                     glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(glyph->bitmap.width, glyph->bitmap.rows),
            glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
            static_cast<GLuint>(glyph->advance.x)
        };

        characters.insert({c, character});
    }

    FT_Done_Face(face);
}


void InitFonts()
{
    FT_Library library;
    if (FT_Init_FreeType(&library))
    {
        FatalError("Failed to initialize FreeType library");
    }

    LoadFont("../fonts/Roboto-Medium.ttf", 36, library);
    FT_Done_FreeType(library);



    { // Setup shader
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        font_shader = new Shader("../shaders/font.vert", "../shaders/font.frag");
        glm::mat4 projection = glm::ortho(0.0f,
        static_cast<float>(g_screen_width),
        static_cast<float>(g_screen_height),
        0.0f);

        BindShader(font_shader->id);
        font_shader->SetMat4("projection", projection);
        font_shader->SetBool("doAnimation", false);
        font_shader->SetInt("animationDuration", 1000);
        font_shader->SetInt("animationTimeElapsed", 0);
        font_shader->SetVec3("animationColor", COLOR_GREEN);
    }
}


void DrawText(std::string_view text, float x, float y, float scale, glm::vec3 color)
{
    BindShader(font_shader->id);
    font_shader->SetVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (char c : text)
    {
        auto it = characters.find(c);
        if (it == end(characters))
        {
            Error("Could not find character '%c' from the font", c);
            continue;
        }

        Character ch = it->second;

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y + (ch.size.y - ch.bearing.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        GLfloat vertices[6][4] = {
            { xpos,     ypos - h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos - h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos - h,   1.0, 0.0 }
        };

        BindTexture(ch.texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    BindTexture(0);
}

void DrawAnimatedText(std::string_view text,
                      float x,
                      float y,
                      float scale,
                      glm::vec3 color,
                      const FontAnimation animation)
{
    float time_elapsed = SDL_GetTicks() - animation.time_started_ms;
    if (time_elapsed <= animation.duration_ms)
    {
        BindShader(font_shader->id);
        font_shader->SetFloat("animationTimeElapsed", (float)time_elapsed);
        font_shader->SetFloat("animationDuration", (float)animation.duration_ms);
        font_shader->SetVec3("animationColor", animation.color);
        font_shader->SetBool("doAnimation", true);
        DrawText(text, x, y, scale, color);
        font_shader->SetBool("doAnimation", false);
    }
    else
    {
        DrawText(text, x, y, scale, color);
    }
}
