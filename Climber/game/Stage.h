#pragma once
#include "../System/Rect.h"
#include <vector>
#include "../System/Geometry.h"
#include "../System/Vec2.h"


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
	uint8_t GetData(int xidx, int yidx)const;
	//マップサイズ取得
	Size GetMapSize()const { return m_dataSize; }
	int GetWidth()const { return m_dataSize.w; }
	int GetHeight()const { return m_dataSize.h; }

	//チップサイズ取得
	int GetChipW()const { return m_chipNumW; }
	int GetChipH()const { return m_chipNumH; }

	int GetTileBrokePoint()const { return m_tileBrokePoint; }

	void SetTileSet(int chipHandle, int chipNumW, int chipNumH);
	//指定された床を書き換える
	void SetTile(int xidx, int yidx, uint8_t id);
	bool IsCollision(const Rect& other,Rect& hitTileRect)const;
	const std::vector<uint8_t>& GetAllData() const;
	
	void Draw(const Camera& camera, int originX, int originY)const;


	// タイルセット画像ハンドルを返す
	int GetTilesetHandle() const { return m_chipHandle; }

	// テクスチャ上の1タイルのピクセルサイズ（例：16）
	int GetChipPixelSize() const { return m_chipPixelSize; }

	// タイルID -> タイルセット上のUV（sx, sy, sw, sh）を返す
	// 返り値: 取得成功なら true
	bool GetTileUV(int tileId, int& sx, int& sy, int& sw, int& sh) const;

	//破壊されたブロックの座標を記録
	void NotifyTileBroken(int tx, int ty);

	void GetAndClearBrokenCenters(std::vector<Vec2>& out);


private:
	// FMFのマップサイズ
	int m_width = 0;
	int m_height = 0;

	//表示用のタイルID
	//std::vector<uint8_t> m_data;
	//1フレーム分の破壊中心座標
	std::vector<Vec2> m_brokenCentersThisFrame;
	//タイル壊した時のポイント
	int m_tileBrokePoint = 10;

	//描画に必要な情報
	int m_chipHandle = -1;
	int m_chipNumW = 0;
	int m_chipNumH = 0;
	int m_chipPixelSize = 16;
};

