#include "Player.h"
#include "DxLib.h"
#include "../System/Camera.h"
#include "Enemy.h"
#include "Rabbit.h"
#include "../System/Input.h"
#include "../System/AnimationManager.h"
#include "../System/SoundManager.h"
#include <algorithm>

namespace
{
	constexpr float  kJumpPower = 12.0f;
	constexpr float  kHighJumpPower = 20.0f;
	constexpr float  kGravity = 0.3f;
}
Player::Player() :
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
	m_CutX(0),
	m_CutY(0),
	m_CutW(0),
	m_CutH(0),
	m_SwitchSpeed(0),
	m_pos(0, 0),
	m_x(0),
	m_brokeCount(0),
	m_highJumpUnlock(false),
	m_highJumpPoint(0),
	m_HighJumpMax(0),
	m_onGround(false),
	IsLeft(false),
	m_animState(AnimState::Idle),
	m_isHighJumpActive(false),
	m_HighJumpFrameMax(0),
	m_animRow(0),
	m_invincibleTime(0),
	m_PersonaHandle(-1),
	m_PersonaCutH(0),
	m_PersonaCutW(0),
	m_PersonaCutX(0),
	m_PersonaCutY(0),
	m_personaFollowAlpha(0.3f),
	m_personaOffsetX(24.0f),
	m_personaOffsetY(20.0f),
	m_PersonaPosX(0),
	m_PersonaPosY(0),
	m_isPersonaDraw(false),

	m_frameCount(0)

{
	//画像の読み込み
	m_IdleHandle		= LoadGraph("data/IdlePlayer.png");
	m_JumpHandle		= LoadGraph("data/JumpPlayer.png");
	m_FallHandle		= LoadGraph("data/FallPlayer.png");
	m_HighJumpHandle	= LoadGraph("data/HighJump.png");
	m_RunHandle			= LoadGraph("data/RunPlayer.png");
	m_HitHandle			= LoadGraph("data/HitPlayer.png");
	m_PersonaHandle		= LoadGraph("data/Persona.png");
	m_pCamera			= std::make_shared<Camera>();
	m_pInput			= std::make_shared<Input>();
	m_pAnimationManager = std::make_shared<AnimationManager>();

	//サウンドの読み込み
	
	

}

Player::~Player()
{

}
void Player::Init()
{
	//当たり判定用の矩形を初期化//位置とサイズを指定
	m_rect.Init(250.0f, 159800.0f, 30.0f, 30.0f);
	m_vel = 0.0f;
	//アニメーション初期化
	m_CutW = 32;
	m_CutH = 32;
	m_PersonaCutW = 96;
	m_PersonaCutH = 80;
	m_speed = 2.0f;
	m_highJumpUnlock = false;
	//解禁に必要なポイント設定
	m_highJumpPoint = 10;
	m_animState = AnimState::Idle;
	m_SwitchSpeed = 0;
	m_frameCount = 0;
	m_brokeCount = 0;
	//アニメーション速度設定
	m_IdleFrameMax = 11;
	m_RunFrameMax = 12;
	m_JumpFrameMax = 1;
	m_FallFrameMax = 1;
	m_HighJumpMax = 6;
	m_invincibleTime = 0;

	SoundManager::Load();

}

void Player::StartInvincible(int frame)
{
	// 無敵時間をフレーム数でセット
	if (frame <= 0)return;
	{
		m_invincibleTime = frame;
	}
}

void Player::TileBroke()
{
	m_brokeCount++;
	if (!m_highJumpUnlock && m_brokeCount >= m_highJumpPoint)
	{
		m_highJumpUnlock = true;

		m_pAnimationManager->Create({ m_rect.GetX(), m_rect.GetY() },
			EffectType::persona
		);
	}
}

bool Player::isHit(const Rabbit& enemy)
{
	return false;
}


void Player::Update(Rect& other, const Bg& bg)
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
		IsLeft = true;

		//SoundManager::PlaySE("run");
	}
	if (m_pInput->IsPressed("right"))
	{
		m_rect.SetX(m_rect.GetX() + m_speed);
		IsLeft = false;
		//SoundManager::PlaySE("run");
	}

	//重力
	m_vel += kGravity;
	// ジャンプ処理（地面にいるときだけ、Input を使用）
	if (m_pInput->IsTriggered("Jump") && m_onGround)  // "Jump" = Z キー
	{
		m_vel = -kJumpPower;
		m_onGround = false;

		m_animState = AnimState::Jump; // ここでジャンプ状態へ
		m_animRow = 2;                 // スプライトのジャンプ行
		m_frameCount = 0;              // 切り替え直後にフレームをリセット

		m_SwitchSpeed = 0;
		m_CutX = 0;
		m_CutY = 0;
		SoundManager::PlaySE("Jump");

	}
	// ハイジャンプ 解禁されていた場合のみジャンプ処理
	if (m_pInput->IsTriggered("HighJump") && m_onGround && m_highJumpUnlock)
	{
		m_vel = -kHighJumpPower;
		m_onGround = false;

		m_animState = AnimState::High;
		m_animRow = 2;
		m_frameCount = 0;
		m_SwitchSpeed = 0;
		m_CutX = 0;
		m_CutY = 0;
		// ハイジャンプは一度使うと解除
		m_highJumpUnlock = false;
		// ハイジャンプ処理中フラグを立てる
		m_isHighJumpActive = true;
		//ブロック破壊数リセット
		m_brokeCount = 0;
		SoundManager::PlaySE("HighJump");
	}

	//Y座標の更新
	m_rect.SetY(m_rect.GetY() + m_vel);


	if (!m_onGround)
	{
		if (m_isHighJumpActive)
		{
			//ハイジャンプの時アニメーションを変えない
		}
		else
		{


			if (m_vel < 0.0f)
			{
				// 上昇中：ジャンプ行
				m_animState = AnimState::Jump;
				m_animRow = 2;

			}
			else
			{
				// 落下中：落下行
				m_animState = AnimState::Fall;
				m_animRow = 3;
				// 上昇中はハイジャンプ処理終了
				m_isHighJumpActive = false;
			}
		}
		if (m_vel >= 0.0f)
		{
			m_isHighJumpActive = false;
		}
		
	}
	else
	{
		// 地面にいる
		if (m_pInput->IsPressed("left") || m_pInput->IsPressed("right"))
		{
			m_animState = AnimState::Run;
			m_animRow = 1;
		}
		else
		{
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
			m_SwitchSpeed++;
			if (m_SwitchSpeed >= m_IdleFrameMax) m_SwitchSpeed = 0;
			break;
			//走り
		case AnimState::Run:
			m_SwitchSpeed++;
			// 走りアニメの最大フレーム数でループ
			if (m_SwitchSpeed >= m_RunFrameMax) m_SwitchSpeed = 0;
			break;
			//ジャンプ
		case AnimState::Jump:
			// ジャンプが1枚だから常に0
			m_SwitchSpeed = 0;
			break;
			//落下
		case AnimState::Fall:
			// 落下が1枚絵だから常に0
			m_SwitchSpeed = 0;
			break;
			
		case AnimState::Hit:
			m_SwitchSpeed = 3;
			break;
		case AnimState::High:
			m_SwitchSpeed++;
			if (m_SwitchSpeed >= 6) m_SwitchSpeed = 0;
			break;
		case AnimState::Persona:
			m_SwitchSpeed++;
			if (m_SwitchSpeed >= 14)
			{
				m_SwitchSpeed = 0;
			}

		}
	}

	//切り抜き座標の決定
	m_CutY = 0;
	m_CutX = m_SwitchSpeed * m_CutW;
	m_PersonaCutY = 0;
	m_PersonaCutX = m_SwitchSpeed * m_PersonaCutW;
	

	//無敵時間のカウントダウン
	if (m_invincibleTime > 0)
	{
		--m_invincibleTime;
	}
	//ペルソナの位置計算
	float personaX = m_rect.GetX();
	float personaY = m_rect.GetY();

	//背中に追従
	float offsetX = (IsLeft ? +m_personaOffsetX : -m_personaOffsetX);
	// 少し上にオフセット
	float offsetY = -m_personaOffsetY;
	// 目標位置
	float targetX = personaX + offsetX;
	float targetY = personaY + offsetY;

	// 解禁された瞬間はスナップワープ防止
	if (m_highJumpUnlock && !m_isPersonaDraw)
	{
		m_PersonaPosX = targetX;
		m_PersonaPosY = targetY;
		m_isPersonaDraw = true;
	}

	// 解禁中は追従スムーズに付いてくる
	if (m_highJumpUnlock)
	{
		// 追従率
		// 0.3f
		float personaFollowing = m_personaFollowAlpha;
		m_PersonaPosX += (targetX - m_PersonaPosX) * personaFollowing;
		m_PersonaPosY += (targetY - m_PersonaPosY) * personaFollowing;
	}
	else
	{
		// ロック中は次に解禁したときにスナップさせる
		m_isPersonaDraw = false;
	}
	//エフェクトの更新
	if (m_pAnimationManager)
	{
		m_pAnimationManager->Update();
	}

}
void Player::Draw(const Camera& camera)
{
	const Vec2 cameraOffset = camera.GetCameraOffset();

	if (m_pAnimationManager)
	{
		m_pAnimationManager->Draw(cameraOffset);
	}

	//中心座標
	const int centerX = static_cast<int>(m_rect.GetX() + cameraOffset.x);
	const int centerY = static_cast<int>(m_rect.GetY() + cameraOffset.y);

	//左上、右上、左下、右下の座標計算
	const int halfW = static_cast<int>(m_rect.GetW() * 0.5f);
	const int halfH = static_cast<int>(m_rect.GetH() * 0.5f);

	const int left = centerX - halfW;
	const int right = centerX + halfW;
	const int Top = centerY - halfH;
	const int Bottom = centerY + halfH;


	// ペルソナを描画（プレイヤーより先＝背面）
	if (m_highJumpUnlock && m_PersonaHandle != -1)
	{
		// 座標
		int personaX = static_cast<int>(m_PersonaPosX + cameraOffset.x);
		int personaY = static_cast<int>(m_PersonaPosY + cameraOffset.y);

		DrawRectRotaGraph(
			personaX, personaY,
			m_PersonaCutX, m_PersonaCutY,
			m_PersonaCutW, m_PersonaCutH,
			1.0f,
			0.0f,
			m_PersonaHandle,
			true,
			IsLeft
		);
	}



	//描画する画像ハンドルを決定
	int handle = m_IdleHandle;
	int persona = m_PersonaHandle;
	
	bool drawPlayer = true;

	//無敵時間中は表示/非表示を切り替える
	if (m_invincibleTime > 0)
	{
		
		const int blinkRate = 6; // 何フレームごとに切り替えるか（調整可）
		bool show = ((m_invincibleTime / blinkRate) % 2) == 0;
		if (show && m_HitHandle != -1)
		{
			// 被弾時の画像を表示
			handle = m_HitHandle;
		}
		else
		{
			// 非表示フェーズなら描画しない（点滅）
			// ただしデバッグ枠は描くため、ここで return せず一旦スキップ描画して下で枠のみ描画する
			drawPlayer = false;
		}
	}
	else
	{
		switch (m_animState)
		{
		case AnimState::Run:  handle = m_RunHandle;  break;
		case AnimState::Jump: handle = m_JumpHandle; break;
		case AnimState::High: handle = m_HighJumpHandle; break;
		case AnimState::Fall: handle = m_FallHandle; break;
		case AnimState::Idle: default: handle = m_IdleHandle; break;
		}
	}



	if (drawPlayer && handle != -1)
	{
		DrawRectRotaGraph(
			centerX, centerY,
			m_CutX, m_CutY,
			m_CutW, m_CutH,
			1.0f,
			0.0f,
			handle,
			true,
			IsLeft
		);
	}



#ifdef _DEBUG
	//当たり判定の枠
	DrawBox(
		left, Top,
		right, Bottom,
		GetColor(255, 0, 0),
		false
	);

#endif

}
