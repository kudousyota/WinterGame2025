#pragma once
#include "Character.h"
#include "Vec2.h"
#include <memory>
class Camera;
class Player;
class Enemy : public Character
{
public:
	Enemy();
	virtual~Enemy();
	void Init();
	void Update(const Player& player);
	void Draw(const Camera& camera);

private:
	Vec2 m_pos;

	float m_vel;
	int m_Handle;

	int m_cutX;
	int m_cutY;
	int m_cutW;
	int m_cutH;
	float m_frameCount;
	float m_switchSpeed;

	bool isHit(const Player& player);

	std::shared_ptr<Camera>m_pCamera;

	Vec2 m_drawOffset;
};

