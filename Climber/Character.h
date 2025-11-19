#pragma once
#include "Rect.h"

class Character
{
public:
	Character();
	virtual ~Character();
	void Init();
	void Update();
	void Draw();

	bool IsHit(const Character& other) const;
	void FixPos(const Character& other);

	Rect GetRect() const;

protected:
	Rect m_rect; //“–‚½‚è”»’è—p

private:
	

};

