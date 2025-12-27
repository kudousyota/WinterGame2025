#pragma once
#include <memory>
#include "Scene.h"
//#include "Geometry.h"

class Stage;
class Rect;
class Camera;
class Bg;
class Player;
class Enemy;
class GameObject;
class Input;
class TitleScene;
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
	std::shared_ptr<Enemy>  m_pEnemy;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Bg>     m_pBg;
	std::shared_ptr<TitleScene> m_pTitleScene;


	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);
	using UpdateFunc_t = void(SceneMain::*)(Input&);
	UpdateFunc_t m_update;	// Update系を受け取るメンバ関数ポインタ

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void(SceneMain::*)();
	DrawFunc_t m_draw;	// Draw系を受け取るメンバ関数ポインタ
	//bool IsHit(const Circle& a, const Circle& b);
};

