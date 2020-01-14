#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

// A vertex used in the font shader. We don't need any normal information
// since fonts are always directly on the screen.
struct FontVertex {
    glm::vec2 position;
    glm::vec2 tex_coord;
};

// Class rperesenting a font. We use the bitmap texture font method, where
// a font is a texture with all the characters in, and to create some text
// on the screen we just bind the texture and use the appropritte texture
// coords for each letter

class Font {
public:
    Font(const char* filepath);
    void bind();

    // Calculate the vertices for a string at a given position
    std::vector<FontVertex> text_vertices(std::string string, glm::vec2 position);


private:
    GLuint texture;
};
