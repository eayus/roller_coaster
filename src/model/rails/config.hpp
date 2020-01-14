#pragma once

#include <glm/glm.hpp>

// File containing configuration information about the track models.
// This allows for easy adjustment, especially when compared to if
// the models were created separately in a modelling application.

// The "Rails" are the metal runners that the rollercoaster slides on
// The "Beams" are the horizontal bars that connect the two rails
// The "Track" is the rails + beams

constexpr float TRACK_LENGTH = 1.0f;
constexpr float TRACK_WIDTH = 0.8f;
constexpr float RAIL_THICKNESS = 0.1f;
constexpr glm::vec3 RAIL_COLOR = glm::vec3(0.4f, 0.4f, 0.4f);
constexpr float BEAM_THICKNESS = 0.05f;
constexpr glm::vec3 BEAM_COLOR = glm::vec3(0.54f, 0.27f, 0.075f);

