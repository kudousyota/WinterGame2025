#pragma once
#include <memory>

class Stage;
class Rect;
class Camera;
class Bg;
class Player;
class Enemy;
class GameObject;
class SceneMain
{
public:
	SceneMain();
	virtual ~SceneMain();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

private:
	bool isStageEnd = false;//ステージの最後に到達しているかどうか
	int m_frameCount;
	//各種オブジェクトのポインタ
	
	//ステージデータ
	std::shared_ptr<Stage>  m_pStage;
	std::shared_ptr<Rect>   m_pRect;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy>  m_pEnemy;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Bg>     m_pBg;
};

