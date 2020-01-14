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
#include <ui/font.hpp>
#include <ui/text.hpp>
#include <ui/button.hpp>
#include <shadow_map.hpp>

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        globals::mouse_just_clicked = true;
    }
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glm::vec3 light_pos(30.0, 90.0, 17.5);


    ShaderProgram colored_mesh_shader(
        VertexShader::from_filepath("res/shaders/colored_mesh.vert"),
        FragmentShader::from_filepath("res/shaders/colored_mesh.frag")
    );
    colored_mesh_shader.bind();
    colored_mesh_shader.set_uniform_vec3("light_position", light_pos);

    ShaderProgram screen_texture_shader(
        VertexShader::from_filepath("res/shaders/screen_texture.vert"),
        FragmentShader::from_filepath("res/shaders/screen_texture.frag")
    );

    ShaderProgram text_shader(
        VertexShader::from_filepath("res/shaders/text.vert"),
        FragmentShader::from_filepath("res/shaders/text.frag")
    );
    text_shader.bind();
    text_shader.set_uniform_float("window_width", static_cast<float>(globals::WINDOW_DIMS.width));
    text_shader.set_uniform_float("window_height", static_cast<float>(globals::WINDOW_DIMS.height));

    ShaderProgram ui_shader(
        VertexShader::from_filepath("res/shaders/ui.vert"),
        FragmentShader::from_filepath("res/shaders/ui.frag")
    );
    ui_shader.bind();
    ui_shader.set_uniform_float("window_width", static_cast<float>(globals::WINDOW_DIMS.width));
    ui_shader.set_uniform_float("window_height", static_cast<float>(globals::WINDOW_DIMS.height));

    RollerCoaster main_coaster("res/roller_coasters/test.track");

    Ground grnd = Ground::underneath_coaster(main_coaster);

    std::vector<Cart> carts;
    carts.push_back(main_coaster.add_cart());
    carts.push_back(Cart::behind(carts[0]));
    carts.push_back(Cart::behind(carts[1]));


    CartCamera cart_camera(carts.back());
    FreeCamera free_camera(glm::vec3(0, 0, 2), *this->window);

    GameMode gm = GameMode::Observer;
    bool paused = false;
    // When we pause, set these coordinates to mouse pos, and restore them when unpausing
    double cursor_x_paused;
    double cursor_y_paused; 

    ScreenTextureRenderer st_renderer;

    std::array<VirtualScreen, 2> vs;

    Font fira_mono("res/fonts/fira_mono_regular.png");

    Button resume_button(glm::vec2(1920.0f * 0.5f, 600.0f), glm::vec2(380.0f, 80.0f));
    Text resume_text("resume game", fira_mono, glm::vec2(790, 585));

    Button quit_button(glm::vec2(1920.0f * 0.5f, 480.0f), glm::vec2(380.0f, 80.0f));
    Text quit_text("quit game", fira_mono, glm::vec2(820, 465));


    // Set up matrices for rendering scene from light's POV
    glm::mat4 light_proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    glm::mat4 light_view = glm::lookAt(light_pos, glm::vec3(0), glm::vec3(0, 0, 1));

    ShaderProgram shader_map_shader(
        VertexShader::from_filepath("res/shaders/shadow_map.vert"),
        FragmentShader::from_filepath("res/shaders/shadow_map.frag")
    );
    shader_map_shader.bind();
    shader_map_shader.set_uniform_mat4("light_matrix", light_proj * light_view);

    ShadowMap shadow_map;


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

    
        auto render_scene = [&](ShaderProgram& shader) {
            shader.bind();
            shader.set_uniform_mat4("model_matrix", glm::mat4(1.0f));
            main_coaster.draw();
            grnd.draw();
            for (auto& cart : carts) cart.draw(shader);
        };

        shadow_map.bind_as_target();
            glClear(GL_DEPTH_BUFFER_BIT);
            render_scene(shader_map_shader);
        shadow_map.unbind_as_target();


        size_t vs_target = 0;

        vs[vs_target].bind_as_target();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            render_scene(colored_mesh_shader);

        vs_target = 1;

        int blur_iterations = paused ? 32 : 0;        
        for (int i = 0; i < blur_iterations; i++) {
            size_t vs_source = (vs_target + 1) % 2;
            vs[vs_source].bind_as_source();
            vs[vs_target].bind_as_target();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Blur blur_type = i % 2 == 0 ? Blur::Horizontal : Blur::Vertical;
            st_renderer.draw_texture(blur_type);

            vs_target = vs_source;
        }

        VirtualScreen::bind_none_as_target();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            size_t vs_source = (vs_target + 1) % 2;
            vs[vs_source].bind_as_source();
            st_renderer.draw_texture(Blur::None);
            if (paused) {
                ui_shader.bind();
                resume_button.draw();
                quit_button.draw();
                text_shader.bind();
                quit_text.draw();
                resume_text.draw();
            }

        glfwSwapBuffers(this->window);
        glfwPollEvents();

        if (!paused && glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            paused = true;
            glfwGetCursorPos(this->window, &cursor_x_paused, &cursor_y_paused);
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        if (glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS && !paused) gm = GameMode::Observer;
        if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS && !paused) gm = GameMode::Ride;

        if (gm == GameMode::Observer && !paused) free_camera.handle_inputs(*this->window);

        if (paused && globals::mouse_just_clicked) {
            double cursor_x;
            double cursor_y;
            glfwGetCursorPos(this->window, &cursor_x, &cursor_y);
            cursor_x = globals::WINDOW_DIMS.width - cursor_x;
            cursor_y = globals::WINDOW_DIMS.height - cursor_y;

            if (quit_button.is_in_bounds(glm::vec2(cursor_x, cursor_y))) break;
            if (resume_button.is_in_bounds(glm::vec2(cursor_x, cursor_y))) {
                paused = false;
                glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwSetCursorPos(this->window, cursor_x_paused, cursor_y_paused);
            }

            globals::mouse_just_clicked = false;
        }
    }
}
