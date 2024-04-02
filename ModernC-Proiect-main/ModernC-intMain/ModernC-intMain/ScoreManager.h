#pragma once
class ScoreManager
{
private:
	int m_scorePlayer1;
	int m_scorePlayer2;
public:
	ScoreManager();
	int GetScorePlayer1() const;
	int GetScorePlayer2() const;

	void SetScorePlayer1(int score);
	void SetScorePlayer2(int score);

	void AddScorePlayer1();
	void AddScorePlayer2();
};

