#include "SceneMain.h"
#include "DxLib.h"
#include "Game.h"
#include "Bg.h"
#include "Camera.h"
#include "Rect.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "CollisionManager.h"
#include "Input.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include "ResultData.h"
#include "SceneContoller.h"
#include <memory>
#include <cassert>

SceneMain::SceneMain(SceneContoller& controller):
Scene(controller),
m_frameCount(0)
{
	m_pPlayer	= std::make_shared<Player>();
	m_pEnemy	= std::make_shared<Enemy>();
	m_pCamera	= std::make_shared<Camera>();
	m_pRect		= std::make_shared<Rect>();
	m_pBg		= std::make_shared<Bg>();
	m_pStage	= std::make_shared<Stage>();
	m_pStageTwo = std::make_shared<Stage>();
	m_pTitleScene = std::make_shared<TitleScene>(m_controller);
	m_pResultScene = std::make_shared<ResultScene>(m_controller);

	m_timer.Reset(10.0f);//指定した秒数でリセット
	m_score = 0;
	m_killCount = 0;
	//ステージをロード
	m_pStage->Load(2);
//	m_pStageTwo->Load(3);
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	m_pPlayer->Init();
	m_pEnemy->Init();
	int chipHandle = LoadGraph("data/mapChip1.png");
	assert(chipHandle > 0);
	int SchipHandle = LoadGraph("data/mapChip1.png");
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
	m_pPlayer->Update(*m_pEnemy,*m_pRect,*m_pBg);
	m_pEnemy->Update(*m_pPlayer);

	//  衝突チェックを呼ぶ（ここで着地判定・押し出し・タイル破壊を行う）
	CollisionManager::CheckCollisions(m_pPlayer, m_pEnemy, m_pStage);

	// カメラ・背景更新（衝突後の位置でカメラを更新するため衝突チェックの後に呼ぶ）
	m_pCamera->UpdateCamera(m_pCamera,m_pPlayer);
	m_pBg->Update();

	m_timer.Update();
	// 時間切れならリザルトへ
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
	m_pEnemy->Draw(*m_pCamera);
	//m_pTitleScene->Draw();
	//ロードしたステージデータの描画
	auto mapSize = m_pStage->MapSize();
	const auto& mapData = m_pStage->GetAllData();


	DrawFormatString(100, 100, 0xffffff, "%.2f,%.2f", m_pPlayer->GetPos().x, m_pPlayer->GetPos().y);

	//仮地面の描画
	//DrawLine(0 + m_pCamera->GetCameraOffset().x, 640 + m_pCamera->GetCameraOffset().y, Game::kScreenWidth + m_pCamera->GetCameraOffset().x, 640 + m_pCamera->GetCameraOffset().y, GetColor(255, 255, 255));
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);

	const int remainSec = static_cast<int>(std::ceil(m_timer.Remaining()));
	DrawFormatString(20, 20, GetColor(255, 255, 255), "TIME: %d", remainSec);
	DrawFormatString(20, 40, GetColor(255, 255, 0), "SCORE: %d", m_score);
	DrawFormatString(20, 60, GetColor(255, 255, 0), "KILLS: %d", m_killCount);

}
