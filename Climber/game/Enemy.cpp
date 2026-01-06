#include "Enemy.h"

Enemy::Enemy(const Position2& pos, float r)
{
}

const Circle& Enemy::GetCollision() const
{
	// TODO: return ステートメントをここに挿入します
	return m_collision;
}

bool Enemy::IsDead() const
{
	return m_isDead;
}

void Enemy::OnDead()
{
	
}
