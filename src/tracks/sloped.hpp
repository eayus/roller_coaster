#pragma once

#include <model/rails/slope.hpp>
#include <model/transform.hpp>
#include <glm/glm.hpp>

#include <model/rails/path.hpp>

namespace tracks::sloped {
    namespace down {
        constexpr auto MODEL = translate_model(rotate_180(SLOPED_TRACK), glm::vec3(0, -1, 0));

        constexpr glm::vec3 calc_position(float percent) {
            return glm::vec3(0.0f, -percent, 0.5 - percent);
        }

        constexpr Rotation calc_rotation(float percent) {
            percent *= 100.0f;

            if (percent <= 20.0f) {
                float t = percent / 20.0f;

                return Rotation(
                    0.0f,
                    glm::radians(-45.0f) * t
                );
            }

            if (percent >= 80.0f) {
                return Rotation(
                    0.0f,
                    glm::radians(-45.0f) * (100.0f - percent) / 20.0f
                );
            }
            
            return Rotation(
                0.0f,
                glm::radians(-45.0f)
            );
        }
    }
}
