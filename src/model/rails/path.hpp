#pragma once

//#include <array>
#include <std_extras/array.hpp>
#include <glm/glm.hpp>
#include <model/rails/config.hpp>
#include <gcem/gcem.hpp>

using std_extras::Array;

struct Rotation {
    constexpr Rotation(float yaw, float pitch)
        : yaw(yaw)
        , pitch(pitch) {}

    float yaw;
    float pitch;
};


constexpr float TRACK_FORWARD_LEN = 1.0f;
constexpr float TRACK_CURVE_LEN = static_cast<float>(GCEM_PI) * 0.25f;
constexpr float TRACK_SLOPED_LEN = gcem::sqrt(1.0f + 1.0f);

constexpr glm::vec3 straight_track_position(float percent) {
    return glm::vec3(0.0f, 0.0f, 0.5f - percent);
}

inline glm::vec3 curved_track_position(float percent) {
    const float theta = percent * glm::radians(90.0f);

    return glm::vec3(
        -0.5f + (0.5f * glm::cos(theta)),
        0.0f,
        0.5f - (0.5f * glm::sin(theta))
    );
}

constexpr glm::vec3 sloped_track_position(float percent) {
    return glm::vec3(0.0f, percent, 0.5f - percent);
}


constexpr Rotation curved_track_rotation(float percent) {
    return Rotation(
        -percent * glm::radians(90.0f),
        0.0f
    );
}

constexpr Rotation straight_track_rotation(float percent) {
    return Rotation(
        0.0f,
        0.0f
    );
}

constexpr Rotation sloped_track_rotation(float percent) {
    percent *= 100.0f;

    if (percent <= 20.0f) {
        float t = percent / 20.0f;

        return Rotation(
            0.0f,
            glm::radians(45.0f) * t
        );
    }

    if (percent >= 80.0f) {
        return Rotation(
            0.0f,
            glm::radians(45.0f) * (100.0f - percent) / 20.0f
        );
    }
    
    return Rotation(
        0.0f,
        glm::radians(45.0f)
    );
}














// Number of positions that make up a path, per rail
constexpr size_t NUM_PATH_SEGMENTS = 10;


// Straight Path
constexpr float STRAIGHT_PATH_SEGMENT_LENGTH = TRACK_LENGTH / static_cast<float>(NUM_PATH_SEGMENTS);

constexpr Array<glm::vec3, 2> SLOPED_PATH = {
      glm::vec3(0.0f, 0.0f, TRACK_LENGTH / 2.0f),
    { glm::vec3(0.0f, 1.0f, -TRACK_LENGTH / 2.0f) }
};

constexpr Array<glm::vec3, 2> STRAIGHT_PATH = {
      glm::vec3(0.0f, 0.0f, (TRACK_LENGTH / 2.0f) - (0.0f * STRAIGHT_PATH_SEGMENT_LENGTH)),
    { glm::vec3(0.0f, 0.0f, (TRACK_LENGTH / 2.0f) - (10.0f * STRAIGHT_PATH_SEGMENT_LENGTH)) }
};


// Curved Path
constexpr float CURVED_PATH_RADIUS = TRACK_LENGTH / 2.0f;
constexpr float CURVED_PATH_ANGLE_STEP = glm::radians(90.0f / static_cast<float>(NUM_PATH_SEGMENTS));

/*constexpr std::array<glm::vec3, NUM_PATH_SEGMENTS> CURVED_PATH = {
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(0.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(0.0f * CURVED_PATH_ANGLE_STEP))
    ),
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(1.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(1.0f * CURVED_PATH_ANGLE_STEP))
    ),
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(2.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(2.0f * CURVED_PATH_ANGLE_STEP))
    ),
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(3.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(3.0f * CURVED_PATH_ANGLE_STEP))
    ),
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(4.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(4.0f * CURVED_PATH_ANGLE_STEP))
    ),
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(5.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(5.0f * CURVED_PATH_ANGLE_STEP))
    ),
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(6.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(6.0f * CURVED_PATH_ANGLE_STEP))
    ),
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(7.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(7.0f * CURVED_PATH_ANGLE_STEP))
    ),
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(8.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(8.0f * CURVED_PATH_ANGLE_STEP))
    ),
    glm::vec3(
        (CURVED_PATH_RADIUS * gcem::cos(9.0f * CURVED_PATH_ANGLE_STEP)) - CURVED_PATH_RADIUS,
        0.0f,
        CURVED_PATH_RADIUS - (CURVED_PATH_RADIUS * gcem::sin(9.0f * CURVED_PATH_ANGLE_STEP))
    ),
};*/
