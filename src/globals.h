#pragma once

const static float PI = 3.14159265359f;
const static int32_t kGridSize = 10;
const static uint32_t kMazeWeight = 2;

const static float kPlayerWalkSpeed = 50.0f;
const static float kPlayerRunSpeed = 100.0f;
const static float kPlayerRotationSpeed = 3.14f;

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

struct Float2 {
	float x, y;
};

inline float Deg2Rad(float deg) {
	return deg * PI / 180.0f;
}

inline float Rad2Deg(float rad) {
	return rad * 180.0f / PI;
}
