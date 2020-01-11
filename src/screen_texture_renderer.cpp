#include <screen_texture_renderer.hpp>
#include <array>

ScreenTextureRenderer::ScreenTextureRenderer()
    : shader_prog(
        VertexShader::from_filepath("res/shaders/screen_texture.vert"),
        FragmentShader::from_filepath("res/shaders/screen_texture.frag")
    )
{
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vertex_buffer);

    glBindVertexArray(this->vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec2), (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec2), (void*)(sizeof(glm::vec2)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    constexpr std::array<glm::vec2, 12> vertices = {
        glm::vec2(-1, 1), glm::vec2(0, 1),  // top left
        glm::vec2(-1, -1), glm::vec2(0, 0), // bottom left
        glm::vec2(1, 1), glm::vec2(1, 1),   // top right

        glm::vec2(1, -1), glm::vec2(1, 0),   // bottom right
        glm::vec2(1, 1), glm::vec2(1, 1),   // top right
        glm::vec2(-1, -1), glm::vec2(0, 0) // bottom left
    };

    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices.data(),
        GL_STATIC_DRAW
    );
}

void ScreenTextureRenderer::draw_texture(GLuint texture) {
    this->shader_prog.bind();
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
