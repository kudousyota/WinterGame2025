#include "Bg.h"
#include "DxLib.h"
#include "Game.h"
#include "../game/Player.h"
#include "Camera.h"

namespace
{
	//マップチップ一つのサイズ
	constexpr int kChipSize = 16;
	//マップチップ拡大率
	constexpr int kChipScale = 3.0f;



}

Bg::Bg():
	m_pos{0,600},
	m_src{1280,720},
	m_graphChipNumX(0),
	m_graphChipNumY(0)
{
	//m_mapHandle = LoadGraph("data/mapChip.png");
	//画像のマップチップ数を数える
	int graphW = 0;
	int graphH = 0;
	GetGraphSize(m_mapHandle, &graphW, &graphH);
	//背景の読み込み
	//m_bgHandle = LoadGraph("data/bg.png");
	m_cloudLonelyHandle = LoadGraph("bg/cloud_lonely.png");
	m_cloudbgHandle = LoadGraph("bg/clouds_bg.png");
	m_cloudmgWonHandle = LoadGraph("bg/clouds_mg_1.png");
	m_cloudmgTwoHandle = LoadGraph("bg/clouds_mg_1_lightened.png");
	m_cloudmgHandle = LoadGraph("bg/clouds_mg_2.png");
	m_cloudHandle = LoadGraph("bg/clouds_mg_3.png");
	m_glacialHandle = LoadGraph("bg/glacial_mountains.png");
	m_glacialWonHandle = LoadGraph("bg/glacial_mountains_lightened.png");
	m_skyHandle = LoadGraph("bg/sky.png");
	m_skyTwoHandle = LoadGraph("bg/sky_lightened.png");

	//縦のマス数は画面の高さ/マップチップ一つの
	//サイズで求められる
	m_graphChipNumX = graphW / kChipSize;
	m_graphChipNumY = graphH / kChipSize;
}

Bg::~Bg()
{
	DeleteGraph(m_mapHandle);
	DeleteGraph(m_bgHandle);
}

void Bg::Init()
{

}
void Bg::Update()
{
}
void Bg::Draw(const Camera& camera)
{
	DrawBg(camera);
}

void Bg::DrawBg(const Camera& camera)
{

	// スクリーン変換オフセット（ワールド→画面への平行移動）
	const Vec2 ofs = camera.GetCameraOffset();
	const int ox = static_cast<int>(ofs.x);
	const int oy = static_cast<int>(ofs.y);



	// 背景の描画
	//DrawExtendGraph(0 , 0 , Game::kScreenWidth , Game::kScreenHeight, m_bgHandle, true);
	//一番奥
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_skyHandle, true);
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_skyTwoHandle, true);
	//二番目に表示


	DrawExtendGraph(
		0 , 0 ,
		Game::kScreenWidth,
		Game::kScreenHeight,
		m_cloudLonelyHandle,
		true
	);



	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_cloudbgHandle, true);
	
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_glacialWonHandle, true);
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_glacialHandle, true);
	

	
	
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_cloudHandle, true);
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_cloudmgHandle, true);
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_cloudmgTwoHandle, true);
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_cloudmgWonHandle, true);

}


