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
		//データサイズ
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
	//ファイルが開けなかった場合、強制終了させる
	assert(handle > 0);
	//ヘッダ情報を入れる
	DataHeader header;

	FileRead_read(&header, sizeof(header), handle);
	//利用者側から参照できるように内部変数にコピー
	m_dataSize.w = header.width;
	m_dataSize.h = header.height;

	//データを受け取る準備をする
	//ヘッダにあるデータのサイズを取得して配列のサイズを変更
	m_data.resize(header.size);
	//生データ
	std::vector<uint8_t>rawData;
	//二行上と同じ
	rawData.resize(header.size);
	FileRead_read(rawData.data(), rawData.size() * sizeof(uint8_t), handle);
	//データは上から下になっているけどしたから読み込みたいので反転させる
	//for文で一行ごと回していく
	for (int y = 0; y < header.height; y++)
	{
		//行を反転させる
		int reverseY = header.height - y - 1;
		//一行ごとにコピーしていく
		//生データのほうは順番行アクセス//コピー元の先頭アドレス
		std::copy_n(&rawData[y * header.width],
			//データ数(その行にいくつデータがあるのか←横一列のデータ数)//コピーする個数
			header.width,
			//コピー先は逆順アクセス//コピー先の先頭アドレス
			&m_data[reverseY * header.width]);
	}
	//読み込み終わったらファイルを閉じる
	FileRead_close(handle);
}

Size Stage::MapSize() const
{
	return m_dataSize;
}

uint8_t Stage::GetData(int xidx, int yidx)
{
	return m_data[yidx * m_dataSize.w + xidx];
}

bool Stage::IsCollision(const Rect& other, Rect& hitTileRect) const
{
	return false;
}

const std::vector<uint8_t>& Stage::GetAllData() const
{
	return m_data;
}

void Stage::SetTileSet(int chipHandle, int chipNumW, int chipNumH)
{
	m_chipHandle = chipHandle;
	if (chipNumW > 0) m_chipNumW = chipNumW;   // 明示値があれば上書き
	if (chipNumH > 0) m_chipNumH = chipNumH;

}


void Stage::Draw(const Camera& camera, int originX, int originY)const
{

	if (m_chipHandle == -1 || m_chipNumW <= 0 || m_chipNumH <= 0) return;

	int texW = 0, texH = 0;
	GetGraphSize(m_chipHandle, &texW, &texH);
	const int tilesPerRow = (m_chipNumW > 0) ? texW / m_chipNumW : 0;
	const int tilesPerCol = (m_chipNumH > 0) ? texH / m_chipNumH : 0;
	const int totalTiles = tilesPerRow * tilesPerCol;
	if (tilesPerRow <= 0 || tilesPerCol <= 0) return;

	const int w = m_dataSize.w;
	const int h = m_dataSize.h;

	// 描画座標 = ワールド − カメラ
	const auto camOfs = camera.GetCameraOffset();
	const int baseX = originX - static_cast<int>(camOfs.x);
	const int baseY = originY - static_cast<int>(camOfs.y);

	// 画面に見える縦行だけ描画（縦スクロール最適化）
	const int screenH = Game::kScreenHeight;

	int minRow = (0, (0 - baseY) / m_chipNumH);
	int maxRow = (h - 1, (screenH - 1 - baseY) / m_chipNumH);

	for (int y = minRow; y <= maxRow; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			uint8_t id = m_data[y * w + x];
	
		 // ★ 0=空 の前提（FMFが1始まりなら tileIndex = id - 1 に変更）
			if (id == 0) continue;

			int tileIndex = static_cast<int>(id);
			if (tileIndex < 0 || tileIndex >= totalTiles) continue;

			const int srcX = (tileIndex % tilesPerRow) * m_chipNumW;
			const int srcY = (tileIndex / tilesPerRow) * m_chipNumH;
			const int dstX = baseX + x * m_chipNumW;
			const int dstY = baseY + y * m_chipNumH;

			DrawRectGraph(dstX, dstY, srcX, srcY, m_chipNumW, m_chipNumH, m_chipHandle, true);

		}
	}
}
