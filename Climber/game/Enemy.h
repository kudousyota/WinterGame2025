#pragma once
#include "Actor.h"
#include <memory>
#include"Geometry.h"

class Enemy
{
public:
	Enemy(const Position2& pos,	// 敵の初期座標
		float r);	// 敵の当たり判定の大きさ

	//const Circle& GetCollision() const;
	bool IsDead() const;

	// 他のオブジェクトに当たったときに呼び出されます
	virtual void OnHit(const Actor& actor) {};
	void OnDead();

	/// <summary>
	/// 座標やパラメータの更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	virtual ~Enemy() {};	// 基底クラスのデストラクタはvirtualにしておく
protected:
	bool   m_isDead = false;	// 死亡フラグ
};

