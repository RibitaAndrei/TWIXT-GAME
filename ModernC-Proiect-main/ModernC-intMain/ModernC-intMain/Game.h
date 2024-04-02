#pragma once
#include "Player.h"
#include "Board.h"
#include "ScoreManager.h"
#include "Constants.h"
#include <iostream>
#include <fstream>

class Game
{
public:
	enum class GameState
	{
		Win,
		Draw,
		None
	};

	Game();
	Game(uint16_t boardSize);
	Game(uint16_t boardSize, std::string playerName1, std::string playerName2);
	Game& operator=(const Game& copy) = default;
	Game(const Game& copy);
	~Game();

	bool IsPilon(const Pilon::Position& pos);

	IPiece::PlayerColor GetColor(const Pilon::Position& pos);

	void SetBoardSize(int size);
	bool IsCorner(const Pilon::Position& pos);

	int GetBoardSize() const;

	Player GetFirstPlayer() const;
	Player GetSecondPlayer() const;

	void RecordMove(const std::string& playerName, const Pilon::Position& coordinates);

	void PlacePilon(const Pilon::Position& coordinates);
	void PlacePilon(Pilon* p, std::unique_ptr<Player>& player);

	void SwapPlayers();

	void ReadPlayersAndBoard(std::string playerName, int& boardSize);
	bool CheckWinCondition() const;
	bool NoMorePilons() const;

	std::vector<IPiece*> GetBridges() const;

	std::reference_wrapper<Player> GetPickingPlayer() const;
	std::reference_wrapper<Player> GetWaitingPlayer() const;

	void SetPickingPlayer(const std::reference_wrapper<Player>& pickingPlayer);
	void SetWaitingPlayer(const std::reference_wrapper<Player>& waitingPlayer);

	void SaveGame(const std::string& fileName);
	void LoadGame(const std::string& fileName);

	void SetPilonsAndBridges(const int& nPilons, const int& nBridges);

	bool IncorrectValues() const;

	int GetTurn() const;

	void SwitchSides();

private:
	std::unique_ptr<Player> m_player1Ptr;
	std::unique_ptr<Player> m_player2Ptr;
	std::reference_wrapper<Player> m_pickingPlayer;
	std::reference_wrapper<Player> m_waitingPlayer;
	Board m_gameBoard;
	GameState m_state;
	int m_turn;

	struct Move {
		std::string playerName;
		Pilon::Position coordinates;

		Move(const std::string& name, const Pilon::Position& pos)
			: playerName(name), coordinates(pos) {}
	};

	std::vector<Move> moveHistory;
};