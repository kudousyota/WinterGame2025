#include "SceneMain.h"
#include "DxLib.h"
#include "Game.h"
#include "Bg.h"
#include "Camera.h"
#include "Rect.h"
#include "Player.h"
#include "Enemy.h"



SceneMain::SceneMain():
m_frameCount(0)
{
	m_pPlayer	= std::make_shared<Player>();
	m_pEnemy	= std::make_shared<Enemy>();
	m_pCamera	= std::make_shared<Camera>();
	m_pRect		= std::make_shared<Rect>();
	m_pBg		= std::make_shared<Bg>();

}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	m_pPlayer->Init();
	m_pEnemy->Init();
}

void SceneMain::Update()
{
	m_frameCount++;
	m_pPlayer->Update();
	m_pEnemy->Update();
	m_pCamera->UpdateCamera(m_pCamera,m_pPlayer);
	m_pBg->Update();

	
}

void SceneMain::Draw()
{
	
	m_pBg->Draw(*m_pCamera);
	m_pBg->DrawMapChip(*m_pCamera);
	m_pRect->Draw();
	m_pPlayer->Draw(*m_pCamera);
	m_pEnemy->Draw(*m_pCamera);
	DrawFormatString(100, 100, 0xffffff, "%.2f,%.2f", m_pPlayer->GetPos().x, m_pPlayer->GetPos().y);

	//‰¼’n–Ê‚Ì•`‰æ
	DrawLine(0 + m_pCamera->GetCameraOffset().x, 640 + m_pCamera->GetCameraOffset().y, Game::kScreenWidth + m_pCamera->GetCameraOffset().x, 640 + m_pCamera->GetCameraOffset().y, GetColor(255, 255, 255));
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
}
