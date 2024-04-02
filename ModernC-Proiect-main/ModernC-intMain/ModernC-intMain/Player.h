#pragma once
#include <string>
#include<vector>
#include "IPiece.h"
#include "Pilon.h"
#include "Bridge.h"
#include <queue>

class Player
{
private:
	IPiece::PlayerColor m_teamColor;
	uint16_t m_pilonCounter;
	uint16_t m_bridgeCounter;
	std::string m_playerName;
	std::vector <IPiece*> m_pilons;
	std::vector <IPiece*> m_bridges;

public:
	Player(IPiece::PlayerColor teamColor);
	Player(std::string playerName, IPiece::PlayerColor teamColor);
	Player& operator=(const Player& copy) = default;
	Player(const Player& copy) = default;
	~Player() = default;

	IPiece::PlayerColor GetColor() const;
	int GetPilonCounter() const;
	int GetBridgeCounter() const;
	std::string GetPlayerName() const;

	void SetColor(const IPiece::PlayerColor& color);
	void SetPlayerName(const std::string& name);
	void SetBridgeCounter(const int& count);
	void SetPilonCounter(const int& count);

	void DecrementPilons();
	void SubtractBridges(const int& nBridges);

	void AddPilon(IPiece* pilon);
	void AddBridge(IPiece* firstPilon, IPiece* secondPilon);
	void MakeAdjacent(const int& firstIndex, const int& secondIndex);

	std::vector<IPiece*> GetPilons() const;
	std::vector<IPiece*> GetBridges() const;

	IPiece* GetLastBridge() const;

	bool Win(const int& boardSize = 24);
	bool HasPilonsInBothBases(std::vector<int> pilonsInPath, const int& boardSize = 24);
};