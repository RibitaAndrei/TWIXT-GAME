#include "mainwindow.h"
#include <QLineEdit>
#include "PromptDialog.h"

mainwindow::mainwindow(std::string fileName, QWidget* parent, const int& boardSize)
	: QMainWindow(parent),
	m_windowHeight{ 2 * kVerticalMarginSize + boardSize * kSquareSize },
	m_windowWidth{ 2 * kHorizontalMarginSize + boardSize * kSquareSize },
	m_boardSizeRects{ boardSize },
	m_boardSizePixels{ m_boardSizeRects * kSquareSize },
	m_gameStarted{ false },
	ui{ new Ui::mainwindowClass },
	m_saveFilePath{ fileName }
{
		ui->setupUi(this);
		m_game = new Game();
		this->setFixedSize(kStartMenuSize);
}

void mainwindow::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (m_gameStarted)
		{
			int colRect{ (e->x() - kHorizontalMarginSize) / kSquareSize };
			int rowRect{ (e->y() - kVerticalMarginSize) / kSquareSize };
			Pilon::Position rectPos{ std::make_pair(rowRect, colRect) };
			if (rowRect >= 0 && colRect >= 0 && rowRect < m_boardSizeRects && colRect < m_boardSizeRects)
			{
				m_game->PlacePilon(rectPos);
				if (m_game->CheckWinCondition() == true)
				{
					PromptDialog winDialog(this, Game::GameState::Win, m_game->GetPickingPlayer().get().GetPlayerName());
					winDialog.exec();
					m_gameStarted = false;
					delete m_game;
					m_game = new Game();
				}

				else if (m_game->NoMorePilons())
				{
					PromptDialog drawDialog(this, Game::GameState::Draw);
					drawDialog.exec();
					m_gameStarted = false;
					delete m_game;
					m_game = new Game();
				}

				m_game->SwapPlayers();
				update();
			}

			else if (backToMainButton.contains(e->x(), e->y()))
			{
				m_gameStarted = false;
				delete m_game;
				m_game = new Game();
				update();
			}

			else if (saveGameButton.contains(e->x(), e->y()))
				m_game->SaveGame(m_saveFilePath);

			else if (switchSidesButton.contains(e->x(), e->y()) && m_game->GetTurn() == 2)
				m_game->SwitchSides();
		}

		else
		{
			if (playButton.contains(e->x(), e->y()))
			{
				while (m_game->IncorrectValues())
				{
					PromptDialog promptUserInfo(this);
					if (promptUserInfo.exec() == QDialog::Rejected)
						break;
					else
					{
						m_game->GetPickingPlayer().get().SetPlayerName(promptUserInfo.GetPlayer1Name());
						m_game->GetWaitingPlayer().get().SetPlayerName(promptUserInfo.GetPlayer2Name());
						SetBoardSize(promptUserInfo.GetBoardSize());
						m_game->SetPilonsAndBridges(promptUserInfo.GetNPilons(), promptUserInfo.GetNBridges());
						promptUserInfo.close();
						m_windowHeight = 2 * kVerticalMarginSize + m_game->GetBoardSize() * kSquareSize;
						m_windowWidth = 2 * kHorizontalMarginSize + m_game->GetBoardSize() * kSquareSize;
						this->setFixedSize(QSize(m_windowWidth, m_windowHeight));
						m_gameStarted = true;
						std::ofstream fout;
						fout.open(m_saveFilePath, std::ofstream::out | std::ofstream::trunc);
						fout.close();
					}
				}
			}

			else if (std::ifstream fin(m_saveFilePath); loadGameButton.contains(e->x(), e->y()) && fin.peek() != std::ifstream::traits_type::eof())
			{
				m_game->LoadGame(m_saveFilePath);
				m_windowHeight = 2 * kVerticalMarginSize + m_game->GetBoardSize() * kSquareSize;
				m_windowWidth = 2 * kHorizontalMarginSize + m_game->GetBoardSize() * kSquareSize;
				this->setFixedSize(QSize(m_windowWidth, m_windowHeight));
				m_gameStarted = true;
			}

			else if (quitButton.contains(e->x(), e->y()))
				this->close();
		}
		update();
	}
}

void DrawProperLine(QPoint& firstNode, QPoint& secondNode, int foundationSize)
{
	QLineF line(firstNode, secondNode);
	QPointF arrowP1 = line.p1() + QPointF((line.dx() * 0.1), (line.dy() * 0.1));
	QPointF arrowP2 = line.p2() - QPointF((line.dx() * 0.1), (line.dy() * 0.1));

	firstNode = arrowP1.toPoint();
	secondNode = arrowP2.toPoint();
}



void mainwindow::paintEvent(QPaintEvent* e)
{
	SetGuiItems();
	if (!m_gameStarted)
	{
		DrawMainMenu(e);
	}
	else
	{
		DrawBoard(e);
		DrawGui(e);
	}
}

void mainwindow::DrawMainMenu(QPaintEvent* e)
{
	QPainter painter(this);

	this->setFixedSize(kStartMenuSize);
	painter.setBrush(Qt::red);
	painter.drawRect(playButton);
	painter.drawRect(quitButton);
	painter.setBrush(Qt::black);
	painter.setFont(QFont("comic sans", 50));
	painter.drawText(titleText, Qt::AlignCenter, "Twixt");
	painter.setFont(QFont("comic sans", 20));
	painter.drawText(playButton, Qt::AlignCenter, "New game");
	painter.drawText(quitButton, Qt::AlignCenter, "Quit");

	if (std::ifstream fin(m_saveFilePath); fin.peek() == std::ifstream::traits_type::eof())
		painter.setBrush(Qt::darkRed);
	else
		painter.setBrush(Qt::red);
	painter.drawRect(loadGameButton);
	painter.setBrush(Qt::black);
	painter.drawText(loadGameButton, Qt::AlignCenter, "Load game");
}

void mainwindow::DrawBoard(QPaintEvent* e)
{
	QPainter painter(this);
	QPen pen;

	for (int indexRow = 0; indexRow < m_game->GetBoardSize(); ++indexRow)
	{
		for (int indexCol = 0; indexCol < m_game->GetBoardSize(); ++indexCol)
		{
			Pilon::Position foundationCoords = std::make_pair(indexRow, indexCol);
			if (!m_game->IsCorner(foundationCoords))
			{
				std::pair<int, int> foundationCenter;
				foundationCenter.first = kHorizontalMarginSize + kSquareSize * indexCol + kSquareSize / 2;
				foundationCenter.second = kVerticalMarginSize + kSquareSize * indexRow + kSquareSize / 2;
				QPoint center(foundationCenter.first, foundationCenter.second);
				QRect foundation{ kHorizontalMarginSize + kSquareSize * indexCol, kVerticalMarginSize + kSquareSize * indexRow, kSquareSize, kSquareSize };
				if (m_game->IsPilon(std::make_pair(indexRow, indexCol)))
				{
					if (m_game->GetColor(std::make_pair(indexRow, indexCol)) == IPiece::PlayerColor::Red)
					{
						painter.setBrush(Qt::red);
						painter.drawEllipse(center, kFoundationRadius, kFoundationRadius);
					}
					else
					{
						painter.setBrush(Qt::blue);
						painter.drawEllipse(center, kFoundationRadius, kFoundationRadius);
					}
				}
				else
				{
					painter.setBrush(Qt::white);
					painter.drawEllipse(center, kFoundationRadius, kFoundationRadius);
				}
			}
		}
	}

	std::vector<IPiece*> bridges = m_game->GetBridges();
	for (auto& bridge : bridges)
	{
		Bridge* b = dynamic_cast<Bridge*>(bridge);
		Pilon* first = dynamic_cast<Pilon*>(b->GetFirstPilon());
		Pilon* second = dynamic_cast<Pilon*>(b->GetSecondPilon());

		uint16_t row = kVerticalMarginSize + kSquareSize * first->GetRow() + kSquareSize / 2;
		uint16_t col = kHorizontalMarginSize + kSquareSize * first->GetColumn() + kSquareSize / 2;
		QPoint firstCenter{ col, row };

		row = kVerticalMarginSize + kSquareSize * second->GetRow() + kSquareSize / 2;
		col = kHorizontalMarginSize + kSquareSize * second->GetColumn() + kSquareSize / 2;
		QPoint secondCenter{ col, row };

		DrawProperLine(firstCenter, secondCenter, kFoundationRadius);
		pen.setWidth(2);
		painter.setPen(pen);
		painter.drawLine(firstCenter, secondCenter);
	}
}

void mainwindow::DrawGui(QPaintEvent* e)
{
	QPainter painter(this);
	QPen pen;

	QPoint topLeftCorner(kHorizontalMarginSize + kSquareSize, kVerticalMarginSize + kSquareSize);
	QPoint topRightCorner(kHorizontalMarginSize + kSquareSize * (m_boardSizeRects - 1), kVerticalMarginSize + kSquareSize);
	QPoint bottomLeftCorner(kHorizontalMarginSize + kSquareSize, kVerticalMarginSize + kSquareSize * (m_boardSizeRects - 1));
	QPoint bottomRightCorner(kHorizontalMarginSize + kSquareSize * (m_boardSizeRects - 1), kVerticalMarginSize + kSquareSize * (m_boardSizeRects - 1));

	QLine redBaseLineLeft(topLeftCorner, bottomLeftCorner);
	QLine redBaseLineRight(topRightCorner, bottomRightCorner);
	QLine blueBaseLineTop(topLeftCorner, topRightCorner);
	QLine blueBaseLineBottom(bottomLeftCorner, bottomRightCorner);

	pen.setWidth(2);

	pen.setColor(Qt::blue);
	painter.setPen(pen);
	painter.drawLine(redBaseLineLeft);
	painter.drawLine(redBaseLineRight);

	pen.setColor(Qt::red);
	painter.setPen(pen);
	painter.drawLine(blueBaseLineTop);
	painter.drawLine(blueBaseLineBottom);

	pen.setColor(Qt::black);
	painter.setPen(pen);
	painter.drawText(player1Name, "Red: " + QString::fromStdString(m_game->GetFirstPlayer().GetPlayerName()));
	painter.drawText(player2Name, "Blue: " + QString::fromStdString(m_game->GetSecondPlayer().GetPlayerName()));
	painter.drawText(player1Pilons, "Remaining pilons: " + QString::number(m_game->GetFirstPlayer().GetPilonCounter()));
	painter.drawText(player2Pilons, "Remaining pilons: " + QString::number(m_game->GetSecondPlayer().GetPilonCounter()));
	painter.drawText(player1Bridges, "Remaining bridges: " + QString::number(m_game->GetFirstPlayer().GetBridgeCounter()));
	painter.drawText(player2Bridges, "Remaining bridges: " + QString::number(m_game->GetSecondPlayer().GetBridgeCounter()));

	painter.setBrush(Qt::red);
	painter.drawRect(backToMainButton);
	painter.drawRect(saveGameButton);
	if (m_game->GetTurn() == 2)
		painter.drawRect(switchSidesButton);
	painter.setBrush(Qt::black);
	painter.setFont(QFont("comic sans", 10));
	painter.drawText(backToMainButton, Qt::AlignCenter, "Back to main menu");
	painter.drawText(saveGameButton, Qt::AlignCenter, "Save game");
	painter.setFont(QFont("comic sans", 15));
	painter.drawText(turnText, Qt::AlignCenter, QString::fromStdString(m_game->GetPickingPlayer().get().GetPlayerName()) + "'s turn");
	if (m_game->GetTurn() == 2)
	{
		painter.setFont(QFont("comic sans", 10));
		painter.drawText(switchSidesButton, Qt::AlignCenter, "Switch sides");
	}
}

void mainwindow::SetGuiItems()
{
	//labels
	player1Name.setRect(kInfoLabelsXOffset, kVerticalMarginSize + kInfoLabelsYOffset, kInfoLabelsWidth, kInfoLabelsHeight);

	player2Name.setRect(m_windowWidth - kHorizontalMarginSize + kInfoLabelsXOffset,
		kVerticalMarginSize + kInfoLabelsYOffset, kInfoLabelsWidth, kInfoLabelsHeight);

	player1Pilons.setRect(kInfoLabelsXOffset, kVerticalMarginSize + kInfoLabelsYOffset + kInfoLabelsHeight, kInfoLabelsWidth, kInfoLabelsHeight);

	player2Pilons.setRect(m_windowWidth - kHorizontalMarginSize + kInfoLabelsXOffset,
		kVerticalMarginSize + kInfoLabelsYOffset + kInfoLabelsHeight, kInfoLabelsWidth, kInfoLabelsHeight);

	player1Bridges.setRect(kInfoLabelsXOffset, kVerticalMarginSize + kInfoLabelsYOffset + 2 * kInfoLabelsHeight, kInfoLabelsWidth, kInfoLabelsHeight);

	player2Bridges.setRect(m_windowWidth - kHorizontalMarginSize + kInfoLabelsXOffset,
		kVerticalMarginSize + kInfoLabelsYOffset + 2 * kInfoLabelsHeight, kInfoLabelsWidth, kInfoLabelsHeight);

	//start menu 
	titleText.setRect(kStartMenuSize.width() / 2 - kStartMenuItemsWidth / 2, kVerticalMarginSize, kStartMenuItemsWidth, kStartMenuItemsHeight);
	playButton.setRect(kStartMenuSize.width() / 2 - kStartMenuItemsWidth / 2, kVerticalMarginSize + kStartMenuItemsHeight + kStartMenuItemsYOffset,
		kStartMenuItemsWidth, kStartMenuItemsHeight);
	loadGameButton.setRect(kStartMenuSize.width() / 2 - kStartMenuItemsWidth / 2,
		kVerticalMarginSize + 2 * (kStartMenuItemsHeight + kStartMenuItemsYOffset), kStartMenuItemsWidth, kStartMenuItemsHeight);
	quitButton.setRect(kStartMenuSize.width() / 2 - kStartMenuItemsWidth / 2,
		kVerticalMarginSize + 3 * (kStartMenuItemsHeight + kStartMenuItemsYOffset), kStartMenuItemsWidth, kStartMenuItemsHeight);

	backToMainButton.setRect(kInfoLabelsXOffset, kInfoLabelsYOffset, kStartMenuItemsWidth, kStartMenuItemsHeight / 2);
	saveGameButton.setRect(m_windowWidth - kInfoLabelsXOffset - kStartMenuItemsWidth, kInfoLabelsYOffset, kStartMenuItemsWidth, kStartMenuItemsHeight / 2);
	turnText.setRect(m_windowWidth / 2 - kStartMenuItemsWidth / 2, kInfoLabelsYOffset, kStartMenuItemsWidth, kStartMenuItemsHeight / 2);
	switchSidesButton.setRect(m_windowWidth - kHorizontalMarginSize + kInfoLabelsXOffset,
		kVerticalMarginSize + kInfoLabelsYOffset + 3 * kInfoLabelsHeight, kInfoLabelsWidth, kInfoLabelsHeight / 2);
}

void mainwindow::SetBoardSize(const int& size)
{
	m_game->SetBoardSize(size);
	m_boardSizeRects = size;
	m_boardSizePixels = size * kSquareSize;
}