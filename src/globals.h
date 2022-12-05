#pragma once

const static int32_t kWindowWidth = 1280;
const static int32_t kWindowHeight = 1280;

const static int32_t kGridSize = 320;
const static uint32_t kMazeWeight = 2;

enum GridNode {
	Empty = 0,
	Collision,
	Path
};

struct Int2 {
	int32_t x, y;
};
