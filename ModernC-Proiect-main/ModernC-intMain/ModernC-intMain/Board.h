#pragma once
#include"IPiece.h"
#include "Pilon.h"
#include "Bridge.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <iosfwd>
#include <vector>
#include <cstdint>
#include <Windows.h>
#include <array>
#include <queue>
#include <QPoint>
#include <fstream>

class Board {
public:
	Board();
	Board(uint16_t size);
	Board(const Board& copy);
	Board& operator=(const Board& copy) = default;
	~Board() = default;

	void SetBoardSize(const uint16_t& size);
	uint16_t GetBoardSize() const;
	void Reset();

	IPiece* GetCell(const int& row, const int& col) const;
	std::vector<std::vector<IPiece*>> GetBoard()const;

	const bool IsInBoard(const Pilon::Position& pos) const;
	const bool IsCorner(const Pilon::Position& pos) const;
	std::vector<IPiece*> AdjacentPilons(const Pilon::Position& currentPos, std::reference_wrapper<Player> activePlayer) const;
	int PlacePilon(const Pilon::Position& posPilon, std::reference_wrapper<Player> pickingPlayer);
	void PlaceBridge(IPiece* firstPilon, IPiece* secondPilon, std::reference_wrapper<Player> pickingPlayer);
	bool HasConnection() const;

	bool IsPilon(const Pilon::Position& pos) const; 
	bool IsBridge(IPiece* f) const;
	bool IsPiece(IPiece* f) const;
	bool IsRedBase(Pilon::Position pos) const;
	bool IsBlueBase(Pilon::Position pos) const;

	std::vector<IPiece*> GetBridges() const;

	IPiece::PlayerColor GetColor(const Pilon::Position& pos);
protected:
	uint16_t m_size;
	std::vector<std::vector<IPiece*>> m_board;
	std::vector<IPiece*> m_allBridges;
};