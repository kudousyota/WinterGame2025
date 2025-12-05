#pragma once
#include "Rect.h"
#include <vector>


/// <summary>
/// ステージデータを管理する
/// </summary>
class Camera;
class Rect;
class Stage
{
	Size m_dataSize;//データサイズ
	std::vector<uint8_t> m_data;
public:
	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="stageNo">ステージの番号</param>
	void Load(int stageNo);
	Size MapSize()const;
	//ゲッター
	uint8_t GetData(int xidx, int yidx);
	//マップサイズ取得
	Size GetMapSize()const { return m_dataSize; }
	int GetWidth()const { return m_dataSize.w; }
	int GetHeight()const { return m_dataSize.h; }
	//チップサイズ取得
	int GetChipW()const { return m_chipNumW; }
	int GetChipH()const { return m_chipNumH; }


	bool IsCollision(const Rect& other,Rect& hitTileRect)const;

	const std::vector<uint8_t>& GetAllData() const;
	void SetTileSet(int chipHandle, int chipNumW,int chipNumH);
	void Draw(const Camera& camera, int originX, int originY)const;

private:
	// FMFのマップサイズ
	int m_width = 0;
	int m_height = 0;

	//表示用のタイルID
	//std::vector<uint8_t> m_data;

	//描画に必要な情報
	int m_chipHandle = -1;
	int m_chipNumW = 0;
	int m_chipNumH = 0;
};

