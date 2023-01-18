#pragma once

const static float PI = 3.14159265359f;
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
	Int2() = default;

	Int2(int32_t x, int32_t y) 
		: x(x)
		, y(y) {
	}

	Int2(const struct Float2& other);

	int32_t x, y;
};

static Int2 operator/(const Int2& lhs, const Int2& rhs) {
	return {lhs.x / rhs.x, lhs.y / rhs.y};
}

static Int2 operator/(const Int2& lhs, int32_t rhs) {
	return {lhs.x / rhs, lhs.y / rhs};
}

static Int2 operator*(const Int2& lhs, const Int2& rhs) {
	return {lhs.x * rhs.x, lhs.y * rhs.x};
}

struct Int3 {
	int32_t x, y, z;
};

struct Float2 {
	Float2() = default;
	Float2(float x, float y) 
		: x(x)
		, y(y) {
	}

	Float2(const struct Int2& other);

	float x, y;
};

static Float2 operator+(const Float2& lhs, const Float2& rhs) {
	return {lhs.x + rhs.x, lhs.y + rhs.y};
}

static Float2 operator*(const Float2& lhs, float rhs) {
	return {lhs.x * rhs, lhs.y * rhs};
}

static Float2 operator/(const Float2& lhs, float rhs) {
	return {lhs.x / rhs, lhs.y / rhs};
}

inline float Deg2Rad(float deg) {
	return deg * PI / 180.0f;
}

inline float Rad2Deg(float rad) {
	return rad * 180.0f / PI;
}
