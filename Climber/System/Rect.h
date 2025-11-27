#pragma once
#include "Vec2.h"
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
	
	//setter
	void SetX(float x) { m_x = x; }
	void SetY(float y) { m_y = y; }


	void Init(float x, float y, float w, float h);
	void Update();
	void Draw();

	bool IsHit(const Rect& other) const;
	//‰Ÿ‚µo‚µ‚½—Ê‚ğ•Ô‚·
	Vec2 FixPos(const Rect& other);

	private:
		float m_x;
		float m_y;
		float m_z;
		float m_w;
		float m_h;
};

