#pragma once
#include "Rect.h"
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
		std::shared_ptr<Player>m_pPlayer,
		std::shared_ptr<Rabbit>m_pRabbit,
		std::shared_ptr<Bat>m_pBat,
		std::shared_ptr<Stage>m_pStage
	);

};

