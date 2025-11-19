#pragma once
#include "Vec2.h"

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
	void DrawMapChip();
	////縦のスクロール量を取得
	//int GetSccrollX();
	////横のスクロール量を取得
	//int GetSccrollY();
	//
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

