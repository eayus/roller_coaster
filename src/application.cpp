#include <GL/glew.h>
#include <application.hpp>
#include <shader.hpp>
#include <shader_program.hpp>
#include <string>
#include <iostream>
#include <camera.hpp>
#include <cart_camera.hpp>
#include <model/scene.hpp>
#include <roller_coaster.hpp>
#include <cart.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <globals.hpp>
#include <free_camera.hpp>
#include <ground.hpp>
#include <screen_texture_renderer.hpp>
#include <virtual_screen.hpp>

void gl_error_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* user_param)
{
    std::cerr << "[OPENGL MESSAGE] " << message << std::endl << std::endl;
}

enum class GameMode {
    Observer,
    Ride,
};

Application::Application() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);

    std::cout << video_mode->width << ", " << video_mode->height << std::endl;

    this->window = glfwCreateWindow(
        video_mode->width,
        video_mode->height,
        "Hello World",
        monitor,
        nullptr
    );

    globals::WINDOW_DIMS.width = video_mode->width;
    globals::WINDOW_DIMS.height = video_mode->height;

    glfwMakeContextCurrent(this->window);
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = true;
    glewInit();
    glDebugMessageCallback(gl_error_callback, nullptr);
}

Application::~Application() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void Application::main_loop() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    ShaderProgram colored_mesh_shader(
        VertexShader::from_filepath("res/shaders/colored_mesh.vert"),
        FragmentShader::from_filepath("res/shaders/colored_mesh.frag")
    );

    ShaderProgram screen_texture_shader(
        VertexShader::from_filepath("res/shaders/screen_texture.vert"),
        FragmentShader::from_filepath("res/shaders/screen_texture.frag")
    );

    RollerCoaster main_coaster("res/roller_coasters/test.track");

    Ground grnd = Ground::underneath_coaster(main_coaster);

    std::vector<Cart> carts;
    carts.push_back(main_coaster.add_cart());
    carts.push_back(Cart::behind(carts[0]));
    carts.push_back(Cart::behind(carts[1]));


    CartCamera cart_camera(carts.back());
    FreeCamera free_camera(glm::vec3(0, 0, 2), *this->window);

    GameMode gm = GameMode::Observer;

    ScreenTextureRenderer st_renderer;

    // Framebuffer stuff
    /*GLuint scene_framebuffer, scene_texture, scene_renderbuffer;
    glGenFramebuffers(1, &scene_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, scene_framebuffer);


    glGenTextures(1, &scene_texture);
    glBindTexture(GL_TEXTURE_2D, scene_texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        globals::WINDOW_DIMS.width,
        globals::WINDOW_DIMS.height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene_texture, 0);


    glGenRenderbuffers(1, &scene_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, scene_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, globals::WINDOW_DIMS.width, globals::WINDOW_DIMS.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, scene_renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
	    std::cerr << "[ERROR]: framebuffer not complete" << std::endl;
    }*/

    VirtualScreen vs;


    while (!glfwWindowShouldClose(this->window)) {

        if (gm == GameMode::Ride) {
            colored_mesh_shader.bind();
            colored_mesh_shader.set_uniform_mat4("camera_matrix", cart_camera.calculate_matrix());
            colored_mesh_shader.set_uniform_vec3("camera_position", cart_camera.get_position());
        } else if (gm == GameMode::Observer) {
            colored_mesh_shader.bind();
            colored_mesh_shader.set_uniform_mat4("camera_matrix", free_camera.calculate_matrix());
            colored_mesh_shader.set_uniform_vec3("camera_position", free_camera.get_position());
        }


        for (auto& cart : carts) cart.update();
        auto cart_pos = carts[0].calc_position();


        vs.bind_as_target();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            colored_mesh_shader.bind();
            colored_mesh_shader.set_uniform_mat4("model_matrix", glm::mat4(1.0f));
            main_coaster.draw();
            grnd.draw();
            for (auto& cart : carts) cart.draw(colored_mesh_shader);

        VirtualScreen::bind_none_as_target();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            vs.bind_as_source();
            st_renderer.draw_texture(0);

        glfwSwapBuffers(this->window);
        glfwPollEvents();

        if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
        if (glfwGetKey(this->window, GLFW_KEY_1) == GLFW_PRESS) glEnable(GL_CULL_FACE);
        if (glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS) gm = GameMode::Observer;
        if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS) gm = GameMode::Ride;

        if (gm == GameMode::Observer) free_camera.handle_inputs(*this->window);
    }
}
