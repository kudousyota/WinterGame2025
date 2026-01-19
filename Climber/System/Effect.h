#pragma once
#include "Actor.h"
class Effect: public Actor 
{
public:
	Effect(const Position2& pos);
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	void OnHit(const Actor& actor)override{}
	bool IsDead() const;
protected:
	bool m_isDead = false;

};

