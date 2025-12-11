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
		OutputDebugString("Stage::Load - FMF data length mismatch\n");
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
	//1マスの大きさ分
	int chipW = texW / m_chipPixelSize;
	int chipH = texH / m_chipPixelSize;
	// 1行・1列あたりのタイル数を計算
	const int w = m_dataSize.w;
	const int h = m_dataSize.h;
	const int totalTiles = w * h;
	// マップデータサイズ取得
	
	if (w <= 0 || h <= 0) return;

	// 描画座標 = ワールド − カメラ
	// カメラのオフセットを取得
	const Vec2 cameraOffset = camera.GetCameraOffset();
	// 画面に見える縦行だけ描画（縦スクロール最適化）
	const int screenH = Game::kScreenHeight;
	
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			const int srcX = m_chipPixelSize * (m_data[x + w * y] % chipW);
			const int srcY = m_chipPixelSize * (m_data[x + w * y] / chipW);
			const int dstX = x * m_chipNumW + cameraOffset.x;
			const int dstY = originX + cameraOffset.y + y * m_chipNumH;
			

			DrawRectRotaGraph(x * m_chipPixelSize+ camera.GetCameraOffset().x, y * m_chipPixelSize +camera.GetCameraOffset().y, srcX,srcY, 16, 16, 1.0f, 0, m_chipHandle, true);
			DrawBox(dstX , dstY, dstX + m_chipNumW, dstY + m_chipNumH, GetColor(255, 0, 0), false);
		}
	}
}