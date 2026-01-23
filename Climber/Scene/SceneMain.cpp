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
#include "../System/SoundManager.h"
#include <memory>
#include <cassert>

SceneMain::SceneMain(SceneContoller& controller):
Scene(controller),
m_draw(0),
m_update(0),
m_fontHandle(-1),
m_popupDisplayTime(0),
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
	m_timer.Reset(10.0f);
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
		r1->SetPos({ 238, 158257, });
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

		auto r6 = std::make_shared<Rabbit>();
		r6->Init();
		r6->SetPos({ 427, 159441 });
		m_pRabbits.push_back(r6);

		auto r7 = std::make_shared<Rabbit>();
		r7->Init();
		r7->SetPos({ 592, 154705 });
		m_pRabbits.push_back(r7);

		auto r8 = std::make_shared<Rabbit>();
		r8->Init();
		r8->SetPos({ 253, 154641 });
		m_pRabbits.push_back(r8);

		auto r9 = std::make_shared<Rabbit>();
		r9->Init();
		r9->SetPos({ 305, 152721 });
		m_pRabbits.push_back(r9);
	}

	// コウモリを複数配置
	{
		auto b1 = std::make_shared<Bat>();
		b1->Init();
		b1->SetPos({ 350, 159800, });
		m_pBats.push_back(b1);

		auto b2 = std::make_shared<Bat>();
		b2->Init();
		b2->SetPos({ 350, 159282, });
		m_pBats.push_back(b2);

		auto b3 = std::make_shared<Bat>();
		b3->Init();
		b3->SetPos({ 320,157906});
		m_pBats.push_back(b3);

		auto b4 = std::make_shared<Bat>();
		b4->Init();
		b4->SetPos({ 495,157225 });
		m_pBats.push_back(b4);

		auto b5 = std::make_shared<Bat>();
		b5->Init();
		b5->SetPos({ 401,156228 });
		m_pBats.push_back(b5);

		auto b6 = std::make_shared<Bat>();
		b6->Init();
		b6->SetPos({ 527,149571 });
		m_pBats.push_back(b6);

		auto b7 = std::make_shared<Bat>();
		b7->Init();
		b7->SetPos({ 321,150626 });
		m_pBats.push_back(b7);

		auto b8 = std::make_shared<Bat>();
		b8->Init();
		b8->SetPos({ 303,151129 });
		m_pBats.push_back(b8);

		auto b9 = std::make_shared<Bat>();
		b9->Init();
		b9->SetPos({ 406,151773 });
		m_pBats.push_back(b9);

		auto b10 = std::make_shared<Bat>();
		b10->Init();
		b10->SetPos({ 53,159422 });
		m_pBats.push_back(b10);

		auto b11 = std::make_shared<Bat>();
		b11->Init();
		b11->SetPos({ 600,159245 });
		m_pBats.push_back(b11);

		auto b12 = std::make_shared<Bat>();
		b12->Init();
		b12->SetPos({ 53,158520 });
		m_pBats.push_back(b12);

		auto b13 = std::make_shared<Bat>();
		b13->Init();
		b13->SetPos({ 53,157265 });
		m_pBats.push_back(b13);

		auto b14 = std::make_shared<Bat>();
		b14->Init();
		b14->SetPos({ 506,155744 });
		m_pBats.push_back(b14);

		auto b15 = std::make_shared<Bat>();
		b15->Init();
		b15->SetPos({ 271,155744 });
		m_pBats.push_back(b15);
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
	
	SoundManager::Load();


	SoundManager::PlayBGM("Stage1");

}

void SceneMain::Update(Input& input)
{
	m_frameCount++;

	//前のスコアを保存
	const int previousScore = m_score;


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


	// 破壊タイルによるスコア集計
	const int brokeNow = m_pPlayer->GetBrokeCount();
	const int tilepoint = m_pStage->GetTileBrokePoint();

	const int delta = brokeNow - m_lastScore;
	if (delta > 0)
	{
		m_score += delta * m_pStage->GetTileBrokePoint();
		m_lastScore = brokeNow;
	}
	if (brokeNow >= m_lastScore)
	{
		//増えた分だけ加点
		const int delta = brokeNow - m_lastScore;
		if (delta > 0)
		{
			m_score += delta * tilepoint;
		}
		m_lastScore = brokeNow;
	}
	else
	{
		//リセット後に壊した分も加点する
		if (brokeNow > 0)
		{
			m_score += brokeNow * tilepoint;
		}
		m_lastScore = brokeNow;
	}

	if (m_score < 0) { m_score = 0; }
	// ここまでスコア計算
	const int scoreDelta = m_score - previousScore;
	if (scoreDelta > 0)
	{
		//ポップ開始
		m_popupAmount = scoreDelta;
		m_popupFrame = 0;
		//1秒間表示
		m_popupDisplayTime = 60;
		m_popupAlpha = 255;
		//表示位置リセット
		m_popupY = 70.0f;
	}

	// カメラ・背景更新
	m_pCamera->UpdateCamera(m_pPlayer);
	m_pBg->Update();

	// タイマー更新
	// 終了判定
	m_timer.Update();
	if (m_timer.IsTimeUp())
	{

		ResultData::SetScore(m_score);
		ResultData::SetKillCount(m_killCount);   //これが必要！！
		auto result = std::make_shared<ResultScene>(m_controller);
		m_controller.ChangeScene(result);
		return;

	}
}


void SceneMain::Draw()
{
	
	m_pBg->Draw(*m_pCamera);
	m_pStage->Draw(*m_pCamera, 0, 0);//ステージデータの描画
	m_pRect->Draw();
	

	for (auto& rabbit: m_pRabbits)
	{
		rabbit->Draw(*m_pCamera);
	}
	for (auto& bat: m_pBats)
	{
		bat->Draw(*m_pCamera,*m_pPlayer);
	}
	m_pPlayer->Draw(*m_pCamera);
	
	
	constexpr int Rabbit = 1;


	//ロードしたステージデータの描画
	auto mapSize = m_pStage->MapSize();
	const auto& mapData = m_pStage->GetAllData();

#ifdef _DEBUG

	DrawFormatString(100, 100, 0xffffff, "%.2f,%.2f", m_pPlayer->GetPos().x, m_pPlayer->GetPos().y);
#endif
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
	//sprintf_s(ScoreBuf, sizeof(ScoreBuf), "+%d", m_score);

	//キルした数
	char KillBuf[64];
	sprintf_s(KillBuf, sizeof(KillBuf), "KILLS:%d", m_killCount);

	const int scoreX = 20;
	const int scoreY = 70;


	DrawStringToHandle(20, 50,  TimeBuf,0xffffff, m_fontHandle, remainSec);
	DrawStringToHandle(20, 70, ScoreBuf, 0xffffff,m_fontHandle,m_score);
	DrawStringToHandle(20, 90, KillBuf, 0xffffff,m_fontHandle,m_killCount);

	// ポップアップ表示
	if (m_popupFrame < m_popupDisplayTime && m_popupAmount > 0)
	{
		m_popupFrame++;
		// Y位置を上に移動
		m_popupY -= 0.5f;
		// 徐々に透明化
		m_popupAlpha = 255 * (m_popupDisplayTime - m_popupFrame) / m_popupDisplayTime;
		// スコアの右端位置を取得
		const int scoreTextWidth =GetDrawStringWidthToHandle(ScoreBuf, std::strlen(ScoreBuf), m_fontHandle);
		// 右に10ずらす
		const int popupX = scoreX + scoreTextWidth + 10; 
		//プラスされる位置
		const int popupY = static_cast<int>(m_popupY);
		// ポップアップテキスト
		char PopupBuf[32];
		std::snprintf(PopupBuf, sizeof(PopupBuf), "+%d", m_popupAmount);

		// 読みやすいように影
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_popupAlpha);
		//カラー
		const unsigned int shadow = GetColor(0, 0, 0);
		//四方向にずらして描画して影を作る
		const int ox[4] = { -1, 1, -1, 1 };
		const int oy[4] = { -1, -1, 1, 1 };
		for (int i = 0; i < 4; ++i)
		{
			DrawStringToHandle(popupX + ox[i], popupY + oy[i], PopupBuf, shadow, m_fontHandle);
		}
		// 本体カラー
		const unsigned int gold = GetColor(255, 240, 90);
		// 本体描画
		DrawStringToHandle(popupX, popupY, PopupBuf, gold, m_fontHandle);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

	
