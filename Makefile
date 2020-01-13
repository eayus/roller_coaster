all: bin/main

run: bin/main
	bin/main

clean:
	rm *.o
	rm src/*.gch
	rm bin/main


bin/main: main.o shader_program.o application.o camera.o roller_coaster.o cart.o cart_camera.o globals.o free_camera.o ground.o textures.o screen_texture_renderer.o virtual_screen.o font.o text.o button.o shadow_map.o
	clang++ $^ -o bin/main -lGL -lglfw -lGLEW -lboost_filesystem -g -std=c++17

main.o: src/main.cpp
	clang++ -std=c++17 -I src -I libs -c $^ -g

application.o: src/application.cpp
	clang++ -std=c++17 -I src -I libs -c $^ -g

shader_program.o: src/shader_program.cpp
	clang++ -std=c++17 -I src -I libs -c $^ -g

camera.o: src/camera.cpp
	clang++ -std=c++17 -I src -I libs -c $^ -g

models.o: src/models.cpp
	clang++ -std=c++17 -I src -I libs -c $^ -g

roller_coaster.o: src/roller_coaster.cpp src/roller_coaster.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

cart.o: src/cart.cpp src/cart.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

cart_camera.o: src/cart_camera.cpp src/cart_camera.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

globals.o: src/globals.cpp src/globals.cpp
	clang++ -std=c++17 -I src -I libs -c $< -g

free_camera.o: src/free_camera.cpp src/free_camera.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

ground.o: src/ground.cpp src/ground.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

textures.o: src/textures.cpp
	clang++ -std=c++17 -I src -I libs -c $< -g

screen_texture_renderer.o: src/screen_texture_renderer.cpp src/screen_texture_renderer.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

virtual_screen.o: src/virtual_screen.cpp src/virtual_screen.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

font.o: src/ui/font.cpp src/ui/font.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

text.o: src/ui/text.cpp src/ui/text.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

button.o: src/ui/button.cpp src/ui/button.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g

shadow_map.o: src/shadow_map.cpp src/shadow_map.hpp
	clang++ -std=c++17 -I src -I libs -c $< -g
