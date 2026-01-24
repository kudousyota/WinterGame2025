#include "Stage.h"
#include <algorithm> //listとか色々ギュッとしてある
#include "DxLib.h"
#include "../System/Camera.h"
#include "../System/Game.h"
#include <cmath>
#include <cassert>

namespace
{
	//配置に必要な情報
	struct DataSetting
	{
		//チップ幅
		uint8_t chipW;
		//チップ高さ
		uint8_t chipH;
		//レイヤー数
		uint8_t layerCount;
		//1チップに何ビット使用
		uint8_t bitCount;
	};
	struct DataHeader
	{
		//正しければ'FMF_'になっている
		char identifier[4];
		//データサイズ（バイト数）
		uint32_t size;
		//データの幅
		uint32_t width;
		//データの高さ
		uint32_t height;
		// データ配置に必要な情報
		DataSetting setting;
	};
}

void Stage::Load(int stageNo)
{
	//ファイル名を入れる変数
	char filePath[32];
	//char filePathTwo[32];
	//filepathに第二引数で作られた文字列を入れる
	sprintf_s(filePath, "data/stage%d.fmf", stageNo);
	//sprintf_s(filePathTwo, "data/stage%d.fmf", stageNo);
	//filePathに保存されたファイル名のファイルを開き、そのハンドルに取得
	auto handle = FileRead_open(filePath);
	//auto handleTwo = FileRead_open(filePathTwo);
	if (handle <= 0)
	{
		assert(handle > 0);
		return;
	}

	//ヘッダ情報を入れる
	DataHeader header;
	FileRead_read(&header, sizeof(header), handle);

	// ヘッダ簡易検査
	if (header.width == 0 || header.height == 0)
	{
		FileRead_close(handle);
		return;
	}

	const uint32_t width = header.width;
	const uint32_t height = header.height;

	//利用者側から参照できるように内部変数にコピー
	m_dataSize.w = static_cast<int>(header.width);
	m_dataSize.h = static_cast<int>(header.height);

	// expected データ数（幅 * 高さ）
	const size_t expectedCount = static_cast<size_t>(header.width) * static_cast<size_t>(header.height);

	// 生データ読み込み
	std::vector<uint8_t> rawData;
	rawData.resize(static_cast<size_t>(header.size));
	FileRead_read(rawData.data(), rawData.size() * sizeof(uint8_t), handle);

	// 生データのサイズが期待と合わない場合は警告して早期終了
	if (rawData.size() < expectedCount)
	{
	
		FileRead_close(handle);
		return;
	}

	// m_data を正しいサイズで確保（幅×高さ）
	m_data.clear();
	m_data.resize(expectedCount);

	// データは上から下になっているが、描画のために反転して格納（下→上）
	for (int y = 0; y < header.height; ++y)
	{
		int reverseY = header.height - y - 1;
		// コピー元とコピー先のポインタを計算してコピー
		const uint8_t* src = rawData.data() + static_cast<size_t>(y) * header.width;
		//yにするとすっごい下にいる
		uint8_t* dst = m_data.data() + static_cast<size_t>(y) * header.width;
		std::copy_n(src, header.width, dst);
	}
	// ファイルを閉じる
	FileRead_close(handle);
}
// マップサイズ取得
Size Stage::MapSize() const
{
	return m_dataSize;
}

uint8_t Stage::GetData(int xidx, int yidx)const
{
	// IDを取得して範囲外なら0を返す
	if (xidx < 0 || yidx < 0 || xidx >= m_dataSize.w || yidx >= m_dataSize.h) return 0;
	return m_data[static_cast<size_t>(yidx) * m_dataSize.w + xidx];
}

bool Stage::IsCollision(const Rect& other, Rect& hitTileRect) const
{
	// 衝突判定
	// タイル幅・高さ
	const int tileW = m_chipNumW;
	const int tileH = m_chipNumH;

	// 対象の辺の座標（Rect が中心基準で実装されている前提）
	const float left = other.GetLeft();
	const float right = other.GetRight();
	const float top = other.GetTop();
	const float bottom = other.GetBottom();

	
	const float eps = 0.001f; // 1e-4f～1e-3f くらいでOK

	int ix0 = static_cast<int>(std::floor(left / tileW));
	int ix1 = static_cast<int>(std::floor((right - eps) / tileW));
	int iy0 = static_cast<int>(std::floor(top / tileH));
	int iy1 = static_cast<int>(std::floor((bottom - eps) / tileH));

	// クランプは現状のままでOK
	if (ix0 < 0) ix0 = 0;
	if (iy0 < 0) iy0 = 0;
	if (ix1 >= m_dataSize.w) ix1 = m_dataSize.w - 1;
	if (iy1 >= m_dataSize.h) iy1 = m_dataSize.h - 1;

	for (int y = iy0; y <= iy1; ++y)
	{
		for (int x = ix0; x <= ix1; ++x)
		{
			uint8_t id = GetData(x, y);
			if (id == 0) continue;

			float tileCenterX = x * tileW + tileW * 0.5f;
			float tileCenterY = y * tileH + tileH * 0.5f;
			hitTileRect.Init(tileCenterX, tileCenterY, (float)tileW, (float)tileH);
			return true;
		}
	}
	return false;
}


void Stage::SetTile(int xidx, int yidx, uint8_t id)
{
	if (xidx < 0 || yidx < 0 || xidx >= m_dataSize.w || yidx >= m_dataSize.h) return;
	m_data[static_cast<size_t>(yidx) * m_dataSize.w + xidx] = id;
}


const std::vector<uint8_t>& Stage::GetAllData() const
{
	// 全データを返す
	return m_data;
}

void Stage::SetTileSet(int chipHandle, int chipNumW, int chipNumH)
{
	// 1チップ情報を設定
	m_chipHandle = chipHandle;
	if (chipNumW > 0) m_chipNumW = chipNumW;   
	if (chipNumH > 0) m_chipNumH = chipNumH;
}

void Stage::Draw(const Camera& camera, int originX, int originY) const
{
	// マップチップが設定されていない場合は描画しない
		if (m_chipHandle == -1 || m_chipNumW <= 0 || m_chipNumH <= 0)
		{
			// タイルセット未設定
			return;
		}
	// チップセット画像のサイズを取得
	int texW = 0, texH = 0;
	GetGraphSize(m_chipHandle, &texW, &texH);
	if (texW <= 0 || texH <= 0) return;

	// テクスチャ上の 1 チップのピクセルサイズ
	const int tilePixel = m_chipPixelSize;  // 通常は 16
	// テクスチャ上の横方向のタイル数
	const int tilesPerRow = texW / tilePixel;

	// 1行・1列あたりのタイル数を計算
	const int w = m_dataSize.w;
	const int h = m_dataSize.h;
	if (w <= 0 || h <= 0) return;

	// 描画スケール（画面上の表示サイズ / テクスチャ上のチップサイズ）
	const float scale = (tilePixel > 0) ? static_cast<float>(m_chipNumW) / static_cast<float>(tilePixel) : 1.0f;

	// カメラオフセット
	const Vec2 cameraOffset = camera.GetCameraOffset();

	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			const uint8_t id = m_data[x + w * y];
			if (id == 0) continue; // 空タイルは描画しない

			// テクスチャ上の切り出し座標
			const int srcX = tilePixel * (id % tilesPerRow);
			const int srcY = tilePixel * (id / tilesPerRow);

			// 描画先（ワールド座標 -> カメラオフセット）
			const int dstX = x * m_chipNumW + cameraOffset.x;
			const int dstY = y * m_chipNumH + cameraOffset.y;

			const int centerX = dstX + m_chipNumW / 2;
			const int centerY = dstY + m_chipNumH / 2;

			// 切り出すサイズはテクスチャ上の tilePixel、描画は scale で拡大
			DrawRectRotaGraph(centerX, centerY, srcX, srcY,
				tilePixel, tilePixel,
				scale, 0, m_chipHandle, true);

#ifdef _DEBUG
			DrawBox(dstX, dstY, dstX + m_chipNumW, dstY + m_chipNumH,
				GetColor(0, 255, 0),
				false);
#endif
		}
	}

	
}

bool Stage::GetTileUV(int tileId, int& sx, int& sy, int& sw, int& sh) const
{
	if (m_chipHandle < 0 || m_chipPixelSize <= 0) return false;

	int texW = 0, texH = 0;
	GetGraphSize(m_chipHandle, &texW, &texH);
	if (texW <= 0 || texH <= 0) return false;

	const int tilePixel = m_chipPixelSize;     // 例：16
	const int tilesPerRow = texW / tilePixel;  // 1行あたりのタイル枚数
	if (tilesPerRow <= 0) return false;

	// id == 0 は空タイルとして扱っているなら UV を返さない
	if (tileId <= 0) return false;

	// Draw と同じ ID → UV の計算
	sx = tilePixel * (tileId % tilesPerRow);
	sy = tilePixel * (tileId / tilesPerRow);
	sw = tilePixel;
	sh = tilePixel;

	// 一応範囲チェック
	if (sx < 0 || sy < 0 || (sx + sw) > texW || (sy + sh) > texH) return false;

	return true;
}
