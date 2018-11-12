FLAGS=-O3 -W -Wall -Wextra -Werror -std=c++17 -fopenmp

rt:
	g++ $(FLAGS) -o rt raytracer.cpp ply_reader.cpp tga_saver.cpp

clean:
	rm -f rt
	rm -f rt.exe
