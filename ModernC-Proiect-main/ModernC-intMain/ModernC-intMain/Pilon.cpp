#include "Pilon.h"

Pilon::Pilon()
	:IPiece()
{
}

Pilon::Pilon(const IPiece::PlayerColor& color, const Position& position, const int& index)
	:IPiece(color),
	m_position{ position },
	m_index{ index }
{
}

Pilon::Position Pilon::GetPosition() const
{
	return m_position;
}

void Pilon::SetPosition(const Position& position)
{
	m_position = position;
}

size_t Pilon::GetRow() const
{
	return m_position.first;
}

void Pilon::SetRow(const size_t& row)
{
	m_position.first = row;
}

size_t Pilon::GetColumn() const
{
	return m_position.second;
}

void Pilon::SetColumn(const size_t& col)
{
	m_position.second = col;
}

IPiece::PlayerColor Pilon::GetColor() const
{
	return m_color;
}

void Pilon::SetColor(const IPiece::PlayerColor& color)
{
	m_color = color;
}

std::vector<int> Pilon::GetAdjacentPilons() const
{
	return m_adjacentPilonsIndexes;
}

void Pilon::AddAdjacentPilon(const int& pilonIndex)
{
	m_adjacentPilonsIndexes.push_back(pilonIndex);
}

int Pilon::GetIndex() const
{
	return m_index;
}

void Pilon::SetIndex(const int& index)
{
	m_index = index;
}

bool Pilon::IsInRedBaseTop()
{
	auto& [row, col] = m_position;
	return row == 0;
}

bool Pilon::IsInRedBaseBottom(int boardSize)
{
	auto& [row, col] = m_position;
	return row == boardSize - 1;
}

bool Pilon::IsInBlueBaseLeft()
{
	auto& [row, col] = m_position;
	return col == 0;
}

bool Pilon::IsInBlueBaseRight(int boardSize)
{
	auto& [row, col] = m_position;
	return col == boardSize - 1;
}
