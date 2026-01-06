#pragma once
class ResultData
{
public:
	static void SetScore(int score) { m_score = score; }
	static int GetScore() { return m_score; }

	//スコアのクリア
	static void ClearScore() { m_score = 0; }

private:
	static int m_score;

};

