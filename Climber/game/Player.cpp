#include "Player.h"
#include "DxLib.h"
#include "Camera.h"
#include "Enemy.h"
#include "Input.h"

namespace
{
	constexpr float  kJumpPower		= 15.0f;
	constexpr float  kHighJumpPower = 20.0f;
	constexpr float  kGravity = 0.3f;
}
Player::Player():
m_speed(0.0f),
m_vel(0.0f),
m_IdleHandle(-1),
m_JumpHandle(-1),
m_FallHandle(-1),
m_RunHandle(-1),
m_IdleFrameMax(0),
m_RunFrameMax(0),
m_JumpFrameMax(0),
m_FallFrameMax(0),
m_cutX(0),
m_cutY(0),
m_cutW(0),
m_cutH(0),
m_switchSpeed(0.0f),
m_pos(0,0),
m_x(0),
m_onGround(false),
m_isLeft(false),
m_frameCount(0)

{
	//画像の読み込み
	m_IdleHandle = LoadGraph("data/IdlePlayer.png");
	m_JumpHandle = LoadGraph("data/JumpPlayer.png");
	m_FallHandle = LoadGraph("data/FallPlayer.png");
	m_RunHandle = LoadGraph("data/RunPlayer.png");
	m_pCamera = std::make_shared<Camera>();
	m_pInput = std::make_shared<Input>();
	
}

Player::~Player()
{

}
void Player::Init()
{
	//当たり判定用の矩形を初期化//位置とサイズを指定
	m_rect.Init(250.0f, 3000.0f, 30.0f, 30.0f);
	m_vel = 0.0f;
	//アニメーション初期化
	m_cutW = 32;
	m_cutH = 32;
	m_speed = 2.0f;
	m_onGround = false;
	m_animState = AnimState::Idle;
	m_switchSpeed = 0;
	m_frameCount = 0;
	//アニメーション速度設定
	m_IdleFrameMax = 11;
	m_RunFrameMax = 12;
	m_JumpFrameMax = 1;
	m_FallFrameMax = 1;


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

	// Input を毎フレーム更新
	m_pInput->Update();

	m_pos = { m_rect.GetX(), m_rect.GetY() };
	

	// 左右移動（Input を使用）
	if (m_pInput->IsPressed("left"))
	{
		m_rect.SetX(m_rect.GetX() - m_speed);
		m_isLeft = true;
	}
	if (m_pInput->IsPressed("right"))
	{
		m_rect.SetX(m_rect.GetX() + m_speed);
		m_isLeft = false;
	}

	//重力
	m_vel += kGravity;
	//衝突判定
	Rect chipRect;
	// ジャンプ処理（地面にいるときだけ、Input を使用）
	if (m_pInput->IsTriggered("Jump") && m_onGround)  // "Jump" = Z キー
	{
		m_vel = -kJumpPower;
		m_onGround = false;

		m_animState = AnimState::Jump; // ここでジャンプ状態へ
		m_animRow = 2;                 // スプライトのジャンプ行
		m_frameCount = 0;              // 切り替え直後にフレームをリセット

		m_switchSpeed = 0;
		m_cutX = 0;
		m_cutY = 0;


	}
	else if (m_pInput->IsTriggered("HighJump") && m_onGround)  // "HighJump" = X キー
	{
		m_vel = -kHighJumpPower;
		m_onGround = false;

		m_animState = AnimState::Jump; 
		m_animRow = 2;              
		m_frameCount = 0;        

		m_switchSpeed = 0;
		m_cutX = 0;
		m_cutY = 0;


	}
	//Y座標の更新
	m_rect.SetY(m_rect.GetY() + m_vel);
	

	if (!m_onGround) {
		if (m_vel < 0.0f) {
			// 上昇中：ジャンプ行
			m_animState = AnimState::Jump;
			m_animRow = 2;
		}
		else {
			// 落下中：落下行
			m_animState = AnimState::Fall;
			m_animRow = 3;
		}
	}
	else {
		// 地面にいる
		if (m_pInput->IsPressed("left") || m_pInput->IsPressed("right")) {
			m_animState = AnimState::Run;
			m_animRow = 1;
		}
		else {
			m_animState = AnimState::Idle;
			m_animRow = 0;
		}
	}


	m_frameCount++;
	// 5フレームごとに進める
	if (m_frameCount >= 5)
	{
		m_frameCount = 0;
		// アニメーションのフレームを進める
		switch (m_animState) 
		{
			//待機
		case AnimState::Idle:
			m_switchSpeed++;
			if (m_switchSpeed >= m_IdleFrameMax) m_switchSpeed = 0;
			break;
			//走り
		case AnimState::Run:
			m_switchSpeed++;
			if (m_switchSpeed >= m_RunFrameMax) m_switchSpeed = 0;
			break;
			//ジャンプ
		case AnimState::Jump:
			// ジャンプが1枚だから常に0
			m_switchSpeed = 0;
			break;
			//落下
		case AnimState::Fall:
			// 落下が1枚絵だから常に0
			m_switchSpeed = 0;
			break;
		}
	}

	//切り抜き座標の決定
	m_cutY = 0;
	m_cutX = m_switchSpeed * m_cutW;

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

	//描画する画像ハンドルを決定
	int handle = m_IdleHandle;
	switch (m_animState)
	{
	case AnimState::Run:  handle = m_RunHandle;  break;
	case AnimState::Jump: handle = m_JumpHandle; break;
	case AnimState::Fall: handle = m_FallHandle; break;
	case AnimState::Idle: default: handle = m_IdleHandle; break;
	}


	DrawRectRotaGraph(
		centerX, centerY,            // 画面の中心位置
		m_cutX, m_cutY,              // 切り抜き開始位置
		m_cutW, m_cutH,              // 切り抜きサイズ
		1.0f,                        // 拡大率
		0.0f,                        // 回転角度
		handle,                      // 画像ハンドル
		true,                        // 透過あり
		m_isLeft                     // 左右反転
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


