#pragma once
#include "Rect.h"
#include <vector>
#include "Vec2.h"
#include <memory>
class Player;
class Rabbit;
class Bat;
class Stage;
class CollisionManager
{
public:
	//全ての当たり判定をチェックする
	static int CheckCollisions(
		std::shared_ptr<Player>& m_pPlayer,
		std::vector<std::shared_ptr<Rabbit>>& m_pRabbits,
		std::vector<std::shared_ptr<Bat>>& m_pBats,
		std::shared_ptr<Stage>& m_pStage
	);

};

