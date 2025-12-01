#include "Stage.h"
#include "DxLib.h"
#include <cassert>
#include <algorithm>//listとか色々ギュッとしてある

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

const std::vector<uint8_t>& Stage::GetAllData() const
{
	return m_data;
}
