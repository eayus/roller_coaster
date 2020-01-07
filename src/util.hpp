#pragma once

#include <glm/glm.hpp>
#include <array>
#include <gcem/gcem.hpp>

constexpr void rotate_vec_right(glm::vec3& vec) {
    // (x, y, z) -> (-z, y, x)
    float tmp = vec.x;
    vec.x = -vec.z;
    vec.z = tmp;
}

constexpr void rotate_vec_left(glm::vec3& vec) {
    // (x, y, z) -> (z, y, -x)
    float tmp = vec.x;
    vec.x = vec.z;
    vec.z = -tmp;
}

constexpr void rotate_vec_down(glm::vec3& vec) {
    // (x, y, z) -> (-x, y, -z)
    vec.x = -vec.x;
    vec.z = -vec.z;
}

constexpr void rotate_vec_right(glm::ivec3& vec) {
    // (x, y, z) -> (-z, y, x)
    float tmp = vec.x;
    vec.x = -vec.z;
    vec.z = tmp;
}

constexpr void rotate_vec_left(glm::ivec3& vec) {
    // (x, y, z) -> (z, y, -x)
    float tmp = vec.x;
    vec.x = vec.z;
    vec.z = -tmp;
}

constexpr void rotate_vec_down(glm::ivec3& vec) {
    // (x, y, z) -> (-x, y, -z)
    vec.x = -vec.x;
    vec.z = -vec.z;
}

constexpr glm::vec3 cross_product(glm::vec3 a, glm::vec3 b) {
    return glm::vec3(
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    );
}

constexpr float magnitude(glm::vec3 a) {
    return gcem::sqrt(
        (a.x * a.x) +
        (a.y * a.y) +
        (a.z * a.z)
    );
}


enum class Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3,
};

constexpr Direction concat_directions(Direction dir1, Direction dir2) {
    const auto x = static_cast<unsigned int>(dir1) + static_cast<unsigned int>(dir2);
    return static_cast<Direction>(x % 4);
}

constexpr Direction turn_dir_left(Direction dir) {
    switch (dir) {
        case Direction::Up: return Direction::Left;
        case Direction::Right: return Direction::Up;
        case Direction::Down: return Direction::Right;
        case Direction::Left: return Direction::Down;
    }
}

constexpr glm::ivec2 dir_to_vec(Direction dir) {
    switch (dir) {
        case Direction::Up: return glm::ivec2(0, 1);
        case Direction::Right: return glm::ivec2(1, 0);
        case Direction::Down: return glm::ivec2(0, -1);
        case Direction::Left: return glm::ivec2(-1, 0);
    }
}

constexpr void rotate_vec_by_dir(glm::vec3& vec, Direction dir) {
    if (dir == Direction::Right) rotate_vec_right(vec);
    else if (dir == Direction::Left) rotate_vec_left(vec);
    else if (dir == Direction::Down) rotate_vec_down(vec);
}

constexpr void rotate_vec_by_dir(glm::ivec3& vec, Direction dir) {
    if (dir == Direction::Right) rotate_vec_right(vec);
    else if (dir == Direction::Left) rotate_vec_left(vec);
    else if (dir == Direction::Down) rotate_vec_down(vec);
}
