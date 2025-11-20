#include "Player.h"
#include "DxLib.h"
#include "Camera.h"


namespace
{
	constexpr float  kMoveSpeed = 2.0f;
	constexpr float  kJumpPower = 10.0f;
	constexpr float  kHighJumpPower = 20.0f;
	constexpr float  kGravity = 0.5f;
	constexpr float  kGroundY = 640.0f;
}
Player::Player():
m_vel(0.0f),
m_Handle(-1),
m_CutX(0),
m_CutY(0),
m_CutW(0),
m_CutH(0),
m_switchSpeed(0.0f),
m_pos(0,0),
m_x(0),
m_drawOffset(0,0),
m_frameCount(0)

{
	//画像の読み込み
	m_Handle = LoadGraph("data/Player.png");
	m_pCamera = std::make_shared<Camera>();
	
}

Player::~Player()
{

}
void Player::Init()
{
	//当たり判定用の矩形を初期化 //これでも行ける
	m_rect.Init(250.0f, 500.0f, 50.0f, 50.0f);
	m_vel = 0.0f;
	//アニメーション初期化
	m_CutW = 32;
	m_CutH = 32;
	
	

}
void Player::Update()
{
	m_pos = { m_rect.GetX(), m_rect.GetY() };
	// アニメーション更新
	m_frameCount++;
	// 5フレームごとに切り替え
	if (m_frameCount >= 5)
	if (m_frameCount >= 5)
	{ 
		m_frameCount = 0;
		m_switchSpeed++;
		// 横に11フレーム
		if (m_switchSpeed >= 11)
		{  
			m_switchSpeed = 0;
		}
		m_CutX = m_switchSpeed * m_CutW; // 横方向の切り抜き位置
		m_CutY = 0;                     // 縦方向は固定
	}



	//移動
	if (CheckHitKey(KEY_INPUT_A))
	{
		m_rect.SetX(m_rect.GetX() - kMoveSpeed);
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		m_rect.SetX(m_rect.GetX() + kMoveSpeed);
	}

	//重力
	m_vel += kGravity;
	
	//地面にいるか
	bool isHitGround = (m_rect.GetY() >= kGroundY);
	if (isHitGround)
	{
		m_vel = 0;
		//地面にいる
		m_rect.SetY(kGroundY) ;
	}
	//地面にいる時ジャンプできる
	if (CheckHitKey(KEY_INPUT_SPACE)&&isHitGround)
	{
		m_vel -= kJumpPower;
	}

	else if (CheckHitKey(KEY_INPUT_Z)&&isHitGround)
	{
		m_vel -= kHighJumpPower;
	}
	//Y座標の更新
	m_rect.SetY(m_rect.GetY() + m_vel);
	
}
void Player::Draw(const Camera& camera)
{

	auto leftTop = static_cast<int>(m_rect.GetX() - m_rect.GetW() * 0.5f);
	auto leftBottom = static_cast<int>(m_rect.GetY() - m_rect.GetH() * 0.5f);
	auto rightTop = static_cast<int>(m_rect.GetX() + m_rect.GetW() * 0.5f);
	auto rightBottom = static_cast<int>(m_rect.GetY() + m_rect.GetH() * 0.5f);

	m_drawOffset = m_pCamera->GetCameraOffset() ;
	
	DrawRectGraph(
		leftTop + static_cast<int>(m_drawOffset.x),   // 描画位置X
		leftBottom + static_cast<int>(m_drawOffset.y),// 描画位置Y
		m_CutX, m_CutY,                                    // 切り抜き開始位置
		m_CutW, m_CutH,                                    // 切り抜きサイズ
		m_Handle,                                          // 画像
		true
	);
	
	
}


