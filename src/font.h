#ifndef _FONT_H_
#define _FONT_H_

#include "common.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

#define COLOR_GREEN glm::vec3(0.7, 0.7, 0.3)

struct FontAnimation {
    u32 duration_ms;
    u32 time_started_ms;
    glm::vec3 color;
};


void init_fonts();
void draw_text(std::string_view text, float x, float y, float scale, glm::vec3 color);
void draw_animated_text(std::string_view text,
                      float x,
                      float y,
                      float scale,
                      glm::vec3 color,
                      const FontAnimation &animation);

#endif // _FONT_H_
