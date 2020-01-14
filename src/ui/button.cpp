#include <ui/button.hpp>
#include <array>

Button::Button(glm::vec2 center_pos, glm::vec2 size)
    : center_pos(center_pos)
    , size(size)
{
    // Gen VAO, buffers etc
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vertex_buffer);

    glBindVertexArray(this->vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)(sizeof(glm::vec2)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glm::vec2 dx(size.x * 0.5f, 0.0f);
    glm::vec2 dy(0.0f, size.y * 0.5f);

    glm::vec3 color = MUSTARD_YELLOW;
    
    // The rectangle for the button
    std::array<UIVertex, 6> vertices = {
        UIVertex { glm::vec2(center_pos - dx - dy), color }, // bottom_left
        UIVertex { glm::vec2(center_pos + dx + dy), color }, // top_right
        UIVertex { glm::vec2(center_pos - dx + dy), color }, // top_left

        UIVertex { glm::vec2(center_pos + dx + dy), color }, // top_right
        UIVertex { glm::vec2(center_pos - dx - dy), color }, // bottom_left
        UIVertex { glm::vec2(center_pos + dx - dy), color }, // bottom_right
    };

    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices.data(),
        GL_STATIC_DRAW
    );
}


void Button::draw() {
    glBindVertexArray(this->vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


bool Button::is_in_bounds(glm::vec2 point) {
    // Simple bounds check from the AABB
    float min_x = this->center_pos.x - (this->size.x * 0.5f);
    float max_x = this->center_pos.x + (this->size.x * 0.5f);

    float min_y = this->center_pos.y - (this->size.y * 0.5f);
    float max_y = this->center_pos.y + (this->size.y * 0.5f);

    return point.x > min_x
        && point.x < max_x
        && point.y > min_y
        && point.y < max_y;
}
