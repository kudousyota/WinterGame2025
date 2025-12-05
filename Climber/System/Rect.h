#pragma once
#include "Vec2.h"

struct Size
{
	int w;
	int h;
};
class Rect
{
public:
	Rect();
	~Rect();

	//getter
	float GetX() const { return m_x; }
	float GetY() const { return m_y; }
	float GetZ() const { return m_z; }
	float GetW() const { return m_w; }
	float GetH() const { return m_h; }

	//中心座標から各辺の座標を取得
	float GetLeft()  const   { return m_x - m_w * 0.5f;}
	float GetTop()   const   { return m_y - m_h * 0.5f;}
	float GetRight() const   { return m_x + m_w * 0.5f;}
	float GetBottom()const   { return m_y + m_h * 0.5f;}

	//setter
	void Set(float left, float top, float right, float bottom);
	
	void SetX(float x) { m_x = x; }
	void SetY(float y) { m_y = y; }


	void Init(float x, float y, float w, float h);
	void Update();
	void Draw();

	//矩形同士が当たっているかどうかを返す
	bool IsHit(const Rect& other) const;

	//矩形同士が交差しているかどうかを返す
	bool Intersects(const Rect& other) const;
	
	//押し出した量を返す
	Vec2 FixPos(const Rect& other);

	private:
		float m_x;
		float m_y;
		float m_z;
		float m_w;
		float m_h;


};

