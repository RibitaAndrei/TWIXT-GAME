#include "Game.h"
#include "Pilon.h"
#include <conio.h>
#include <iostream>
#include <iomanip>

Game::Game()
    :m_player1Ptr{ std::make_unique<Player>(IPiece::PlayerColor::Red) },
    m_player2Ptr{ std::make_unique<Player>(IPiece::PlayerColor::Blue) },
    m_pickingPlayer{ *m_player1Ptr },
    m_waitingPlayer{ *m_player2Ptr },
    m_state{ GameState::None },
    m_gameBoard{ 0 },
    m_turn{ 1 }
{}

Game::Game(uint16_t boardSize)
    :m_player1Ptr{ std::make_unique<Player>(IPiece::PlayerColor::Red) },
    m_player2Ptr{ std::make_unique<Player>(IPiece::PlayerColor::Blue) },
    m_gameBoard{ boardSize },
    m_pickingPlayer{ *m_player1Ptr },
    m_waitingPlayer{ *m_player2Ptr },
    m_state{ GameState::None },
    m_turn{ 1 }
{}

Game::Game(uint16_t boardSize, std::string playerName1, std::string playerName2) :
    m_gameBoard{ boardSize }, m_player1Ptr{ std::make_unique<Player>(playerName1, IPiece::PlayerColor::Red) },
    m_player2Ptr{ std::make_unique<Player>(playerName2, IPiece::PlayerColor::Blue) }
    , m_pickingPlayer{ *m_player1Ptr }, m_waitingPlayer{ *m_player2Ptr },
    m_state{ GameState::None },
    m_turn{ 1 }
{}

Game::Game(const Game& copy) :
    Game(copy.GetBoardSize(), copy.m_player1Ptr->GetPlayerName(), copy.m_player2Ptr->GetPlayerName())
{}

Game::~Game()
{}

bool Game::IsPilon(const Pilon::Position& pos)
{
    return m_gameBoard.IsPilon(pos);
}

IPiece::PlayerColor Game::GetColor(const Pilon::Position& pos)
{
    return m_gameBoard.GetColor(pos);
}

void Game::SetBoardSize(int size)
{
    m_gameBoard.SetBoardSize(size);
}

bool Game::IsCorner(const Pilon::Position& pos)
{
    return m_gameBoard.IsCorner(pos);
}

int Game::GetBoardSize() const
{
    return m_gameBoard.GetBoard().size();
}

Player Game::GetFirstPlayer() const
{
    return *m_player1Ptr;
}

Player Game::GetSecondPlayer() const
{
    return *m_player2Ptr;
}

void Game::RecordMove(const std::string& playerName, const Pilon::Position& coordinates)
{
    moveHistory.emplace_back(playerName, coordinates);
}

void Game::PlacePilon(const Pilon::Position& coordinates)
{
    int nBridges{ m_gameBoard.PlacePilon(coordinates, m_pickingPlayer) };
    //m_pickingPlayer.get().DecrementPilons();
    m_pickingPlayer.get().SubtractBridges(nBridges);
}

void Game::PlacePilon(Pilon* p, std::unique_ptr<Player>& player)
{
    int nBridges{ m_gameBoard.PlacePilon(p->GetPosition(), *player.get()) };
    m_pickingPlayer.get().DecrementPilons();
    m_pickingPlayer.get().SubtractBridges(nBridges);
}

void Game::SwapPlayers()
{
    std::swap(m_pickingPlayer, m_waitingPlayer);
    m_turn++;
}

void Game::ReadPlayersAndBoard(std::string playerName, int& boardSize)
{

    std::cout << kPlayer1Choice;
    std::cin >> playerName;
    m_player1Ptr->SetPlayerName(playerName);

    std::cout << kPlayer2Choice;
    std::cin >> playerName;
    m_player2Ptr->SetPlayerName(playerName);

    std::cout << kBoardChoice;
    std::cin >> boardSize;
    m_gameBoard.SetBoardSize(boardSize);
}

bool Game::CheckWinCondition() const
{
    return m_pickingPlayer.get().Win(m_gameBoard.GetBoardSize());
}

bool Game::NoMorePilons() const
{
    if (m_pickingPlayer.get().GetPilonCounter() == 0 && m_waitingPlayer.get().GetPilonCounter() == 0)
        return true;
    return false;
}

std::vector<IPiece*> Game::GetBridges() const
{
    return m_gameBoard.GetBridges();
}

std::reference_wrapper<Player> Game::GetPickingPlayer() const
{
    return m_pickingPlayer;
}

std::reference_wrapper<Player> Game::GetWaitingPlayer() const
{
    return m_waitingPlayer;
}

void Game::SetPickingPlayer(const std::reference_wrapper<Player>& pickingPlayer)
{
    m_pickingPlayer = pickingPlayer;
}

void Game::SetWaitingPlayer(const std::reference_wrapper<Player>& waitingPlayer)
{
    m_waitingPlayer = waitingPlayer;
}

void Game::SaveGame(const std::string& fileName)
{
    std::ofstream fout(fileName);
    fout << m_gameBoard.GetBoardSize() << std::endl;
    fout << m_player1Ptr.get()->GetPlayerName() << std::endl;
    std::vector<IPiece*> pilons = m_player1Ptr->GetPilons();
    fout << pilons.size() << std::endl;
    for (auto& pilon : pilons)
    {
        Pilon* p = dynamic_cast<Pilon*>(pilon);
        fout << std::setw(2) << std::setfill('0') << p->GetRow();
        fout << " " << std::setw(2) << std::setfill('0') << p->GetColumn() << std::endl;
    }

    fout << m_player2Ptr.get()->GetPlayerName() << std::endl;
    pilons = m_player2Ptr->GetPilons();
    fout << pilons.size() << std::endl;
    for (auto& pilon : pilons)
    {
        Pilon* p = dynamic_cast<Pilon*>(pilon);
        fout << std::setw(2) << std::setfill('0') << p->GetRow();
        fout << " " << std::setw(2) << std::setfill('0') << p->GetColumn() << std::endl;
    }

    fout << m_player1Ptr.get()->GetPilonCounter() + m_player1Ptr.get()->GetPilons().size() << std::endl;
    fout << m_player1Ptr.get()->GetBridgeCounter() + m_player1Ptr.get()->GetBridges().size() << std::endl;
}

void Game::LoadGame(const std::string& fileName)
{
    std::ifstream fin(fileName);
    std::vector<IPiece*> redPilons;
    std::vector<IPiece*> bluePilons;
    int index{ 0 };
    std::string line;
    std::getline(fin, line);
    int boardSize{ std::stoi(line) };
    m_gameBoard.SetBoardSize(boardSize);
    std::getline(fin, line);
    m_player1Ptr.get()->SetPlayerName(line);
    std::getline(fin, line);
    int nPilons{ std::stoi(line) };
    for (int indexLine = 0; indexLine < nPilons; ++indexLine)
    {
        std::getline(fin, line);
        int row = (line[0] - '0') * 10 + (line[1] - '0');
        int col = (line[3] - '0') * 10 + (line[4] - '0');
        Pilon* pilon = new Pilon(IPiece::PlayerColor::Red, std::make_pair(row, col), index);
        PlacePilon(pilon, m_player1Ptr);
        index++;
    }

    std::getline(fin, line);
    m_player2Ptr.get()->SetPlayerName(line);
    std::getline(fin, line);
    nPilons = std::stoi(line);
    for (int indexLine = 0; indexLine < nPilons; ++indexLine)
    {
        std::getline(fin, line);
        int row{ (line[0] - '0') * 10 + (line[1] - '0') };
        int col{ (line[3] - '0') * 10 + (line[4] - '0') };
        Pilon* pilon = new Pilon(IPiece::PlayerColor::Blue, std::make_pair(row, col), index);
        PlacePilon(pilon, m_player2Ptr);
        index++;
    }

    std::getline(fin, line);
    int nTotalPilons = std::stoi(line);
    std::getline(fin, line);
    int nTotalBridges = std::stoi(line);
    SetPilonsAndBridges(nTotalPilons, nTotalBridges);

    m_player1Ptr.get()->SetPilonCounter(nTotalPilons - m_player1Ptr.get()->GetPilons().size());
    m_player2Ptr.get()->SetPilonCounter(nTotalPilons - m_player2Ptr.get()->GetPilons().size());
    m_player1Ptr.get()->SetBridgeCounter(nTotalBridges - m_player1Ptr.get()->GetBridges().size());
    m_player2Ptr.get()->SetBridgeCounter(nTotalBridges - m_player2Ptr.get()->GetBridges().size());
}

void Game::SetPilonsAndBridges(const int& nPilons, const int& nBridges)
{
    m_player1Ptr.get()->SetPilonCounter(nPilons);
    m_player1Ptr.get()->SetBridgeCounter(nBridges);
    m_player2Ptr.get()->SetPilonCounter(nPilons);
    m_player2Ptr.get()->SetBridgeCounter(nBridges);
}

bool Game::IncorrectValues() const
{
    if (m_gameBoard.GetBoardSize() < 5 || m_player1Ptr.get()->GetPlayerName().size() == 0 || m_player2Ptr.get()->GetPlayerName().size() == 0
        || m_player1Ptr.get()->GetPilonCounter() < 5 || m_player1Ptr.get()->GetBridgeCounter() < 5
        || m_player1Ptr.get()->GetPlayerName().compare(m_player2Ptr.get()->GetPlayerName()) == 0)
        return true;

}

int Game::GetTurn() const
{
    return m_turn;
}

void Game::SwitchSides()
{
    std::string player1Name = m_player1Ptr.get()->GetPlayerName();
    m_player1Ptr.get()->SetPlayerName(m_player2Ptr.get()->GetPlayerName());
    m_player2Ptr.get()->SetPlayerName(player1Name);
    m_turn++;
}
