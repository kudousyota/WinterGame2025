#include "ResultData.h"

int ResultData::m_score = 0;
int ResultData::m_killCount = 0;

void ResultData::SetScore(int score)
{
	m_score = score;
}

int ResultData::GetScore()
{
	return m_score;
}

void ResultData::SetKillCount(int count)
{
	m_killCount = count;
}

int ResultData::GetKillCount()
{
	return m_killCount;
}
