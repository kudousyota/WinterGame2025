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

	//マップ全体の幅
	constexpr float kMapWidth = 1280.0f;
	//マップ全体の高さ
	constexpr float kMapHeight = 5000.0f;


	//チップを置く数
	constexpr int kChipNumX = 12;
	constexpr int kChipNumY = kMapHeight /kChipSize ;

	constexpr int kChipData[kChipNumX][kChipNumY] =
	{
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,0,0,0,0,0,0,0,0,0},
	};
		
}

Bg::Bg():
	m_pos{0,0},
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
	DrawMapChip(camera);
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
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_bgHandle,true);
}

void Bg::DrawMapChip(const Camera& camera)
{
	// マップチップの描画
	Vec2 cameraPos = camera.GetCameraOffset();

	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// マップチップIDを取得
			int chipID = kChipData[x][y];
			if (chipID == 0) continue;

			// マップチップ画像の切り抜き位置を計算
			int chipSrcX = (chipID % m_graphChipNumX) * kChipSize;
			int chipSrcY = (chipID / m_graphChipNumX) * kChipSize;
			// 描画位置を計算
			int drawX = static_cast<int>(m_pos.x + x * kChipSize * kChipScale - cameraPos.x);
			int drawY = static_cast<int>(m_pos.y + y * kChipSize * kChipScale - cameraPos.y);

			DrawRectRotaGraph(
				drawX, drawY,
				chipSrcX, chipSrcY,
				kChipSize, kChipSize,
				kChipScale, 0.0f,
				m_mapHandle, true);
		}
	}

}

