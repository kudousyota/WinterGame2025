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
	constexpr int kChipData[kChipNumY][kChipNumX] =
	{
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
		{1,2,2,3,0,0,0,0,0,0,0,0,},
		{1,0,0,0,0,0,0,0,0,0,0,0,},
	};
		
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
	DrawExtendGraph(0 +camera.GetCameraOffset().x, 0 + camera.GetCameraOffset().y, Game::kScreenWidth + camera.GetCameraOffset().x, Game::kScreenHeight + camera.GetCameraOffset().y, m_bgHandle, true);
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
			int chipID = kChipData[y][x];
			if (chipID == 0) continue;

			// マップチップ画像の切り抜き位置を計算
			int chipSrcX = (chipID % m_graphChipNumX) * kChipSize;
			int chipSrcY = (chipID / m_graphChipNumX) * kChipSize;
			// 描画位置を計算
			int drawY = static_cast<int>(m_pos.y + y * kChipSize * kChipScale);
			int drawX = static_cast<int>(m_pos.x + x * kChipSize * kChipScale);
			//画面外は描画しない
			if (drawX < 0 - kChipSize)continue;
			if (drawX > kMapWidth)continue;
			if (drawY < 0 - kChipScale)continue;
			if (drawY > kMapHeight)continue;
		DrawRectRotaGraph(
				drawX + cameraPos.x, drawY + cameraPos.y,
				chipSrcX, chipSrcY,
				kChipSize, kChipSize,
				kChipScale, 0.0f,
				m_mapHandle, true);
#ifdef DEBUG
			// 当たり判定
			//DrawBoxAA(drawX, drawY, drawX + kChipSize * kChipScale, drawY + kChipSize * kChipScale, 0x00xxff, true);

#endif 

		}
		
	}

}

bool Bg::IsCollision(Rect rect, Rect& chipRect)
{
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			int chipLeft = static_cast<int>(x * kChipSize * kChipScale);
			int chipRight = static_cast<int>(chipLeft + kChipSize * kChipScale);
			int chipTop = static_cast<int>(y * kChipSize * kChipScale);
			int chipBottom = static_cast<int>(chipTop + kChipSize * kChipScale);

			//空チップはスキップ
			if (kChipData[y][x] == 0)continue;

		/*
		AABB判定は高速：四則演算と比較だけで判定できる
		 中心座標＋幅高さでも同じことができる：
		 左端 = 中心X - 幅 / 2
		 右端 = 中心X + 幅 / 2
		 上端 = 中心Y - 高さ / 2
		 下端 = 中心Y + 高さ / 2
		 */

			//絶対に当たらない場合
			if (chipLeft > rect.GetH())continue;
			if (chipTop > rect.GetY()) continue;
			if (chipRight < rect.GetX()) continue;
			if (chipBottom < rect.GetY()) continue;
			

			//ぶつかったマップチップの矩形を設定する
			chipRect.Set(chipLeft,chipTop,chipRight,chipBottom);

			return true;
		}
	}
	return false;
}

