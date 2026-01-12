#pragma once
#include "Character.h"
#include "../System/Vec2.h"
#include <memory>
#include "../System/Geometry.h"

class Camera;
class Player;
class CollisionManager;
class EnemyFactory;
class Rabbit : public Character
{
public:
	Rabbit();
	// EnemyFactoryからhandleと位置を渡して生成できるようにオーバーロード追加
	Rabbit(int handle, const Position2& pos);

	virtual~Rabbit();
	void Init();
	
	void Update(const Player& player);
	void Draw(const Camera& camera);


	Vec2 GetPos() { return m_pos; }

	//縦速度の取得
	float GetVelY() const { return m_vel; }
	void SetVelY(float vel) { m_vel = vel; }

	// 横速度の取得
	float GetVelX() const { return m_speed; }
	void SetVelX(float speed)
	{
		m_speed = speed;
		// 速度の符号で向きフラグを更新（0 のときは現状維持）
		if (speed < 0.0f) isLeft = false;
		else if (speed > 0.0f) isLeft = true;
	}

	// 地面フラグ操作
	bool IsOnGround() const { return IsonGround; }
	void SetOnGround(bool onGround) { IsonGround = onGround; }

	void OnDead() { IsDead = true; }
	bool IsDead;

private:
	Vec2 m_pos;

	float m_vel;
	int m_Handle;
	int m_RunHandle;

	int m_cutX;
	int m_cutY;
	int m_cutW;
	int m_cutH;
	int m_speed;
	float m_frameCount;
	float m_switchSpeed;
	bool isLeft;
	bool IsonGround = false;


	bool isHit(const Player& player);
	

	std::shared_ptr<Camera>m_pCamera;

	// Renderer ハンドルの所有者フラグ（Factory 経由で渡されたハンドルは所有しない）
	bool m_ownsHandle = true;
};