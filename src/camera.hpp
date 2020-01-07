#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>

enum class CameraMode {
    Free,                  // Let the user move camera with arrow keys
    Fixed,                 // Camera position is autoamtically controlled
};

class Camera {
public:
    Camera(const glm::vec3& initial_position, GLFWwindow& window);

    void transform_matrix(glm::mat4& out_matrix);
    void update(GLFWwindow& window);
    glm::vec3 get_position() const;
    void set_mode(CameraMode mode);
    void set_position(const glm::vec3& position);
    void set_facing(const glm::vec3& facing);

private:
    CameraMode mode;
    glm::vec3 position;
    glm::vec3 facing;
};
