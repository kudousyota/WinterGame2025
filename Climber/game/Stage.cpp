#include "Stage.h"
#include "DxLib.h"
#include <algorithm>//listとか色々ギュッとしてある

namespace
{
	//配置に必要な情報
	struct DataSetting
	{
		uint8_t chipW; //チップ幅
		uint8_t chipH; //チップ高さ
		uint8_t layerCount;//レイヤー数
		uint8_t bitCount;//1チップに何ビット使用
	};
	struct DataHeader
	{
		char identifier[4];//正しければ'FMF_'になっている
		uint32_t size; //データサイズ
		uint32_t width;//データの幅
		uint32_t height; //データの高さ
		DataSetting setting;	// データ配置に必要な情報
	};
}

void Stage::Load(int stageNo)
{
	wchar_t filePath[32]; //ファイル名を入れる変数
	wsprintf(filePath, "data/stage%d.fmf", stageNo);
	auto handle = FileRead_open(filePath);
	DataHeader hander;//ヘッダ情報を入れる
	FileRead_read(&hander, sizeof(handle), handle);

	
}

Size Stage::MapSize() const
{
	return Size();
}

uint8_t Stage::GetData(int xidx, int yidx)
{
	return 0;
}

const std::vector<uint8_t>& Stage::GetAllData() const
{
	// TODO: return ステートメントをここに挿入します
}
