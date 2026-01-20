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
	//m_pRabbit	= std::make_shared<Rabbit>();
	//m_pBat		= std::make_shared<Bat>();
	m_pCamera	= std::make_shared<Camera>();
	m_pRect		= std::make_shared<Rect>();
	m_pBg		= std::make_shared<Bg>();
	m_pStage	= std::make_shared<Stage>();
	m_pStageTwo = std::make_shared<Stage>();
	m_pTitleScene = std::make_shared<TitleScene>(m_controller);
	m_pResultScene = std::make_shared<ResultScene>(m_controller);
	//m_pEnemyFactory = std::make_shared<EnemyFactory>();

	//指定した秒数で終了
	m_timer.Reset(2.0f);
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
	//フォントの作成
	m_fontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 24, -1, -1);

	m_pPlayer->Init();
	// うさぎを複数配置
	{
		auto r1 = std::make_shared<Rabbit>();
		r1->Init();
		r1->SetPos({ 350.0f, 159800.0f, });
		m_pRabbits.push_back(r1);

		auto r2 = std::make_shared<Rabbit>();
		r2->Init();
		r2->SetPos({ 319.0f, 158577.0f, });
		m_pRabbits.push_back(r2);

		auto r3 = std::make_shared<Rabbit>();
		r3->Init();
		r3->SetPos({ 321.0f, 157425.0f });
		m_pRabbits.push_back(r3);

		auto r4 = std::make_shared<Rabbit>();
		r4->Init();
		r4->SetPos({ 301.0f, 156721.0f });
		m_pRabbits.push_back(r4);

		auto r5 = std::make_shared<Rabbit>();
		r5->Init();
		r5->SetPos({ 641.0f, 150193.0f });
		m_pRabbits.push_back(r5);

	}

	// コウモリを複数配置
	{
		auto b1 = std::make_shared<Bat>();
		b1->Init();
		b1->SetPos({ 350.0f, 159800.0f, });
		m_pBats.push_back(b1);

		auto b2 = std::make_shared<Bat>();
		b2->Init();
		b2->SetPos({ 350.0f, 159282.0f, });
		m_pBats.push_back(b2);

		auto b3 = std::make_shared<Bat>();
		b3->Init();
		b3->SetPos({ 320.0f,157906.0f });
		m_pBats.push_back(b3);

		auto b4 = std::make_shared<Bat>();
		b4->Init();
		b4->SetPos({ 495.0f,157225.0f });
		m_pBats.push_back(b4);

		auto b5 = std::make_shared<Bat>();
		b5->Init();
		b5->SetPos({ 401.0f,156228.0f });
		m_pBats.push_back(b5);

		auto b6 = std::make_shared<Bat>();
		b6->Init();
		b6->SetPos({ 527.0f,149571.0f });
		m_pBats.push_back(b6);

		auto b7 = std::make_shared<Bat>();
		b7->Init();
		b7->SetPos({ 321.0f,150626.0f });
		m_pBats.push_back(b7);
	}

	
	//m_pRabbit->Init();
	//m_pBat->Init();
	int chipHandle = LoadGraph("data/mapChip1.png");
	//assert(chipHandle > 0);
	int SchipHandle = LoadGraph("data/Enemy.png");
	assert(SchipHandle > 0);
	//タイルセットの設定
	//小さすぎたから1チップ32x32で設定
	m_pStage->SetTileSet(chipHandle, 32, 32);
	//m_pStageTwo->SetTileSet(SchipHandle, 32, 32);
}

void SceneMain::Update(Input& input)
{
	m_frameCount++;
	//  キャラの更新（移動・重力など）
	m_pPlayer->Update(*m_pRect, *m_pBg);
	//敵の更新
	for (auto& rabbit : m_pRabbits)
	{
		rabbit->Update(*m_pPlayer);
	}
	for (auto& bat : m_pBats )
	{
		bat->Update(*m_pPlayer);
	}

	// 衝突チェックを呼ぶここで着地判定・押し出し・タイル破壊を行う

	if (m_pPlayer && m_pStage)
	{
		int killCount = 0;
		m_score += CollisionManager::CheckCollisions(m_pPlayer, m_pRabbits, m_pBats, m_pStage,killCount);
		m_killCount += killCount;
	}


	// 敵が死亡していれば削除

	m_pRabbits.erase(
		std::remove_if(m_pRabbits.begin(), m_pRabbits.end(),
			[](auto& r) { return r->IsDead; }),
		m_pRabbits.end()
	);

	m_pBats.erase(
		std::remove_if(m_pBats.begin(), m_pBats.end(),
			[](auto& b) { return b->IsDead; }),
		m_pBats.end()
	);


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
	m_pCamera->UpdateCamera(m_pPlayer);
	m_pBg->Update();

	// タイマー更新
	// 終了判定
	m_timer.Update();
	if (m_timer.IsTimeUp())
	{

		ResultData::SetScore(m_score);
		ResultData::SetKillCount(m_killCount);   // これが必要！！
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

	for (auto& rabbit: m_pRabbits)
	{
		rabbit->Draw(*m_pCamera);
	}
	for (auto& bat: m_pBats)
	{
		bat->Draw(*m_pCamera,*m_pPlayer);
	}
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
