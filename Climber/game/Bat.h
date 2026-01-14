#pragma once
#include "Character.h"
#include "../System/Vec2.h"
#include <memory>
#include "../System/Geometry.h"

class Camera;
class Player;
class CollisionManager;
class EnemyFactory;

class Bat : public Character
{
public:
	Bat();
	// Factory からハンドルと位置を渡して生成できるようにオーバーロード追加
	Bat(int handle, const Position2& pos);

	virtual ~Bat();

	void Init();

	void Update(const Player& player);

	void Draw(const Camera& camera,const Player& player);
	Vec2 GetPos() { return m_pos; }
	Vec2 SetPos(const Vec2& pos)
	{
		m_pos = pos;
		m_rect.SetX(pos.x);
		m_rect.SetY(pos.y);
		return m_pos;
	}

	//縦速度の取得
	float GetVelY() const { return m_vel; }
	void SetVelY(float vel) { m_vel = vel; }

	// 横速度の取得
	float GetVelX() const { return m_speed; }
	void SetVelX(float speed)
	{
		m_speed = speed;
		// 速度の符号で向きフラグを更新（0 のときは現状維持）
		if (speed < 0.0f) IsLeft = false;
		else if (speed > 0.0f) IsLeft = true;
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

	int m_cutX;
	int m_cutY;
	int m_cutW;
	int m_cutH;
	int m_speed;
	float m_frameCount;
	float m_switchSpeed;
	bool IsLeft;
	bool IsonGround = false;


	bool isHit(const Player& player);

	std::shared_ptr<Camera>m_pCamera;

	// Factory 経由で渡されたハンドルを所有しないためのフラグ
	bool m_ownsHandle = true;
};