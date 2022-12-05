#include "pch.h"

#include "maze.h"

int32_t main(int32_t argc, char* argv[]) {
	std::unique_ptr<MeinMaze> maze = std::make_unique<MeinMaze>();
	return maze->Run();
}
