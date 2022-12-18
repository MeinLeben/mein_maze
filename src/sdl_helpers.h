#pragma once

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
