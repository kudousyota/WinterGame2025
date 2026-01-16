#pragma once
class ResultData
{
public:
	static void SetScore(int score);
	static int GetScore();
	static void SetKillCount(int count);
	static int GetKillCount();

private:
	static int m_score;
	static int m_killCount;

};

