#pragma once

#include <GL/glew.h>

class VirtualScreen {
public:
    VirtualScreen();
    ~VirtualScreen();
    void bind_as_target();
    void bind_as_source();

    static void bind_none_as_target();

private:
    GLuint frame_buffer;
    GLuint texture;
    GLuint render_buffer;
};
