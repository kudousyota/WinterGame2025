#include "Enemy.h"
#include "Dxlib.h"
#include "Camera.h"
namespace
{
	constexpr float kGravity = 0.5;
	constexpr float kGroundY = 640.0f;
}

Enemy::Enemy():
	m_vel(0.0f),
	m_Handle(-1),
	m_CutX(0),
	m_CutY(0),
	m_CutW(0),
	m_CutH(0),
	m_switchSpeed(0.0f),
	m_pos(0, 0),
	m_drawOffset(0, 0),
	m_frameCount(0)
{
	//画像の読み込み
	m_Handle = LoadGraph("data/Enemy.png");
	m_pCamera = std::make_shared<Camera>();
}

Enemy::~Enemy()
{
}
void Enemy::Init()
{
	m_rect.Init(500.0f, 500.0f, 50.0f, 50.0f);
	m_vel = 0.0f;
	//アニメーション
	m_CutW = 34;
	m_CutH = 40;

}
void Enemy::Update()
{
	m_pos = { m_rect.GetX(),m_rect.GetY() };
	//アニメーションの更新
	m_frameCount++;
	if (m_frameCount >= 5)
	{
		m_frameCount = 0;
		m_switchSpeed++;
		//横にずらす
		if (m_switchSpeed >= 5)
		{
			m_switchSpeed = 0;
		}
		m_CutX = m_switchSpeed * m_CutW;//切り取りいち
		m_CutY = 0;
	}
	//重力
	m_vel += kGravity;

	//地面にいるか
	bool isHitGround = (m_rect.GetY() >= kGroundY);
	if (isHitGround)
	{
		m_vel = 0;
		//地面にいる
		m_rect.SetY(kGroundY);
	}
	m_rect.SetY(m_rect.GetY() + m_vel);

}
void Enemy::Draw(const Camera& camera)
{
	auto leftTop = static_cast<int>(m_rect.GetX() - m_rect.GetW() * 0.5f);
	auto leftBottom = static_cast<int>(m_rect.GetY() - m_rect.GetH() * 0.5f);
	auto rightTop = static_cast<int>(m_rect.GetX() + m_rect.GetW() * 0.5f);
	auto rightBottom = static_cast<int>(m_rect.GetY() + m_rect.GetH() * 0.5f);

	m_drawOffset = camera.GetCameraOffset();

	DrawRectGraph(
		leftTop + static_cast<int>(m_drawOffset.x),   // 描画位置X
		leftBottom + static_cast<int>(m_drawOffset.y),// 描画位置Y
		m_CutX, m_CutY,                                    // 切り抜き開始位置
		m_CutW, m_CutH,                                    // 切り抜きサイズ
		m_Handle,                                          // 画像
		true
	);
}