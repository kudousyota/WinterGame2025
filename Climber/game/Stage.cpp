#include "Stage.h"
#include <algorithm> //listとか色々ギュッとしてある
#include "DxLib.h"
#include "Camera.h"
#include "Game.h"
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
	//filepathに第二引数で作られた文字列を入れる
	sprintf_s(filePath, "data/stage%d.fmf", stageNo);
	//filePathに保存されたファイル名のファイルを開き、そのハンドルに取得
	auto handle = FileRead_open(filePath);
	if (handle <= 0)
	{
		OutputDebugString("Stage::Load - Failed to open FMF file\n");
		assert(handle > 0);
		return;
	}

	//ヘッダ情報を入れる
	DataHeader header;
	FileRead_read(&header, sizeof(header), handle);

	// ヘッダ簡易検査
	if (header.width == 0 || header.height == 0)
	{
		OutputDebugString("Stage::Load - Invalid header width/height\n");
		FileRead_close(handle);
		return;
	}

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
		OutputDebugString("Stage::Load - FMF data length mismatch\n");
		FileRead_close(handle);
		return;
	}

	// m_data を正しいサイズで確保（幅×高さ）
	m_data.clear();
	m_data.resize(expectedCount);

	// データは上から下になっているが、描画のために反転して格納（下→上）
	for (uint32_t y = 0; y < header.height; ++y)
	{
		uint32_t reverseY = header.height - y - 1;
		// コピー元とコピー先のポインタを計算してコピー
		const uint8_t* src = rawData.data() + static_cast<size_t>(y) * header.width;
		uint8_t* dst = m_data.data() + static_cast<size_t>(reverseY) * header.width;
		std::copy_n(src, header.width, dst);
	}
	// ファイルを閉じる
	FileRead_close(handle);
	OutputDebugString("Stage::Load - FMF loaded successfully\n");
}
// マップサイズ取得
Size Stage::MapSize() const
{
	return m_dataSize;
}

uint8_t Stage::GetData(int xidx, int yidx)
{
	// IDを取得して範囲外なら0を返す
	if (xidx < 0 || yidx < 0 || xidx >= m_dataSize.w || yidx >= m_dataSize.h) return 0;
	return m_data[static_cast<size_t>(yidx) * m_dataSize.w + xidx];
}

bool Stage::IsCollision(const Rect& other, Rect& hitTileRect) const
{
	// 衝突判定//未実装
	
	return false;
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
	OutputDebugString("Stage::SetTileSet called\n");
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
	// 1行・1列あたりのタイル数を計算
	const int tilesPerRow = (m_chipNumW > 0) ? texW / m_chipNumW : 0;
	const int tilesPerCol = (m_chipNumH > 0) ? texH / m_chipNumH : 0;
	const int totalTiles = tilesPerRow * tilesPerCol;
	if (tilesPerRow <= 0 || tilesPerCol <= 0) return;
	// マップデータサイズ取得
	const int w = m_dataSize.w;
	const int h = m_dataSize.h;
	if (w <= 0 || h <= 0) return;
	// データサイズと m_data のサイズが一致しない場合は描画しない
	if (static_cast<size_t>(w) * static_cast<size_t>(h) != m_data.size())
	{
		OutputDebugString("Stage::Draw - m_data size mismatch\n");
		// 続行しない
		return;
	}

	// 描画座標 = ワールド − カメラ
	// カメラのオフセットを取得
	const auto camOfs = camera.GetCameraOffset();
	const int baseX = originX + static_cast<int>(camOfs.x);
	const int baseY = originY + static_cast<int>(camOfs.y);

	// 画面に見える縦行だけ描画（縦スクロール最適化）
	const int screenH = Game::kScreenHeight;
	// スクリーンに映っている行の最小値と最大値を計算し、マップ範囲内にクランプ
	int minRow = (0 - baseY) / m_chipNumH;
	int maxRow = (screenH - 1 - baseY) / m_chipNumH;
	//画面が上にずれている場合の補正
	if (minRow < 0) minRow = 0;
	if (maxRow < 0) maxRow = -1; // 画面外
	if (maxRow >= h) maxRow = h - 1;
	if (minRow > h - 1) return; // 全部画面外
	// 各行をループ
	for (int y = minRow; y <= maxRow; ++y)
	{
		// 各列をループ
		for (int x = 0; x < w; ++x)
		{
			// 安全にインデックスを取得
			size_t idx = static_cast<size_t>(y) * w + static_cast<size_t>(x);
			if (idx >= m_data.size()) continue;
			// タイルIDを取得
			uint8_t id = m_data[idx];

			// 0=空 の前提（FMFが1始まりなら tileIndex = id - 1 に変更）
			if (id == 0) continue;
			// タイルインデックスを計算
			int tileIndex = static_cast<int>(id);
			// tileIndex の変換が必要ならここで行う（例: FMF が 1始まりなら --tileIndex;）
			if (tileIndex < 0 || tileIndex >= totalTiles) continue;
			// チップセット内の描画位置を計算
			const int srcX = (tileIndex % tilesPerRow) * m_chipNumW;
			const int srcY = (tileIndex / tilesPerRow) * m_chipNumH;
			const int dstX = baseX + x * m_chipNumW;
			const int dstY = baseY + y * m_chipNumH;

			DrawRectGraph(dstX, dstY, srcX, srcY, m_chipNumW, m_chipNumH, m_chipHandle, true);
			DrawBox(dstX, dstY, dstX + m_chipNumW, dstY + m_chipNumH, GetColor(255, 0, 0), false);
		}
	}
}