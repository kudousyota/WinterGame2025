#pragma once
#include "Character.h"
#include "Vec2.h"
#include <memory>

class Camera;
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
	 void Update();
	 void Draw(const Camera& camera);

	 Vec2 GetPos() { return m_pos ; }

private:
	
	float m_x;
	Vec2 m_pos;

	float m_vel;

	//アニメーション
	int m_Handle;
	//カット位置
	int m_CutX;
	int m_CutY;
	//１フレームの高さ
	int m_CutW;
	int m_CutH;
	//フレームカウント
	float m_frameCount;
	//切り替え速度
	float m_switchSpeed;


	std::shared_ptr<Camera> m_pCamera;
	//カメラのオフセット値
	Vec2 m_drawOffset;
	////拡大率
	//float m_scaleX;
	//float m_scaleY;
	////角度
	//int m_Angle;

};

