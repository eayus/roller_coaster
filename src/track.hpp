#pragma once

#include <model/tracks.hpp>
#include <util.hpp>

// Simple struct storing information about a track segemnet in a roller coaster

struct TrackSegment {
    TrackType type;
    glm::ivec3 position;
    Direction direction;
};

