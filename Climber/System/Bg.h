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

	
	//”wŒi•\Ž¦
	void DrawBg(const Camera& camera);
	
	


private:

	//ƒ}ƒbƒvƒ`ƒbƒv‰æ‘œ
	int m_mapHandle;
	//”wŒi
	int	m_bgHandle;
	int m_cloudLonelyHandle;
	int m_cloudbgHandle;
	int m_cloudmgWonHandle;
	int m_cloudmgTwoHandle;
	int m_cloudmgHandle;
	int m_cloudHandle;
	int m_glacialHandle;
	int m_glacialWonHandle;
	int m_skyHandle;
	int m_skyTwoHandle;

	
	//”wŒi‚ÌˆÊ’u
	Vec2 m_pos;
	Vec2 m_src;
	

	int m_graphChipNumX;
	int m_graphChipNumY;
};

