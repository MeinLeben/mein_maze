#pragma once

const static int32_t kWindowWidth = 640;
const static int32_t kWindowHeight = 640;

const static int32_t kGridSize = 160;
const static uint32_t kMazeWeight = 2;

enum GridState {
	Empty = 0,
	Collision,
	Path
};

struct Int2 {
	int32_t x, y;
};
