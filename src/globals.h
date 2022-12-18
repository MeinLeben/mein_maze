#pragma once

const static std::string kTitle = "Mein Maze";

const static int32_t kWindowWidth = 640;
const static int32_t kWindowHeight = 640;

const static int32_t kGridSize = 10;
const static uint32_t kMazeWeight = 2;

enum GridState {
	Empty = 0,
	Collision,
	Path
};

struct Int2 {
	int32_t x, y;
};

struct Int3 {
	int32_t x, y, z;
};