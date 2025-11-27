#pragma once
#include "Rect.h"
#include "Vec2.h"

class Character
{
public:
	Character();
	virtual ~Character();
	void Init();
	void Update();
	void Draw();

	bool IsHit(const Character& other) const;
	Vec2 FixPos(const Character& other);

	Rect GetRect() const;
	const Vec2& GetPos()const { return m_pos; }

protected:
	Rect m_rect; //“–‚½‚è”»’è—p
	Vec2 m_pos;

private:
	

};

