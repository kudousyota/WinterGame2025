#include "Bg.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
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
	m_mapHandle = LoadGraph("data/mapChip.png");
	//画像のマップチップ数を数える
	int graphW = 0;
	int graphH = 0;
	GetGraphSize(m_mapHandle, &graphW, &graphH);
	
	m_bgHandle = LoadGraph("data/bg.png");

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
	Vec2 cameraPos = camera.GetCameraOffset();

	/*DrawRectRotaGraph(m_pos.x, m_pos.y ,
		m_src.x,m_src.y,
		kMapWidth,kMapHeight,
		0.0f, 1.0f,
		m_bgHandle, false);*/

	// 背景の描画
	DrawExtendGraph(0 +camera.GetCameraOffset().x, 0 + camera.GetCameraOffset().y, Game::kScreenWidth + camera.GetCameraOffset().x, Game::kScreenHeight + camera.GetCameraOffset().y, m_bgHandle, true);
}




