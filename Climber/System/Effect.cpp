#include "Effect.h"

Effect::Effect(const Position2& pos)
{
	m_pos = pos;
}

void Effect::Update()
{
}

void Effect::Draw(const Vec2& camraOffset)
{
}

bool Effect::IsDead() const
{
	return m_isDead;
}