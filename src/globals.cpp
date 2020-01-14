#include <globals.hpp>

// Initialisation of globals

namespace globals {
    float WindowDimensions::aspect_ratio() const {
        return static_cast<float>(WINDOW_DIMS.width)
             / static_cast<float>(WINDOW_DIMS.height);
    }

    WindowDimensions WINDOW_DIMS;

    bool mouse_just_clicked = false;
}
