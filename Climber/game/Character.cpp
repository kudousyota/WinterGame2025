#include "Character.h"

Character::Character()
{
}

Character::~Character()
{
}
void Character::Init()
{
}
void Character::Update()
{
}
void Character::Draw()
{
}

bool Character::IsHit(const Character& other) const
{
	return m_rect.IsHit(other.m_rect);
}

Vec2 Character::FixPos(const Character& other)
{
	return m_rect.FixPos(other.m_rect);
}

Rect Character::GetRect() const
{
	return m_rect;
}

void Character::CheckHitMap(Rect chipRect)
{
	
}
