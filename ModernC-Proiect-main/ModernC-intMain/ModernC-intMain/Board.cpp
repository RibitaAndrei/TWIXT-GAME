#include "board.h"
#include <iostream>
#include <Windows.h>
#include <stack>
#include <vector>


Board::Board()
{
}

Board::Board(uint16_t size)
	: m_size{ size }
{
	m_board.resize(m_size);
	for (int index = 0; index < m_size; index++)
		m_board[index].resize(m_size);
}

Board::Board(const Board& copy)
	: m_size{ copy.m_size },
	m_board{ copy.m_board }
{}

IPiece* Board::GetCell(const int& row, const int& col) const
{
	if (row >= 0 && row < m_size && col >= 0 && col < m_size)
	{
		return m_board[row][col];
	}
}

std::vector<std::vector<IPiece*>> Board::GetBoard() const
{
	return m_board;
}

bool Board::HasConnection() const
{
	// Matrice pentru a marca celulele vizitate
	std::vector<std::vector<bool>> visited(m_size, std::vector<bool>(m_size, false));

	// Stiva pentru a efectua DFS
	std::stack<std::pair<int, int>> stack;

	// Pornim DFS de la prima și ultima coloană
	for (int i = 0; i < m_size; ++i)
	{
		if (!visited[i][0] && IsBridge(m_board[i][0]))
		{
			stack.push({ i, 0 });
			visited[i][0] = true;
		}
		if (!visited[i][m_size - 1] && IsBridge(m_board[i][m_size - 1]))
		{
			stack.push({ i, m_size - 1 });
			visited[i][m_size - 1] = true;
		}
	}

	// Pornim DFS de la prima și ultima linie
	for (int j = 0; j < m_size; ++j)
	{
		if (!visited[0][j] && IsBridge(m_board[0][j]))
		{
			stack.push({ 0, j });
			visited[0][j] = true;
		}
		if (!visited[m_size - 1][j] && IsBridge(m_board[m_size - 1][j]))
		{
			stack.push({ m_size - 1, j });
			visited[m_size - 1][j] = true;
		}
	}

	// Direcții posibile pentru deplasare (sus, jos, stânga, dreapta)
	const std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

	// DFS
	while (!stack.empty())
	{
		auto [x, y] = stack.top();
		stack.pop();

		// Vizitează toate celulele adiacente
		for (const auto& dir : directions)
		{
			int newX = x + dir.first;
			int newY = y + dir.second;

			// Verifică dacă celula adiacentă este validă și nu a fost vizitată
			if (newX >= 0 && newX < m_size && newY >= 0 && newY < m_size && !visited[newX][newY] && IsBridge(m_board[newX][newY]))
			{
				stack.push({ newX, newY });
				visited[newX][newY] = true;
			}
		}
	}
	// Verifică dacă s-au vizitat toate celulele de pe margini
	for (int i = 0; i < m_size; ++i)
	{
		if (visited[i][0] && visited[i][m_size - 1])
			return true;
	}

	for (int j = 0; j < m_size; ++j)
	{
		if (visited[0][j] && visited[m_size - 1][j])
			return true;
	}

	return false;
}

const bool Board::IsCorner(const Pilon::Position& pos) const
{
	auto [row, col] = pos;
	if ((row == 0 && col == 0) ||
		(row == 0 && col == m_size - 1) ||
		(row == m_size - 1 && col == 0) ||
		(row == m_size - 1 && col == m_size - 1))
		return true;
	return false;
}

void Board::Reset()
{
	for (int i = 0; i < m_size; ++i) {
		for (int j = 0; j < m_size; ++j) {
			delete m_board[i][j];
		}
	}
}

int Board::PlacePilon(const Pilon::Position& posPilon, std::reference_wrapper<Player> pickingPlayer)
{
	auto& [row, col] = posPilon;

	if (!IsInBoard(posPilon))
		return 0;

	if (IsPilon(posPilon))
		return 0;

	m_board[row][col] = new Pilon(pickingPlayer.get().GetColor(), posPilon, pickingPlayer.get().GetPilons().size());
	pickingPlayer.get().AddPilon(m_board[row][col]);

	std::vector<IPiece*> positions = AdjacentPilons(posPilon, pickingPlayer);
	for (int index = 0; index < positions.size(); index++)
		PlaceBridge(m_board[row][col], positions[index], pickingPlayer);

	return positions.size();
}

bool DoBridgesIntersect(const QPoint& p1, const QPoint& p2, const QPoint& p3, const QPoint& p4)
{
	QPoint vectorBridge1 = p2 - p1;
	QPoint vectorBridge2 = p3 - p4;
	QPoint diff = p1 - p3;

	float crossProduct1 = vectorBridge2.x() * diff.y() - vectorBridge2.y() * diff.x();
	float crossProduct2 = vectorBridge2.x() * vectorBridge1.y() - vectorBridge2.y() * vectorBridge1.x();

	float t1 = crossProduct1 / crossProduct2;
	float t2 = (diff.x() * vectorBridge1.y() - diff.y() * vectorBridge1.x()) / crossProduct2;

	if (t1 >= -1 && t1 <= 0 && t2 >= -1 && t2 <= 0)
	{
		return true;
	}

	return false;
}

void Board::PlaceBridge(IPiece* firstPilon, IPiece* secondPilon, std::reference_wrapper<Player> pickingPlayer)
{
	pickingPlayer.get().AddBridge(firstPilon, secondPilon);
	m_allBridges.push_back(pickingPlayer.get().GetLastBridge());
}

const bool Board::IsInBoard(const Pilon::Position& pos) const
{
	auto [row, col] = pos;
	if (row >= 0 && row < m_size && col >= 0 && col < m_size && !IsCorner(pos))
		return true;
	return false;
}

void Board::SetBoardSize(const uint16_t& size)
{
	m_size = size;
	m_board.resize(m_size);
	for (int index = 0; index < m_size; index++)
		m_board[index].resize(m_size);
}

uint16_t Board::GetBoardSize() const
{
	return m_size;
}

bool Board::IsPilon(const Pilon::Position& pos) const //modificat 17.12
{
	auto& [row, col] = pos;
	Pilon* p = dynamic_cast<Pilon*>(m_board[row][col]);
	if (p)
		return true;
	return false;
}

bool Board::IsBridge(IPiece* f) const
{
	Bridge* b = dynamic_cast<Bridge*>(f);
	if (b)
		return true;
	return false;
}

bool Board::IsPiece(IPiece* f) const
{
	if (IsBridge(f))
		return true;
	return false;
}

bool Board::IsRedBase(Pilon::Position pos) const
{
	auto& [row, col] = pos;
	if (row == 0 || row == m_size - 1)
		return true;
	return false;
}

bool Board::IsBlueBase(Pilon::Position pos) const
{
	auto& [row, col] = pos;
	if (col == 0 || col == m_size - 1)
		return true;
	return false;
}

std::vector<IPiece*> Board::GetBridges() const
{
	return m_allBridges;
}

IPiece::PlayerColor Board::GetColor(const Pilon::Position& pos)
{
	auto& [row, col] = pos;
	return m_board[row][col]->GetColor();
}

bool BridgesSharePilon(const QPoint& p1, const QPoint& p2, const QPoint& q1, const QPoint& q2)
{
	if (p1 == q1 || p1 == q2 || p2 == q1 || p2 == q2)
		return true;
	return false;
}

std::vector<IPiece*> Board::AdjacentPilons(const Pilon::Position& currentPos, std::reference_wrapper<Player> activePlayer) const
{
	auto& [row, col] = currentPos;
	std::vector<IPiece*> adjacentPilons;
	int rowVect[] = { -1, -2, -2, -1, 1, 2, 2, 1 };
	int colVect[] = { -2, -1, 1, 2, 2, 1, -1, -2 };
	for (int index = 0; index < 8; index++)
	{
		if (Pilon::Position cellPos = std::make_pair(row + rowVect[index], col + colVect[index]); IsInBoard(cellPos) == true)
		{
			Pilon* pilon = dynamic_cast<Pilon*> (m_board[row + rowVect[index]][col + colVect[index]]);
			if (pilon && pilon->GetColor() == activePlayer.get().GetColor())
			{
				bool collidesWithOtherBridge{ false };
				for (auto& bridge : m_allBridges)
				{
					Bridge* currentBridge = dynamic_cast<Bridge*>(bridge);
					Pilon* pilon1 = dynamic_cast<Pilon*>(currentBridge->GetFirstPilon());
					Pilon* pilon2 = dynamic_cast<Pilon*>(currentBridge->GetSecondPilon());
					QPoint p1(pilon1->GetColumn(), pilon1->GetRow());
					QPoint p2(pilon2->GetColumn(), pilon2->GetRow());
					QPoint q1(col, row);
					QPoint q2(cellPos.second, cellPos.first);
					if (DoBridgesIntersect(p1, p2, q1, q2) && !BridgesSharePilon(p1, p2, q1, q2))
						collidesWithOtherBridge = true;
				}
				if (!collidesWithOtherBridge)
				{
					adjacentPilons.push_back(pilon);
					Pilon* p = dynamic_cast<Pilon*>(m_board[row][col]);
					activePlayer.get().MakeAdjacent(p->GetIndex(), pilon->GetIndex());
				}
			}
		}
	}

	return adjacentPilons;
}