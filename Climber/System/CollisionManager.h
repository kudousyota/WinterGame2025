#pragma once
#include "Rect.h"
#include <memory>
class Player;
class Enemy;
class CollisionManager
{
public:
	//全ての当たり判定をチェックする
	static void CheckCollisions(
		std::shared_ptr<Player>m_pPlayer,
		std::shared_ptr<Enemy>m_pEnemy
	);

};

