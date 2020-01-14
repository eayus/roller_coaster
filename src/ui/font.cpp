#include <ui/font.hpp>
#include <cctype>
#include <stb/stb_image.h>
#include <globals.hpp>
#include <iostream>

// Conmstants describing the layout of the letters in the bitmap
constexpr size_t CELL_SIZE = 32;
constexpr size_t GRID_SIZE = 8;

// Location of '0' char
constexpr size_t ZERO_X = 0;
constexpr size_t ZERO_Y = 5;

// Location of 'A' char
constexpr size_t A_X = 1;
constexpr size_t A_Y = 3;


Font::Font(const char* filepath) {
    // Load image
    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath, &width, &height, &num_channels, STBI_rgb_alpha);

    if (!data) std::cerr << "ERRor loading font" << std::endl;

    // Generate texture, and attach image we loaded
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        CELL_SIZE * GRID_SIZE,
        CELL_SIZE * GRID_SIZE,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Font::bind() {
    glBindTexture(GL_TEXTURE_2D, this->texture);
}


std::vector<FontVertex> Font::text_vertices(std::string text, glm::vec2 position) {
    std::vector<FontVertex> result;

    // Iterate through every char in the string
    for (int i = 0; i < text.length(); i++) {
        const auto ci = static_cast<unsigned char>(text[i]);

        // calculate its location within the letter grid
        int x, y;

        if (std::isdigit(ci)) {
            const auto n = ci - static_cast<unsigned char>('0');

            x = (ZERO_X + n) % GRID_SIZE;
            y = ZERO_Y - ((ZERO_X + n) / GRID_SIZE);
        } else if (std::isalpha(ci)) {
            const auto n = ci - static_cast<unsigned char>('a');

            x = (A_X + n) % GRID_SIZE;
            y = A_Y - ((A_X + n) / GRID_SIZE);
        } else {
            x = 0;
            y = 7;
        }

        // Finally calculate the actually vertices, and texture coordinates
        const glm::vec2 pos_offset = position + (static_cast<float>(i) * glm::vec2(CELL_SIZE, 0));
        const auto uv = glm::vec2(x, y);
        const auto uvn = uv / static_cast<float>(GRID_SIZE);
        const auto dxn = glm::vec2(1, 0) / static_cast<float>(GRID_SIZE);
        const auto dyn = glm::vec2(0, 1) / static_cast<float>(GRID_SIZE);

        std::vector<FontVertex> square = {
            { glm::vec2(0, 0) + pos_offset,                 uvn },
            { glm::vec2(CELL_SIZE, 0) + pos_offset,         uvn + dxn},
            { glm::vec2(0, CELL_SIZE) + pos_offset,         uvn + dyn},

            { glm::vec2(CELL_SIZE, 0) + pos_offset,         uvn + dxn },
            { glm::vec2(0, CELL_SIZE) + pos_offset,         uvn + dyn },
            { glm::vec2(CELL_SIZE, CELL_SIZE) + pos_offset, uvn + dxn + dyn },
        };

        result.insert(result.end(), square.begin(), square.end());
    }

    return result;
}
