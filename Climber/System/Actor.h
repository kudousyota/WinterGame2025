#pragma once
#include "Geometry.h"
#include "../System/Camera.h"

class Camera;
//配置可能のゲームオブジェクトの基底クラス
class Actor
{
protected:
	Position2 m_pos;
public:
	virtual void Update() = 0;
	virtual void Draw(const Vec2& camraOffset) = 0;
	virtual void OnHit(const Actor& actor) = 0;
	virtual ~Actor() {};
	const Position2& GetPos() const;
};


