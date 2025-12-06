#include "SceneMain.h"
#include "DxLib.h"
#include "Game.h"
#include "Bg.h"
#include "Camera.h"
#include "Rect.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"


SceneMain::SceneMain():
m_frameCount(0)
{
	m_pPlayer	= std::make_shared<Player>();
	m_pEnemy	= std::make_shared<Enemy>();
	m_pCamera	= std::make_shared<Camera>();
	m_pRect		= std::make_shared<Rect>();
	m_pBg		= std::make_shared<Bg>();
	m_pStage	= std::make_shared<Stage>();

	m_pStage->Load(1);//ステージ1をロード

}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	m_pPlayer->Init();
	m_pEnemy->Init();
	int chipHandle = LoadGraph("data/stage1.fmf");
	m_pStage->SetTileSet(chipHandle, 16, 16);//タイルセットの設定
}

void SceneMain::Update()
{
	m_frameCount++;
	m_pPlayer->Update(*m_pEnemy,*m_pRect,*m_pBg);
	m_pEnemy->Update(*m_pPlayer);
	m_pCamera->UpdateCamera(m_pCamera,m_pPlayer);
	m_pBg->Update();

	
}

void SceneMain::Draw()
{
	
	m_pBg->Draw(*m_pCamera);
	//m_pBg->DrawMapChip(*m_pCamera);
	m_pStage->Draw(*m_pCamera, 0, 0);//ステージデータの描画
	m_pRect->Draw();
	m_pPlayer->Draw(*m_pCamera);
	m_pEnemy->Draw(*m_pCamera);

	//ロードしたステージデータの描画
	auto mapSize = m_pStage->MapSize();
	const auto& mapData = m_pStage->GetAllData();


	DrawFormatString(100, 100, 0xffffff, "%.2f,%.2f", m_pPlayer->GetPos().x, m_pPlayer->GetPos().y);

	//仮地面の描画
	DrawLine(0 + m_pCamera->GetCameraOffset().x, 640 + m_pCamera->GetCameraOffset().y, Game::kScreenWidth + m_pCamera->GetCameraOffset().x, 640 + m_pCamera->GetCameraOffset().y, GetColor(255, 255, 255));
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
}
