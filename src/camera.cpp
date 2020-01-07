#include <camera.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

Camera::Camera(const glm::vec3& initial_position, GLFWwindow& window)
    : position(0, 2, 3)
    , facing(0, 0, -3)
    , mode(CameraMode::Free)
{
    glfwSetCursorPos(&window, 0.0, 0.0);
}

void Camera::transform_matrix(glm::mat4& out_matrix) {
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        1920.0f / 1080.0f, //TODO: base this off window size
        0.1f,
        1000.0f
    );

    glm::mat4 view = glm::lookAt(
        this->position,
        this->position + this->facing,
        glm::vec3(0, 1, 0)
    );

    out_matrix = proj * view;
}

void Camera::update(GLFWwindow& window) {
    if (this->mode == CameraMode::Free) {
        double cursor_x;
        double cursor_y;
        glfwGetCursorPos(&window, &cursor_x, &cursor_y);
        glfwSetCursorPos(&window, 0.0, 0.0);

        this->facing = glm::rotate(
            this->facing,
            glm::radians(static_cast<float>(cursor_x)) * 0.5f,
            glm::vec3(0, -1, 0)
        );

        glm::vec3 forward = 0.05f * glm::normalize(glm::vec3(this->facing.x, 0, this->facing.z));
        glm::vec3 left = 0.05f * glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));

        float rotate_amount = glm::radians(static_cast<float>(cursor_y) * 0.5f);
        if (rotate_amount > 0) {
            float current_angle = glm::angle(glm::vec3(0, -1, 0), glm::normalize(this->facing));

            if (current_angle >= rotate_amount) {
                this->facing = glm::rotate(
                    this->facing,
                    rotate_amount,
                    left
                );
            }
        } else {
            float current_angle = glm::angle(glm::vec3(0, 1, 0), glm::normalize(this->facing));

            if (current_angle >= -rotate_amount) {
                this->facing = glm::rotate(
                    this->facing,
                    rotate_amount,
                    left
                );
            }
        }

        if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS) this->position += forward;
        if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS) this->position -= forward;
        if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS) this->position -= left;
        if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS) this->position += left;
        if (glfwGetKey(&window, GLFW_KEY_SPACE) == GLFW_PRESS) this->position += 0.05f * glm::vec3(0, 1, 0);
        if (glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) this->position -= 0.05f * glm::vec3(0, 1, 0);
    }
}

glm::vec3 Camera::get_position() const {
    return this->position;
}

void Camera::set_mode(CameraMode mode) {
    this->mode = mode;
}

void Camera::set_position(const glm::vec3& position) {
    this->position = position;
}

void Camera::set_facing(const glm::vec3& facing) {
    this->facing = facing;
}
