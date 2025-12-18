#include "Player.h"
#include "DxLib.h"
#include "Camera.h"
#include "Enemy.h"


namespace
{
	constexpr float  kJumpPower		= 10.0f;
	constexpr float  kHighJumpPower = 20.0f;
	constexpr float  kGravity = 0.5f;
}
Player::Player():
m_speed(0.0f),
m_vel(0.0f),
m_Handle(-1),
m_cutX(0),
m_cutY(0),
m_cutW(0),
m_cutH(0),
m_switchSpeed(0.0f),
m_pos(0,0),
m_x(0),
m_onGround(false),
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
	m_cutW = 32;
	m_cutH = 32;
	m_speed = 2.0f;
	m_onGround = false;

}
void Player::Update(const Enemy& enemy, Rect& other,const Bg& bg)
{
	//中心座標から上下左右の座標を計算
	const float halfW = m_rect.GetW() * 0.5f;
	const float halfH = m_rect.GetH() * 0.5f;
	//真ん中
	float centerY = m_rect.GetY();
	//下端Y座標
	float bottomY = centerY + halfH;

	m_pos = { m_rect.GetX(), m_rect.GetY() };
	// アニメーション更新
	m_frameCount++;
	// 5フレームごとに切り替え
	if (m_frameCount >= 5)
	{ 
		m_frameCount = 0;
		m_switchSpeed++;
		// 横に11フレーム
		if (m_switchSpeed >= 11)
		{  
			m_switchSpeed = 0;
		}
		m_cutX = m_switchSpeed * m_cutW; // 横方向の切り抜き位置
		m_cutY = 0;                     // 縦方向は固定
	}



	//左右移動
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		m_rect.SetX(m_rect.GetX() - m_speed);
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		m_rect.SetX(m_rect.GetX() + m_speed);
	}

	//重力
	m_vel += kGravity;
	//衝突判定
	Rect chipRect;
	// ジャンプ：地面にいるときだけ
	if (CheckHitKey(KEY_INPUT_SPACE) && m_onGround)
	{
		m_vel =- kJumpPower;
		m_onGround = false;
	}
	else if (CheckHitKey(KEY_INPUT_Z) && m_onGround)
	{
		m_vel =- kHighJumpPower;
		m_onGround = false;
	}
	//Y座標の更新
	m_rect.SetY(m_rect.GetY() + m_vel);
	
	// 敵との衝突判定（押し出しのみ、速度は触らない）
	if (m_rect.IsHit(enemy.GetRect()))
	{
		Vec2 push = m_rect.FixPos(enemy.GetRect());
		m_rect.SetX(m_rect.GetX() + push.x);
		m_rect.SetY(m_rect.GetY() + push.y);
		// 敵との衝突では速度は変更しない（CollisionManager で地面判定が有効化されるから）
	}
}
void Player::Draw(const Camera& camera)
{
	const Vec2 cameraOffset = camera.GetCameraOffset();

	//中心座標
	const int centerX = static_cast<int>(m_rect.GetX() + cameraOffset.x);
	const int centerY = static_cast<int>(m_rect.GetY() + cameraOffset.y);

	//左上、右上、左下、右下の座標計算
	const int halfW = static_cast<int>(m_rect.GetW() * 0.5f);
	const int halfH = static_cast<int>(m_rect.GetH() * 0.5f);

	const int left   = centerX - halfW;
	const int right  = centerX + halfW;
	const int Top    = centerY - halfH;
	const int Bottom = centerY + halfH;

	

	DrawRectRotaGraph(
		centerX, centerY,            // 画面の中心位置
		m_cutX, m_cutY,              // 切り抜き開始位置
		m_cutW, m_cutH,              // 切り抜きサイズ（例：32×32）
		1.0f,                        // 拡大率
		0.0f,                        // 回転角度
		m_Handle,                    // 画像ハンドル
		TRUE                         // 透過あり
	);

	
#ifdef _DEBUG
	//当たり判定の枠
	DrawBox(
		left,Top,
		right,Bottom,
		GetColor(255, 0, 0),
		false
	);
#endif
	
}
bool Player::isHit(const Enemy& enemy)
{
	
	return m_rect.IsHit(enemy.GetRect());
}


