#pragma once
class ResultData
{
public:
	static void SetScore(int score);
	static int GetScore();

private:
	static int m_score;

};

