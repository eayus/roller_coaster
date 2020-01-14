#include <ground.hpp>
#include <array>
#include <model/rails/config.hpp>
#include <stb/stb_image.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>

Ground Ground::underneath_coaster(const RollerCoaster& rc) {
    Ground grnd;

    // Gen buffers and arrays and specify vertex format
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
    const float offset = 0.5f + EXTRA_RIM;
    float left = static_cast<float>(dims.min_x) - offset;
    float right = static_cast<float>(dims.max_x) + offset;
    float back = static_cast<float>(dims.min_z) - offset;
    float front = static_cast<float>(dims.max_z) + offset;
    float y = -RAIL_THICKNESS * 0.5f;

    // Face orders: Top Left, Bottom Left, Top Right, Bottom right
    
    constexpr float SUBDIVIDE_SIZE = 0.3f;

    int num_horiz = ceil((right - left) / SUBDIVIDE_SIZE);
    int num_vert = ceil((front - back) / SUBDIVIDE_SIZE);

    // Subdivide whole plane into small segments, and offset the Y coord by a small amount.
    std::vector<glm::vec3> points;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 50);
    auto r = std::bind(distribution, generator);
    
    for (int j = 0; j < num_vert; j++) {
        for (int i = 0; i < num_horiz; i++) {
            points.emplace_back(
                left + (i * SUBDIVIDE_SIZE),
                y + (static_cast<float>(r()) / 1000.0f),
                back + (j * SUBDIVIDE_SIZE)
            );
        }
    }



    std::vector<Vertex> vertices;
    std::vector<Index> indices;


    // Code to generate the faces from the points. Each face follows a similar structure.
    // We iterate over pairs of vertices in the grid, and create triangles with the apporpriate
    // normals. To give a better visual effect, we alternative the way we divide the grid
    // square into triangles


    // Top Face
    for (int j = 1; j < num_vert; j++) {
        for (int i = 1; i < num_horiz; i++) {
            Index index_offset = static_cast<Index>(vertices.size());

            auto top_left = points.at((i - 1) + ((j - 1) * num_horiz));
            auto bot_left = points.at((i - 1) + (j * num_horiz));
            auto top_right = points.at(i + ((j - 1) * num_horiz));
            auto bot_right = points.at(i + (j * num_horiz));

            if ((i + j) % 2 == 0) {
                auto delta_x1 = top_right - top_left;
                auto delta_z1 = bot_left - top_left;

                auto delta_x2 = bot_right - bot_left;
                auto delta_z2 = bot_right - top_right;

                auto normal1 = glm::cross(delta_z1, delta_x1);
                auto normal2 = glm::cross(delta_z2, delta_x2);

                vertices.emplace_back(top_left, normal1, GRASS_COLOR);
                vertices.emplace_back(bot_left, normal1, GRASS_COLOR);
                vertices.emplace_back(top_right, normal1, GRASS_COLOR);

                vertices.emplace_back(bot_left, normal2, GRASS_COLOR);
                vertices.emplace_back(bot_right, normal2, GRASS_COLOR);
                vertices.emplace_back(top_right, normal2, GRASS_COLOR);
            } else {
                auto delta_x1 = top_right - top_left;
                auto delta_z1 = bot_right - top_right;

                auto delta_x2 = bot_right - bot_left;
                auto delta_z2 = bot_left - top_left;

                auto normal1 = glm::cross(delta_z1, delta_x1);
                auto normal2 = glm::cross(delta_z2, delta_x2);

                vertices.emplace_back(top_left, normal1, GRASS_COLOR);
                vertices.emplace_back(bot_right, normal1, GRASS_COLOR);
                vertices.emplace_back(top_right, normal1, GRASS_COLOR);

                vertices.emplace_back(top_left, normal2, GRASS_COLOR);
                vertices.emplace_back(bot_left, normal2, GRASS_COLOR);
                vertices.emplace_back(bot_right, normal2, GRASS_COLOR);
            }

            indices.push_back(index_offset + 0);
            indices.push_back(index_offset + 1);
            indices.push_back(index_offset + 2);
            indices.push_back(index_offset + 3);
            indices.push_back(index_offset + 4);
            indices.push_back(index_offset + 5);
        }
    }


    // Back Side
    for (int i = 1; i < num_horiz; i++) {
        const auto t1 = points.at(i - 1);
        const auto t2 = points.at(i);

        auto b1 = t1;
        b1.y = y - GROUND_HEIGHT;

        auto b2 = t2;
        b2.y = y - GROUND_HEIGHT;

        Index index_offset = static_cast<Index>(vertices.size());

        glm::vec3 normal(0, 0, -1);

        vertices.emplace_back(t1, normal, STONE_COLOR);
        vertices.emplace_back(t2, normal, STONE_COLOR);
        vertices.emplace_back(b1, normal, STONE_COLOR);
        vertices.emplace_back(b2, normal, STONE_COLOR);

        indices.push_back(index_offset + 0);
        indices.push_back(index_offset + 2);
        indices.push_back(index_offset + 1);

        indices.push_back(index_offset + 2);
        indices.push_back(index_offset + 3);
        indices.push_back(index_offset + 1);
    }

    // Front Side
    for (int i = 1; i < num_horiz; i++) {
        const auto t1 = points.at(((num_vert - 1) * num_horiz) + i - 1);
        const auto t2 = points.at(((num_vert - 1) * num_horiz) + i);

        auto b1 = t1;
        b1.y = y - GROUND_HEIGHT;

        auto b2 = t2;
        b2.y = y - GROUND_HEIGHT;

        Index index_offset = static_cast<Index>(vertices.size());

        glm::vec3 normal(0, 0, 1);

        vertices.emplace_back(t1, normal, STONE_COLOR);
        vertices.emplace_back(t2, normal, STONE_COLOR);
        vertices.emplace_back(b1, normal, STONE_COLOR);
        vertices.emplace_back(b2, normal, STONE_COLOR);

        indices.push_back(index_offset + 0);
        indices.push_back(index_offset + 2);
        indices.push_back(index_offset + 1);

        indices.push_back(index_offset + 2);
        indices.push_back(index_offset + 3);
        indices.push_back(index_offset + 1);
    }

    // Left
    for (int i = 1; i < num_vert; i++) {
        const auto t1 = points.at((i - 1) * num_horiz);
        const auto t2 = points.at(i * num_horiz);

        auto b1 = t1;
        b1.y = y - GROUND_HEIGHT;

        auto b2 = t2;
        b2.y = y - GROUND_HEIGHT;

        Index index_offset = static_cast<Index>(vertices.size());

        glm::vec3 normal(-1, 0, 0);

        vertices.emplace_back(t1, normal, STONE_COLOR);
        vertices.emplace_back(t2, normal, STONE_COLOR);
        vertices.emplace_back(b1, normal, STONE_COLOR);
        vertices.emplace_back(b2, normal, STONE_COLOR);

        indices.push_back(index_offset + 0);
        indices.push_back(index_offset + 2);
        indices.push_back(index_offset + 1);

        indices.push_back(index_offset + 2);
        indices.push_back(index_offset + 3);
        indices.push_back(index_offset + 1);
    }

    // Right
    for (int i = 1; i < num_vert; i++) {
        const auto t1 = points.at(((i - 1) * num_horiz) + (num_horiz - 1));
        const auto t2 = points.at((i * num_horiz) + (num_horiz - 1));

        auto b1 = t1;
        b1.y = y - GROUND_HEIGHT;

        auto b2 = t2;
        b2.y = y - GROUND_HEIGHT;

        Index index_offset = static_cast<Index>(vertices.size());

        glm::vec3 normal(1, 0, 0);

        vertices.emplace_back(t1, normal, STONE_COLOR);
        vertices.emplace_back(t2, normal, STONE_COLOR);
        vertices.emplace_back(b1, normal, STONE_COLOR);
        vertices.emplace_back(b2, normal, STONE_COLOR);

        indices.push_back(index_offset + 0);
        indices.push_back(index_offset + 2);
        indices.push_back(index_offset + 1);

        indices.push_back(index_offset + 2);
        indices.push_back(index_offset + 3);
        indices.push_back(index_offset + 1);
    }

    // Bottom
    const float bottom_y = y - GROUND_HEIGHT;
    {
        const auto real_left = points.at(0).x;
        const auto real_right = points.at(num_horiz - 1).x;
        const auto real_back = points.at(0).z;
        const auto real_front = points.at(num_horiz * (num_vert - 1)).z;
        Index index_offset = static_cast<Index>(vertices.size());

        const glm::vec3 normal(0, -1, 0);
        vertices.emplace_back(glm::vec3(real_left, bottom_y, real_back), normal, STONE_COLOR);
        vertices.emplace_back(glm::vec3(real_left, bottom_y, real_front), normal, STONE_COLOR);
        vertices.emplace_back(glm::vec3(real_right, bottom_y, real_back), normal, STONE_COLOR);
        vertices.emplace_back(glm::vec3(real_right, bottom_y, real_front), normal, STONE_COLOR);

        indices.push_back(index_offset + 0);
        indices.push_back(index_offset + 1);
        indices.push_back(index_offset + 2);

        indices.push_back(index_offset + 1);
        indices.push_back(index_offset + 2);
        indices.push_back(index_offset + 3);
    }



    // Store number of indices later for the draw call
    grnd.num_indices = indices.size();


    // Fill OpenGL buffers with the vertices we just generated
    glBindBuffer(GL_ARRAY_BUFFER, grnd.vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grnd.index_buffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(Index),
        indices.data(),
        GL_STATIC_DRAW
    );

    return grnd;
}


void Ground::draw() {
    glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);
}
