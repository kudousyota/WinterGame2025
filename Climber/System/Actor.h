#pragma once
#include "Geometry.h"
//配置可能のゲームオブジェクトの基底クラス
class Actor
{
protected:
	Position2 m_pos;
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void OnHit(const Actor& actor) = 0;
	virtual ~Actor() {};
	const Position2& GetPos() const;
};


