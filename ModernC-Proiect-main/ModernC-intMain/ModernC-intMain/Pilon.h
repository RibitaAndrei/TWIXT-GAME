#pragma once
#include <utility>
#include <vector>
#include "IPiece.h"

class Pilon : public IPiece
{
public:
	using Position = std::pair<size_t, size_t>;

	Pilon();
	Pilon(const IPiece::PlayerColor& color, const Position& position, const int& index);
	Pilon& operator=(const Pilon& copy) = default;
	Pilon(const Pilon& copy) = default;
	~Pilon() = default;

	Position GetPosition() const;
	void SetPosition(const Position& position);

	size_t GetRow() const;
	void SetRow(const size_t& row);

	size_t GetColumn() const;
	void SetColumn(const size_t& col);

	virtual IPiece::PlayerColor GetColor() const override;
	virtual void SetColor(const IPiece::PlayerColor& color) override;

	std::vector<int> GetAdjacentPilons() const;
	void AddAdjacentPilon(const int& p);

	int GetIndex() const;
	void SetIndex(const int& index);

	bool IsInRedBaseTop();
	bool IsInRedBaseBottom(int boardSize = 24);
	bool IsInBlueBaseLeft();
	bool IsInBlueBaseRight(int boardSize = 24);

private:
	Position m_position;
	std::vector<int> m_adjacentPilonsIndexes;
	int m_index;
};