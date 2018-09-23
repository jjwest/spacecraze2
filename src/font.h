#ifndef _FONT_H_
#define _FONT_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

void InitFonts();
void DrawText(std::string_view text, float x, float y, float scale, glm::vec3 color);

#endif // _FONT_H_
