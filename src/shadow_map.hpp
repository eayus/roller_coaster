#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

constexpr unsigned int SHADOW_RESOLUTION = 2048;

class ShadowMap {
public:
    ShadowMap();
    void bind_as_target();
    void unbind_as_target();
    void bind_as_source();

private:
    GLuint texture;
    GLuint frame_buffer;
};
