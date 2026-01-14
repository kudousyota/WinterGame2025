#pragma once
#include "Character.h"
#include "../System/Vec2.h"
#include <memory>

class Bg;
class Camera;
class Rabbit;
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
	 void Update(Rect& other, const Bg& bg);
	 void Draw(const Camera& camera);


	 Vec2 GetPos() { return m_pos ; }

	 //縦速度の取得
	 float GetVelY() const{ return m_vel; }
	 void SetVelY(float vel) { m_vel = vel; }

	 // 横速度の取得
	 float GetVelX () const { return m_speed; }
	 void SetVelX(float speed) { m_speed = speed; }

	 // 地面フラグ操作
	 bool IsOnGround() const { return m_onGround; }
	 void SetOnGround(bool onGround) { m_onGround = onGround; }

	 // ハイジャンプのフラグ取得
	 bool IsHighJumpUnlock() const { return m_highJumpUnlock; }
	 // ハイジャンプで速度を与えられているか
	 bool IsHighJumpNum() const { return m_vel < -20.0f; }
	 // ハイジャンプ処理フラグ（衝突で速度を消さないか判定するために使用）
	 bool IsHighJumpActive() const { return m_isHighJumpActive; }
	 void SetHighJumpActive(bool v) { m_isHighJumpActive = v; }
	 
	 //無敵時間関連
	 // 無敵時間の取得
	 void StartInvincible(int frame = 180);
	 bool Isinvincible() const { return m_invincibleTime > 0; }
	 int GetInvincibleTime() const { return m_invincibleTime; }

	 // ハイジャンプ中フラグ（true の間は衝突で縦速度を消さない）
	 bool m_isHighJumpActive = false;
	 // 破壊したタイル数の取得
	 int GetBrokeCount() const { return m_brokeCount; }

	 

	 // タイル破壊CollisionManagerから呼ぶ
	 void TileBroke();


private:
	//アニメーション	
	enum class AnimState { Idle, Run, Jump, Fall ,Hit};

	AnimState m_animState = AnimState::Idle; // 現在の状態
	int       m_animRow = 0;               // 現在のアニメ行（0=Idle,1=Run,2=Jump,3=Fall）
	int       m_runFrameMax = 11;           // 走りの横コマ数（画像に合わせて調整）
	int       m_jumpFrameMax = 1;            // ジャンプの横コマ数（複数なら変更）
	int       m_fallFrameMax = 1;            // 落下の横コマ数（複数なら変更）
	int       m_HitFrameMax = 1;             // ダメージの横コマ数（複数なら変更）

	//画像ハンドル
	int m_IdleHandle;
	int m_JumpHandle;
	int m_FallHandle;
	int m_RunHandle;
	int m_HitHandle;
	//カット位置
	int m_cutX;
	int m_cutY;
	//１フレームの高さ
	int m_cutW;
	int m_cutH;
	//フレームカウント
	float m_frameCount;
	//アニメ速度
	int m_IdleFrameMax; // 待機アニメの速度調整用
	int m_RunFrameMax; // 走りアニメの速度調整用
	int m_JumpFrameMax; // ジャンプアニメの速度調整用
	int m_FallFrameMax; // 落下アニメの速度調整用
	//切り替え速度
	int m_switchSpeed;

	//向き判定
	bool IsLeft = false;
	//ハイジャンプ解禁
	bool m_highJumpUnlock;
	//解禁に必要なポイント
	int m_highJumpPoint;
	//破壊した数
	int m_brokeCount;
	//地面にいるかどうか
	bool m_onGround = false;

	//無敵時間
	int m_invincibleTime;


	float m_x;
	Vec2 m_pos;

	float m_vel;
	float m_speed;


	//当たり判定
	bool isHit(const Rabbit& enemy);


	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<CollisionManager> m_pCollision;
	std::shared_ptr<Input> m_pInput;
	//std::shared_ptr<Rabbit>m_pRabbit;
	//std::shared_ptr<Enemy>m_pEnemy;

	////拡大率
	//float m_scaleX;
	//float m_scaleY;
	////角度
	//int m_Angle;

};

