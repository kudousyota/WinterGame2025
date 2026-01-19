#include "Effect.h"

Effect::Effect(const Position2& pos)
{
	m_pos = pos;
}

bool Effect::IsDead() const
{
	return m_isDead;
}