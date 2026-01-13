#include "SceneMain.h"
#include "DxLib.h"
#include "../System/Game.h"
#include "../System/Bg.h"
#include "../System/Camera.h"
#include "../System/Rect.h"
#include "../game/Player.h"
#include "../game/Enemy.h"
#include "../game/EnemyFactory.h"
#include "../game/Rabbit.h"
#include "../game/Bat.h"
#include "../game/Stage.h"
#include "../System/CollisionManager.h"
#include "../System/Input.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include "../System/ResultData.h"
#include "SceneContoller.h"
#include <memory>
#include <cassert>

SceneMain::SceneMain(SceneContoller& controller):
Scene(controller),
m_draw(0),
m_update(0),
m_fontHandle(-1),
m_frameCount(0)
{
	m_pPlayer	= std::make_shared<Player>();
	m_pRabbit	= std::make_shared<Rabbit>();
	m_pBat		= std::make_shared<Bat>();
	m_pCamera	= std::make_shared<Camera>();
	m_pRect		= std::make_shared<Rect>();
	m_pBg		= std::make_shared<Bg>();
	m_pStage	= std::make_shared<Stage>();
	m_pStageTwo = std::make_shared<Stage>();
	m_pTitleScene = std::make_shared<TitleScene>(m_controller);
	m_pResultScene = std::make_shared<ResultScene>(m_controller);
	//m_pEnemyFactory = std::make_shared<EnemyFactory>();

	//指定した秒数で終了
	m_timer.Reset(100.0f);
	m_score = 0;
	m_killCount = 0;
	//ステージをロード
	m_pStage->Load(2);
//	m_pStageTwo->Load(3);
}

SceneMain::~SceneMain()
{
	//フォントの解放
	DeleteFontToHandle(m_fontHandle);
}

void SceneMain::Init()
{
	m_fontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 24, -1, -1);
	m_pPlayer->Init();
	m_pRabbit->Init();
	m_pBat->Init();
	int chipHandle = LoadGraph("data/mapChip1.png");
	assert(chipHandle > 0);
	int SchipHandle = LoadGraph("data/Enemy.png");
	assert(SchipHandle > 0);
	//タイルセットの設定
	//小さすぎたから1チップ32x32で設定
	m_pStage->SetTileSet(chipHandle, 32, 32);
	m_pStageTwo->SetTileSet(SchipHandle, 32, 32);
}

void SceneMain::Update(Input& input)
{
	m_frameCount++;
	//  キャラの更新（移動・重力など）
	m_pPlayer->Update(*m_pRabbit,*m_pRect,*m_pBg);

	if (m_pRabbit) { m_pRabbit->Update(*m_pPlayer); }
	if (m_pBat) { m_pBat->Update(*m_pPlayer); }

	// 衝突チェックを呼ぶここで着地判定・押し出し・タイル破壊を行う

	if (m_pPlayer && m_pStage) {
		m_score += CollisionManager::CheckCollisions(m_pPlayer, m_pRabbit, m_pBat, m_pStage);
	}


	// --- 敵が死亡していれば削除

	if (m_pRabbit && m_pRabbit->IsDead)
	{
		m_pRabbit.reset(); // 削除
		m_killCount++;     // キル数カウント
	}
	if (m_pBat && m_pBat->IsDead)
	{
		m_pBat.reset();
		m_killCount++;
	}

	// 破壊タイルによるスコア集計（差分）
	const int brokeNow = m_pPlayer->GetBrokeCount();
	const int delta = brokeNow - m_lastScore;
	if (delta > 0)
	{
		m_score += delta * m_pStage->GetTileBrokePoint();
		m_lastScore = brokeNow;
	}
	if (m_score < 0) { m_score = 0; }

	// カメラ・背景更新
	m_pCamera->UpdateCamera(m_pCamera, m_pPlayer);
	m_pBg->Update();

	// タイマー更新
	m_timer.Update();
	if (m_timer.IsTimeUp())
	{
		ResultData::SetScore(m_score);
		auto result = std::make_shared<ResultScene>(m_controller);
		m_controller.ChangeScene(result);
		return;
	}
}


void SceneMain::Draw()
{
	
	m_pBg->Draw(*m_pCamera);
	m_pStage->Draw(*m_pCamera, 0, 0);//ステージデータの描画
	//m_pStageTwo->Draw(*m_pCamera, 0, 0);
	m_pRect->Draw();
	m_pPlayer->Draw(*m_pCamera);

	if (m_pRabbit) { m_pRabbit->Draw(*m_pCamera); }
	if (m_pBat) { m_pBat->Draw(*m_pCamera, *m_pPlayer); }

	//m_pEnemyFactory->Draw();
	//m_pTitleScene->Draw();
	
	constexpr int Rabbit = 1;


	//ロードしたステージデータの描画
	auto mapSize = m_pStage->MapSize();
	const auto& mapData = m_pStage->GetAllData();


	DrawFormatString(100, 100, 0xffffff, "%.2f,%.2f", m_pPlayer->GetPos().x, m_pPlayer->GetPos().y);

	//仮地面の描画
	//DrawLine(0 + m_pCamera->GetCameraOffset().x, 640 + m_pCamera->GetCameraOffset().y, Game::kScreenWidth + m_pCamera->GetCameraOffset().x, 640 + m_pCamera->GetCameraOffset().y, GetColor(255, 255, 255));
	//DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	//DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);

	const int remainSec = static_cast<int>(std::ceil(m_timer.Remaining()));
	//タイマー
	char TimeBuf[64];
	sprintf_s(TimeBuf, sizeof(TimeBuf), "TIME:%d", remainSec);
	//スコア
	char ScoreBuf[64];
	sprintf_s(ScoreBuf, sizeof(ScoreBuf), "SCORE:%d", m_score);
	//キルした数
	char KillBuf[64];
	sprintf_s(KillBuf, sizeof(KillBuf), "KILLS:%d", m_killCount);
	DrawStringToHandle(20, 50,  TimeBuf,0xffffff, m_fontHandle, remainSec);
	DrawStringToHandle(20, 70, ScoreBuf, 0xffffff,m_fontHandle,m_score);
	DrawStringToHandle(20, 90, KillBuf, 0xffffff,m_fontHandle,m_killCount);

}
