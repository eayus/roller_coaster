#pragma once

#include <model/model.hpp>
#include <model/transform.hpp>
#include <util.hpp>

// Functions to create several primtiive models. The implementaiton is
// fairly trivial, they are created through consturction of smaller
// primitives

constexpr Model<3, 3> make_triangle(
    glm::vec3 point1,
    glm::vec3 point2,
    glm::vec3 point3,
    glm::vec3 color
) {
    glm::vec3 normal = cross_product(point2 - point1, point3 - point1);

    return Model<3, 3>(
        {{
            Vertex(point1, normal, color),
            Vertex(point2, normal, color),
            Vertex(point3, normal, color)
        }},
        {{ 0, 1, 2 }}
    );
}

constexpr Model<4, 6> make_rect(
    glm::vec3 bottom_left,
    glm::vec3 right_dir,
    glm::vec3 up_dir,
    glm::vec3 color
) {
    glm::vec3 bottom_right = bottom_left + right_dir;
    glm::vec3 top_left     = bottom_left + up_dir;
    glm::vec3 top_right    = bottom_left + right_dir + up_dir;

    glm::vec3 normal = cross_product(right_dir, up_dir);

    return Model<4, 6>(
        {{
            Vertex(bottom_left, normal, color),
            Vertex(bottom_right, normal, color),
            Vertex(top_left, normal, color),
            Vertex(top_right, normal, color)
        }},
        {{ 0, 1, 2, 1, 3, 2 }}
    );
}


constexpr Model<16, 24> make_endless_cuboid(
    glm::vec3 bottom_back_left,
    glm::vec3 right_dir,
    glm::vec3 up_dir,
    glm::vec3 forward_dir,
    glm::vec3 color
) {
    auto left_face = make_rect(
        bottom_back_left,
        forward_dir,
        up_dir,
        color
    );

    auto right_face = make_rect(
        bottom_back_left + right_dir + forward_dir,
        -forward_dir,
        up_dir,
        color
    );

    auto bottom_face = make_rect(
        bottom_back_left,
        right_dir,
        forward_dir,
        color
    );

    auto top_face = make_rect(
        bottom_back_left + up_dir + forward_dir,
        right_dir,
        -forward_dir,
        color
    );

    return combine_all(left_face, right_face, bottom_face, top_face);
}
