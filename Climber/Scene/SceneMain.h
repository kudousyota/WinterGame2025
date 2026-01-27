#pragma once
#include <memory>
#include "Scene.h"
#include "../System/GameTimer.h"
#include "../System/HitStopManager.h"
#include <vector>
#include <algorithm>
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
	std::shared_ptr<CollisionManager>m_pCollisionManager;
	//std::vector<std::shared_ptr<CollisionManager>>m_pCollisionmanager;

	HitStopManager m_hitStop;

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
	//表示中の+加点
	int m_popupAmount = 0;
	//表示中の+加点のフレームカウント
	int m_popupFrame = 0;
	//表示時間
	int m_popupDisplayTime;
	//現在アルファ(0-255)
	int m_popupAlpha = 0;
	//表示Y
	float m_popupY = 0.0f;  
	//前フレームのスコア
	int  m_prevScore = 0;   

	enum class ClockState{ Twelve ,Three,Six,Nine};
	ClockState m_ClockState = ClockState::Twelve; //現在の状態
	void AdvanceClock();

	float   m_clockElapsedSec = 0.0f;     // 秒カウント
	
	float   m_clockScale = 3.0f;          // 表示倍率
	int     m_clockAlpha = 255;           // アルファ（点滅用）
	bool    m_clockBlinkUp = false;       // アルファ増減方向

	bool m_clockSwitc = false;

	//ヒットストップ
	bool m_hitStopRequested;


	bool m_thirtyActive = false;   // 30秒演出中
	bool m_thirtyTriggered = false; // 一度だけ出すため
	int  m_thirtyFrame = 0;        // 経過フレーム
	int  m_thirtyAlpha = 0;        // 透明度

	//10秒演出
	bool m_tenActive = false;
	bool m_tenTriggered = false;
	int  m_tenFrame = 0;
	int  m_tenAlpha = 0;

	
	//ハンドル
	int m_fontHandle;
	int m_clocktwelveHandle;
	int m_clockThreeHandle;
	int m_clockSixHandle;
	int m_cloclNineHanlde;
	

 // 位置
	std::vector<float> m_fxX;
	std::vector<float> m_fxY;
	// 進行・寿命
	std::vector<int>   m_fxFrame;         // 経過フレーム
	std::vector<int>   m_fxTotalFrames;   // 再生コマ数（例：8）
	std::vector<int>   m_fxFramePerCell;  // 1コマ表示フレーム数（例：2）
	// 見た目
	std::vector<int>   m_fxAlpha;         // 必要ならフェード用（今は 255 固定でもOK）
	// スプライトシート選択
	std::vector<int>   m_fxStartCol;      // 再生開始コマの「列」
	std::vector<int>   m_fxStartRow;      // 再生開始コマの「行」

	
	
	// スプライトシートの横/縦のコマ数（画像サイズから算出）
	int m_effectSheetCols = 0;
	int m_effectSheetRows = 0;


};

