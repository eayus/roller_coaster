#include <ui/text.hpp>

Text::Text(std::string s, Font& font, glm::vec2 position)
    : font(font)
{
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vertex_buffer);

    glBindVertexArray(this->vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), (void*)(sizeof(glm::vec2)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Calculate the vertices from the font
    std::vector<FontVertex> vertices = font.text_vertices(
        s,
        position
    );

    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(FontVertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    this->num_vertices = vertices.size();
}


void Text::draw() {
    this->font.bind();
    glBindVertexArray(this->vao);
        glDrawArrays(GL_TRIANGLES, 0, this->num_vertices);
    glBindVertexArray(0);
}
