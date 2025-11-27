#include "CollisionManager.h"

void CollisionManager::CheckCollisions(std::shared_ptr<Player> m_pPlayer, std::shared_ptr<Enemy> m_pEnemy)
{
	Rect player;
	Rect enemy;

	player.Init(250.0f, 500.0f, 50.0f, 50.0f);
	enemy.Init(500.0f, 500.0f, 50.0f, 50.0f);

	enemy.GetX();
	enemy.GetY();

	if (player.IsHit(enemy))
	{
		enemy.FixPos(player);
	}
	
	
}
