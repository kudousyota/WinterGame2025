#pragma once
#include "Rect.h"
#include <memory>
class Player;
class Rabbit;
class Stage;
class CollisionManager
{
public:
	//全ての当たり判定をチェックする
	static void CheckCollisions(
		std::shared_ptr<Player>m_pPlayer,
		std::shared_ptr<Rabbit>m_pEnemy,
		std::shared_ptr<Stage>m_pStage
	);

};

