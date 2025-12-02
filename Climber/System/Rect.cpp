#include "Rect.h"
#include "Vec2.h"
#include <Dxlib.h>
#include <cmath>
#include "Camera.h"

Rect::Rect():
m_x(0),
m_y(0),
m_z(0),
m_w(0),
m_h(0),
m_left(0),
m_top(0),
m_right(0),
m_bottom(0)
{
}

Rect::~Rect()
{
}

void Rect::Set(float left, float top, float right, float bottom)
{
	m_left = left;
	m_top = top;
	m_right = right;
	m_bottom = bottom;
}

void Rect::Init(float x, float y, float w, float h)
{
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
}

void Rect::Update()
{
}

void Rect::Draw()
{
	//DrawBox(m_x, m_y, m_w, m_h, GetColor(255, 0, 0), true);
}

bool Rect::IsHit(const Rect& other) const
{
	//Xの差分を取得
	float disX = other.GetX() - m_x;

	//Yの差分を取得
	float disY = other.GetY() - m_y;

	//絶対値化
	disX = std::fabs(disX);
	disY = std::fabs(disY);
	
	//幅(W)の確認するための値取得
	//幅を出す為に割る
	float checkW = (m_w + other.GetW()) / 2.0f;
	//高さの確認するための値を取得
	float checkH = (m_h + other.GetH()) / 2.0f;

	//Ｘ軸で重なっているか
	bool isHitX = disX < checkW;
	//Y軸で重なっているか
	bool isHItY = disY < checkH;

	//両方重なっていたら正解
	//重なっていなったら間違い
	return isHitX && isHItY;
}

Vec2 Rect::FixPos(const Rect& other)
{
	//元の位置を保持しておく
	float prevposX = m_x;
	float prevposY = m_y;

	//Xの差分を取得
	float disX = other.GetX() - m_x;
	//Ｙの差分を取得
	float disY = other.GetY() - m_y;
	//絶対値化
	disY = std::fabs(disY);
	disX = std::fabs(disX);
	//長さを計算
	float length = std::sqrt(disX * disX + disY * disY);
	

	float overlapX = (m_w + other.GetW()) / 2.0f - std::fabs(disX);
	float overlapY = (m_h + other.GetH()) / 2.0f - std::fabs(disY);
	// 最大押し出し量
	const float maxPush = 5.0f;

	//W上のでの位置を確認
	float rateX = disX / length;

	//H上での位置を確認
	float rateY = disY / length;

	// Xの方が大きい場合
	if (rateX > rateY)
	{
		//幅の合計の半分を取得

		float halfW = (m_w + other.GetW()) / 2.0f;

		//押し出し量を計算
		float moveX = halfW - disX;

		//押し出し側が左側にいるか判定

		bool isHItLeft = other.GetX() < m_x;

		if (isHItLeft)
		{
			//右側に押す
			m_x += moveX;
		}
		//右側にいる場合
		else
		{
			//左側に押す
			m_x -= moveX;

		}


	}
	//Yの方が大きい場合
	else
	{
		//高さの合計を取得
		float halfH = (m_h + other.GetH()) / 2.0f;

		//押し出し量を計算
		float moveY = halfH - disY;

		//押し出し側が上側にいるか判定

		bool isHitUp = other.GetY() < m_y;

		if (isHitUp)
		{
			//上側に押す
			m_y += moveY;
		}
		//上側にいる場合
		else
		{
			//下側に押す
			m_y -= moveY;
		}
	}
	return Vec2(m_x - prevposX, m_y - prevposY);
}