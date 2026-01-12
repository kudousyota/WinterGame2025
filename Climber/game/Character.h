#pragma once
#include "../System/Rect.h"
#include "../System/Vec2.h"

class Rect;
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

	Rect& GetRect();
	const Rect& GetRect() const;
	const Vec2& GetPos()const { return m_pos; }

protected:
	//当たり判定用
	Rect m_rect; 
	Vec2 m_pos;
	/// <summary>
	/// マップチップとの当たり判定
	/// </summary>
	void CheckHitMap(Rect chipRect);

private:


};