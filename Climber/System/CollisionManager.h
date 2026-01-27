#pragma once
#include "Rect.h"
#include <vector>
#include "Vec2.h"
#include <memory>
class Player;
class Rabbit;
class Bat;
class Stage;
class CollisionManager
{
public:
	void Init();
	void Update();
	void Draw(std::shared_ptr<Player>& m_pPlayer,
		std::shared_ptr<Stage>& m_pStage);

	//全ての当たり判定をチェックする
	static int CheckCollisions(
		std::shared_ptr<Player>& m_pPlayer,
		std::vector<std::shared_ptr<Rabbit>>& m_pRabbits,
		std::vector<std::shared_ptr<Bat>>& m_pBats,
		std::shared_ptr<Stage>& m_pStage,
		int& killCount
	);
	

private:
	// エフェクト画像の 1コマサイズ（16×16 固定）
	int m_effectCutW;
	int m_effectCutH;
	int m_effectCutX;
	int	m_effectCutY;
	int m_frameCount;
	int m_effectHandle = -1;

	int m_SwitchSpeed;
	int m_effectFrameMax;



};

