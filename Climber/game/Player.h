#pragma once
#include "Character.h"
#include "Vec2.h"
#include <memory>

class Bg;
class Camera;
class Enemy;
class CollisionManager;
class Input;
class Player : public Character
{
public:
	Player();
virtual~Player();

/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="x">X : X座標</param>
	/// <param name="y">Y : Y座標</param>
	/// <param name="w">W : 幅</param>
	/// <param name="h">H : 高さ</param>
	 void Init();
	 void Update(const Enemy& enemy, Rect& other, const Bg& bg);
	 void Draw(const Camera& camera);


	 Vec2 GetPos() { return m_pos ; }

	 //縦速度の取得
	 float GetVelY() const{ return m_vel; }
	 void SetVelY(float vel) { m_vel = vel; }

	 // 地面フラグ操作
	 bool IsOnGround() const { return m_onGround; }
	 void SetOnGround(bool onGround) { m_onGround = onGround; }


private:
	
	float m_x;
	Vec2 m_pos;

	float m_vel;
	float m_speed;

	//アニメーション
	int m_Handle;
	//カット位置
	int m_cutX;
	int m_cutY;
	//１フレームの高さ
	int m_cutW;
	int m_cutH;
	//フレームカウント
	float m_frameCount;
	//切り替え速度
	float m_switchSpeed;
	//当たり判定
	bool isHit(const Enemy& enemy);


	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<CollisionManager> m_pCollision;
	std::shared_ptr<Input> m_pInput;

	bool m_onGround = false;

	////拡大率
	//float m_scaleX;
	//float m_scaleY;
	////角度
	//int m_Angle;

};

