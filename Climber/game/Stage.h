#pragma once
#include "Rect.h"
#include <vector>

/// <summary>
/// ステージデータを管理する
/// </summary>
class Stage
{
	Size m_dataSize;//データサイズ
	std::vector<uint8_t> m_data;

public:
	void Load(int stageNo);
	Size MapSize()const;

	uint8_t GetData(int xidx, int yidx);

	const std::vector<uint8_t>& GetAllData() const;
};

