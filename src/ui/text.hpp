#pragma once

#include <GL/glew.h>
#include <string>
#include <ui/font.hpp>
#include <glm/glm.hpp>

// Class repersenting a text string, rendered with a given font

class Text {
public:
    Text(std::string s, Font& font, glm::vec2 position);
    void draw();

private:
    Font& font;

    GLuint vao;
    GLuint vertex_buffer;
    GLsizei num_vertices;
};
