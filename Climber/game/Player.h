#pragma once
#include "Character.h"
#include "Vec2.h"
#include <memory>

class Camera;
class Enemy;
class CollisionManager;
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
	 void Update(const Enemy& enemy, Rect& other);
	 void Draw(const Camera& camera);


	 Vec2 GetPos() { return m_pos ; }



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
	//カメラのオフセット値
	Vec2 m_drawOffset;
	////拡大率
	//float m_scaleX;
	//float m_scaleY;
	////角度
	//int m_Angle;

};

