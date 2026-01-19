#pragma once
#include "Effect.h"

class PersonaEffect:public Effect
{
public:
	PersonaEffect(const Position2& pos, int handle);
	~PersonaEffect();
	void Init();
	void Update() override;
	void Draw() override;
	bool IsDead()const;
protected:
	bool m_isDead = false;
private:
	Position2 m_pos;
	int m_handle;
	int m_timer;
	bool m_dead;

};

