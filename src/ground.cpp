#include <ground.hpp>
#include <array>

Ground Ground::underneath_coaster(const RollerCoaster& rc) {
    Ground grnd;

    glGenVertexArrays(1, &grnd.vao);
    glGenBuffers(1, &grnd.vertex_buffer);
    glGenBuffers(1, &grnd.index_buffer);

    glBindVertexArray(grnd.vao);
        glBindBuffer(GL_ARRAY_BUFFER, grnd.vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grnd.index_buffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    // Calculate Vertices
    const auto& dims = rc.get_dims();
    float left = static_cast<float>(dims.min_x) - 0.5f;
    float right = static_cast<float>(dims.max_x) + 0.5f;
    float back = static_cast<float>(dims.min_z) - 0.5f;
    float front = static_cast<float>(dims.max_z) + 0.5f;
    float y = -0.2f;

    // Face orders: Top Left, Bottom Left, Top Right, Bottom right

    std::array<glm::vec3, 3 * 4> vertices = { // pos, normal, color
        // Top Face
        glm::vec3(left, y, back), glm::vec3(0, 1, 0), GRASS_COLOR,
        glm::vec3(left, y, front), glm::vec3(0, 1, 0), GRASS_COLOR,
        glm::vec3(right, y, back), glm::vec3(0, 1, 0), GRASS_COLOR,
        glm::vec3(right, y, front), glm::vec3(0, 1, 0), GRASS_COLOR,
    };

    std::array<unsigned short, 6> indices = {
        // Top Face
        0, 1, 2, 1, 3, 2
    };

    
    glBindBuffer(GL_ARRAY_BUFFER, grnd.vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glm::vec3),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grnd.index_buffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned short),
        indices.data(),
        GL_STATIC_DRAW
    );

    return grnd;
}


void Ground::draw() {
    glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, 6 /* num indices */, GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);
}
