#include "Player.h"

Player::Player(IPiece::PlayerColor teamColor)
	:m_teamColor{ teamColor },
	m_pilonCounter{ 0 },
	m_bridgeCounter{ 0 },
	m_playerName{""}
{
}

Player::Player(std::string playerName, IPiece::PlayerColor teamColor)
	:m_pilonCounter{ 0 },
	m_bridgeCounter{ 0 },
	m_teamColor{ teamColor },
	m_playerName{ playerName }
{
}

IPiece::PlayerColor Player::GetColor() const
{

	return m_teamColor;
}

int Player::GetPilonCounter() const
{
	return m_pilonCounter;
}

int Player::GetBridgeCounter() const
{
	return m_bridgeCounter;
}

std::string Player::GetPlayerName() const
{
	return m_playerName;
}

void Player::SetColor(const IPiece::PlayerColor& color)
{
	m_teamColor = color;
}

void Player::DecrementPilons()
{
	m_pilonCounter--;
}

void Player::SubtractBridges(const int& nBridges)
{
	m_bridgeCounter -= nBridges;
}

void Player::SetPlayerName(const std::string& name)
{
	m_playerName = name;
}

void Player::SetBridgeCounter(const int& count)
{
	m_bridgeCounter = count;
}

void Player::SetPilonCounter(const int& count)
{
	m_pilonCounter = count;
}

void Player::AddPilon(IPiece* pilon)
{
	Pilon* p = dynamic_cast<Pilon*>(pilon);
	m_pilons.push_back(p);
	DecrementPilons();
}

void Player::AddBridge(IPiece* firstPilon, IPiece* secondPilon)
{
	m_bridges.push_back(new Bridge(firstPilon, secondPilon, m_teamColor));
}

void Player::MakeAdjacent(const int& firstIndex, const int& secondIndex)
{
	Pilon* firstPilon = dynamic_cast<Pilon*>(m_pilons[firstIndex]);
	Pilon* secondPilon = dynamic_cast<Pilon*>(m_pilons[secondIndex]);

	firstPilon->AddAdjacentPilon(secondIndex);
	secondPilon->AddAdjacentPilon(firstIndex);
}

std::vector<IPiece*> Player::GetPilons() const
{
	return m_pilons;
}

std::vector<IPiece*> Player::GetBridges() const
{
	return m_bridges;
}

IPiece* Player::GetLastBridge() const
{
	if (!m_bridges.empty())
		return m_bridges[m_bridges.size() - 1];
}

bool Player::Win(const int& boardSize)
{
	if (!m_pilons.empty())
	{
		std::queue<int> visited;
		std::vector<int> notVerified;
		int verified{ 0 };
		for (int pilonIndex = 1; pilonIndex < m_pilons.size(); ++pilonIndex)
			notVerified.push_back(pilonIndex);
		visited.push(0);
		std::vector<int> pilonsInPath;
		pilonsInPath.push_back(0);
		while (verified < m_pilons.size())
		{
			while (!visited.empty())
			{
				int currentPilonIndex = visited.front();
				Pilon* p = dynamic_cast<Pilon*>(m_pilons[currentPilonIndex]);
				for (auto& pilon : p->GetAdjacentPilons())
				{
					auto thisPilon = std::find(notVerified.cbegin(), notVerified.cend(), pilon);
					if (thisPilon != notVerified.cend())
					{
						notVerified.erase(thisPilon);
						visited.push(pilon);
						pilonsInPath.push_back(pilon);
					}
				}
				visited.pop();
				verified++;
			}

			if (HasPilonsInBothBases(pilonsInPath, boardSize))
				return true;
			else if (verified < m_pilons.size())
			{
				visited.push(notVerified[0]);
				pilonsInPath.clear();
			}
		}
		return false;
	}
	return false;
}

bool Player::HasPilonsInBothBases(std::vector<int> pilonsInPath, const int& boardSize)
{
	bool firstBase = false, secondBase = false;
	for (auto& pIndex : pilonsInPath)
	{
		Pilon* pilon = dynamic_cast<Pilon*>(m_pilons[pIndex]);
		if (m_teamColor == IPiece::PlayerColor::Red)
		{
			if (pilon->IsInRedBaseTop())
				firstBase = true;
			else if (pilon->IsInRedBaseBottom(boardSize))
				secondBase = true;
		}
		else if (m_teamColor == IPiece::PlayerColor::Blue)
		{
			if (pilon->IsInBlueBaseLeft())
				firstBase = true;
			else if (pilon->IsInBlueBaseRight(boardSize))
				secondBase = true;
		}
	}
	return firstBase && secondBase;
}
