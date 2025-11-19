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
	constexpr int kChipScale = 1.0f;

	//マップ全体の幅
	constexpr float kMapWidth = 1280.0f;
	//マップ全体の高さ
	constexpr float kMapHeight = 5000.0f;


	//チップを置く数
	constexpr int kChipNumX = 12;
	constexpr int kChipNumY = Game::kScreenHeight /kChipSize ;

	constexpr int kChipData[kChipNumX][kChipNumY] =
	{
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
	m_graphChipNumX = graphH / kChipSize;
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
	DrawMapChip();
}

void Bg::DrawBg(const Camera& camera)
{
	DrawExtendGraph(m_pos.x, m_pos.y,
		m_src.x,m_src.y,
		m_bgHandle, false);
}

void Bg::DrawMapChip()
{
	/*for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			int posX = static_cast<int>(x *kChipSize * kChipScale - )
		}
	}*/
	
	//DrawRectRotaGraph(, 0, m_mapHandle, true);
}

