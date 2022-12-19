#include "pch.h"

#include "sdl_helpers.h"

static void FillBottomFlatTriangle(SDL_Renderer* pRenderer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3) {
	const float invslope1 = float(x2 - x1) / float(y2 - y1);
	const float invslope2 = float (x3 - x1) / float(y3 - y1);

	float curx1 = x1;
	float curx2 = x1 + 0.5f;
	for (int32_t scanlineY = y1; scanlineY <= y2; scanlineY++) {
		SDL_RenderDrawLine(pRenderer, (int32_t)curx1, scanlineY, (int32_t)curx2, scanlineY);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

static void FillTopFlatTriangle(SDL_Renderer* pRenderer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3) {
	const float invslope1 = float(x3 - x1) / float(y3 - y1);
	const float invslope2 = float(x3 - x2) / float(y3 - y2);

	float curx1 = x3;
	float curx2 = x3 + 0.5f;

	for (int32_t scanlineY = y3; scanlineY > y1; scanlineY--) {
		SDL_RenderDrawLine(pRenderer, (int32_t)curx1, scanlineY, (int32_t)curx2, scanlineY);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void SDL_RenderFillTriangle(SDL_Renderer* pRenderer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3) {
	if (y1 > y2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	if (y1 > y3) {
		std::swap(x1, x3);
		std::swap(y1, y3);
	}

	if (y2 > y3) {
		std::swap(x2, x3);
		std::swap(y2, y3);
	}

	if (y2 == y3) {
		FillBottomFlatTriangle(pRenderer, x1, y1, x2, y2, x3, y3);
	} else if (y1 == y2) {
		FillTopFlatTriangle(pRenderer, x1, y1, x2, y2, x3, y3);
	} else
	{
		int32_t x4 = (int32_t)(x1 + ((float)(y2 - y1) / (float)(y3 - y1)) * (x3 - x1));
		int32_t y4 = y2;
		FillBottomFlatTriangle(pRenderer, x1, y1, x2, y2, x4, y4);
		FillTopFlatTriangle(pRenderer, x2, y2, x4, y4, x3, y3);
	}
}

void SDL_RenderFillCircle(SDL_Renderer* pRenderer, int32_t x, int32_t y, int32_t radius) {
	for (int32_t width = 0; width < radius * 2; width++) {
		for (int32_t height = 0; height < radius * 2; height++) {
			int32_t dx = radius - width;
			int32_t dy = radius - height;
			if ((dx * dx + dy * dy) <= (radius * radius)) {
				SDL_RenderDrawPoint(pRenderer, x + dx, y + dy);
			}
		}
	}
}