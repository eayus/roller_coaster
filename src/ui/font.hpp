#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct FontVertex {
    glm::vec2 position;
    glm::vec2 tex_coord;
};

class Font {
public:
    Font(const char* filepath);
    void bind();
    std::vector<FontVertex> text_vertices(std::string string, glm::vec2 position);


private:
    GLuint texture;
};
