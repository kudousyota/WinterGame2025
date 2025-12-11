#include "Player.h"
#include "DxLib.h"
#include "Camera.h"
#include "Enemy.h"


namespace
{
	constexpr float  kJumpPower = 10.0f;
	constexpr float  kHighJumpPower = 20.0f;
	constexpr float  kGravity = 0.5f;
	constexpr float  kGroundY = 1520.0f;
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

}
void Player::Update(const Enemy& enemy, Rect& other,const Bg& bg)
{
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



	//移動
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
	//地面にいるか
	bool isHitGround = (m_rect.GetY() >= kGroundY);
	if (isHitGround)
	{
		m_vel = 0;
		//地面にいる//ここに床の上に乗せる処理
		m_rect.SetY(kGroundY);
	}
	//地面にいる時ジャンプできる
	if (CheckHitKey(KEY_INPUT_SPACE)&&isHitGround)
	{
		m_vel -= kJumpPower;
	}
	//ハイジャンプ
	else if (CheckHitKey(KEY_INPUT_Z)&&isHitGround)
	{
		m_vel -= kHighJumpPower;
	}
	//Y座標の更新
	m_rect.SetY(m_rect.GetY() + m_vel);
	
	//押し出し処理
	if (m_rect.IsHit(enemy.GetRect()))
	{
		//押し出し量を取得//FixPosが押し出し量を返すようにしたい
		Vec2 receive = m_rect.FixPos(enemy.GetRect());
		//Y方向の押し出し量を反映
		m_rect.FixPos(enemy.GetRect());
		
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


