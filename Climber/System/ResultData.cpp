#include "ResultData.h"

int ResultData::m_score = 0;

void ResultData::SetScore(int score)
{
	m_score = score;
}

int ResultData::GetScore()
{
	return m_score;
}