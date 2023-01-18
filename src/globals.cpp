#include "globals.h"

Int2::Int2(const Float2& other) 
	: x(static_cast<int32_t>(other.x))
	, y(static_cast<int32_t>(other.y)) {
}

Float2::Float2(const Int2& other)
	: x(static_cast<float>(other.x))
	, y(static_cast<float>(other.y)) {
}
