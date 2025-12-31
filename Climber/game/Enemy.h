#pragma once
#include "Character.h"
#include "Vec2.h"
#include <memory>
class Camera;
class Player;
class CollisionManager;
class Enemy : public Character
{
public:
	Enemy();
	virtual~Enemy();
	void Init();
	void Update(const Player& player);
	void Draw(const Camera& camera);


	Vec2 GetPos() { return m_pos; }

	//縦速度の取得
	float GetVelY() const { return m_vel; }
	void SetVelY(float vel) { m_vel = vel; }

	// 横速度の取得
	float GetVelX() const { return m_speed; }
	void SetVelX(float speed) { m_speed = speed; }

	// 地面フラグ操作
	bool IsOnGround() const { return IsonGround; }
	void SetOnGround(bool onGround) { IsonGround = onGround; }


private:
	Vec2 m_pos;

	float m_vel;
	int m_Handle;

	int m_cutX;
	int m_cutY;
	int m_cutW;
	int m_cutH;
	int m_speed;
	float m_frameCount;
	float m_switchSpeed;
	bool IsonGround = false;


	bool isHit(const Player& player);

	std::shared_ptr<Camera>m_pCamera;

};

