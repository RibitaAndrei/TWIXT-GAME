#include "ScoreManager.h"

ScoreManager::ScoreManager()
{
	m_scorePlayer1 = 0;
	m_scorePlayer2 = 0;
}

int ScoreManager::GetScorePlayer1() const
{
	return m_scorePlayer1;
}

int ScoreManager::GetScorePlayer2() const
{
	return m_scorePlayer2;
}

void ScoreManager::SetScorePlayer1(int score)
{
	m_scorePlayer1 = score;
}

void ScoreManager::SetScorePlayer2(int score)
{
	m_scorePlayer2 = score;
}

void ScoreManager::AddScorePlayer1()
{
	m_scorePlayer1++;
}

void ScoreManager::AddScorePlayer2()
{
	m_scorePlayer2++;
}