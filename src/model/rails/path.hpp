#pragma once

//#include <array>
#include <std_extras/array.hpp>
#include <glm/glm.hpp>
#include <model/rails/config.hpp>
#include <gcem/gcem.hpp>
#include <model/track_type.hpp>

using std_extras::Array;

// Utility file for defining what path the cart should take through the track

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


constexpr Rotation curved_track_rotation(float percent, TrackType prev_type, TrackType next_type) {
    return Rotation(
        -percent * glm::radians(90.0f),
        0.0f
    );
}

constexpr Rotation straight_track_rotation(float percent, TrackType prev_type, TrackType next_type) {
    return Rotation(
        0.0f,
        0.0f
    );
}

constexpr Rotation sloped_track_rotation(float percent, TrackType prev_type, TrackType next_type) {

    if (percent <= 0.2f && prev_type != TrackType::SlopeUp) {
        return Rotation(0.0f, glm::radians(45.0f) * percent / 0.2f);
    }

    if (percent >= 0.8f && next_type != TrackType::SlopeUp) {
        return Rotation(0.0f, glm::radians(45.0f) * (1.0f - percent) / 0.2f);
    }

    return Rotation(
        0.0f,
        glm::radians(45.0f)
    );
}


/*// Number of positions that make up a path, per rail
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
constexpr float CURVED_PATH_ANGLE_STEP = glm::radians(90.0f / static_cast<float>(NUM_PATH_SEGMENTS));*/
