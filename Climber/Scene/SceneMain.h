#pragma once
#include <memory>
#include "Scene.h"
#include "../System/GameTimer.h"
#include <vector>
//#include "Geometry.h"

class Stage;
class Rect;
class Camera;
class Bg;
class Player;
class EnemyFactory;
class Enemy;
class Rabbit;
class Bat;
class GameObject;
class Input;
class TitleScene;
class ResultScene;
class CollisionManager;
class SceneMain  : public Scene
{
public:
	SceneMain(SceneContoller& controller);
	virtual ~SceneMain();

	virtual void Init();
	virtual void Update(Input& input);
	virtual void Draw();

private:
	bool isStageEnd = false;//ステージの最後に到達しているかどうか
	int m_frameCount;
	//各種オブジェクトのポインタ
	//ステージデータ
	std::shared_ptr<Stage>  m_pStage;
	std::shared_ptr<Stage>  m_pStageTwo;
	std::shared_ptr<Rect>   m_pRect;
	std::shared_ptr<Player> m_pPlayer;
	//std::shared_ptr<Rabbit>  m_pRabbit;
	std::vector<std::shared_ptr<Rabbit>>m_pRabbits;
	std::vector<std::shared_ptr<Bat>>m_pBats;
	//std::shared_ptr<Bat>    m_pBat;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Bg>     m_pBg;
	std::shared_ptr<TitleScene> m_pTitleScene;
	std::shared_ptr<ResultScene>m_pResultScene;
	std::shared_ptr<EnemyFactory>m_pEnemyFactory;

	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);
	using UpdateFunc_t = void(SceneMain::*)(Input&);
	UpdateFunc_t m_update;	// Update系を受け取るメンバ関数ポインタ

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void(SceneMain::*)();
	DrawFunc_t m_draw;	// Draw系を受け取るメンバ関数ポインタ
	GameTimer m_timer{ 60.0f };//60秒タイマー
	float m_limitSeconds = 60.0f;//制限時間
	int m_score = 0;//スコア
	int m_lastScore = 0;//前回のスコア
	int m_killCount = 0;//倒した敵の数

	int m_fontHandle;

	
	
	
	//bool IsHit(const Circle& a, const Circle& b);
};

