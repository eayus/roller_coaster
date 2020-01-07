all: bin/main

run: bin/main
	bin/main

clean:
	rm *.o
	rm src/*.gch
	rm bin/main


bin/main: main.o shader_program.o application.o camera.o roller_coaster.o cart.o cart_camera.o
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
