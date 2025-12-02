#pragma once
#include "Vec2.h"

class Rect;
class Camera;
class Player;
class Bg
{
public:
	Bg();
	virtual ~Bg();
	void Init();
	void Update();
	void Draw(const Camera& camera);

	
	//背景表示
	void DrawBg(const Camera& camera);
	
	//マップを表示
	void DrawMapChip(const Camera& camera);

/// <summary>
/// 指定した矩形とあったっているか
/// </summary>
	bool IsCollision(Rect rect, Rect& chipRect);

private:

	//マップチップ画像
	int m_mapHandle;
	//背景
	int	m_bgHandle;
	
	//背景の位置
	Vec2 m_pos;
	Vec2 m_src;
	

	int m_graphChipNumX;
	int m_graphChipNumY;
};

