#pragma once

#include <model/basic.hpp>
#include <model/transform.hpp>
#include <model/rails/straight.hpp>
#include <model/rails/curved.hpp>

constexpr auto FLOOR_MODEL = make_rect(
    glm::vec3(-10.0f, -5.0f, 10.0f),
    glm::vec3(20.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -20.0f),
    glm::vec3(0.1f, 0.9f, 0.4f)
);

constexpr auto TESTING_MODEL = make_endless_cuboid(
    glm::vec3(-0.5f, 2.0f, -0.5f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f)
);

constexpr auto COMBINED_MODEL = combine_all(
    FLOOR_MODEL,
    STRAIGHT_TRACK,
    translate_model(STRAIGHT_TRACK, glm::vec3(0, 0, 1)),
    translate_model(CURVED_TRACK, glm::vec3(0, 0, -1)),
    translate_model(rotate_90(STRAIGHT_TRACK), glm::vec3(-1, 0, -1)),
    translate_model(rotate_270(CURVED_TRACK), glm::vec3(-2, 0, -1))
);
