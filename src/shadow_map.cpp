#include <shadow_map.hpp>
#include <globals.hpp>

ShadowMap::ShadowMap() {
    glGenFramebuffers(1, &this->frame_buffer);
    glGenTextures(1, &this->texture);
    
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        SHADOW_RESOLUTION,
        SHADOW_RESOLUTION,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            this->texture,
            0
        );
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void ShadowMap::bind_as_target() {
    glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);
}


void ShadowMap::unbind_as_target() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, globals::WINDOW_DIMS.width, globals::WINDOW_DIMS.height);
}


void ShadowMap::bind_as_source() {
    glBindTexture(GL_TEXTURE_2D, this->texture);
}
