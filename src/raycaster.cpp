#include "pch.h"

#include "maze.h"
#include "raycaster.h"

RayCaster RayCaster::ms_instance;

const static float kFOV = 60.0f;
const static Float2 kProjectionPlane = { 320.0f, 200.0f };

void RayCaster::Execute(const RayCasterInfo& info) {
	const Maze* pMaze = info.pMaze;
	const Grid* pGrid = pMaze->GetGrid();

	const float depth = (pMaze->GetSize().x * pMaze->GetSize().x) * (pMaze->GetSize().y * pMaze->GetSize().y);
	float startAngle = Rad2Deg(info.sourceAngle) - kFOV * 0.5f;
	for (int32_t x = 0; x < kProjectionPlane.x; x++) {
		const float rayAngle = Deg2Rad(startAngle + (kFOV / kProjectionPlane.x) * x);
		const Float2 direction = { cosf(rayAngle), sinf(rayAngle) };

		float distanceToWall = 0.0f;
		bool hitWall = false;
		bool boundry = false;
		while (!hitWall && (distanceToWall * distanceToWall) < depth) {
			distanceToWall += 0.1f;

			Int2 test = (Int2)(info.sourcePosition + (direction * distanceToWall)) / pMaze->GetSizePerTile();
			if (test.x < 0 || test.x >= pMaze->GetNumTiles().x || test.y < 0 || test.y >= pMaze->GetNumTiles().y) {
				hitWall = true;
				break;
			} else {
				if (pGrid->GetState(test) == GridState::Collision) {
					hitWall = true;
					break;
				}
			}
		}

		if (hitWall) {
			const Float2 hit = { info.sourcePosition.x + (direction.x * distanceToWall), info.sourcePosition.y + (direction.y * distanceToWall) };
			for (auto iter : m_rayHandlers) {
				iter->OnHit({ info.sourcePosition, direction, distanceToWall });
			}
		}
	}
}
