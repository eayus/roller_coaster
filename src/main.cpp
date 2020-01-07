#include <iostream>
#include <application.hpp>

int main() {
    std::cout << "hello world!" << std::endl;

    Application app;
    app.main_loop();

    return 0;
}
