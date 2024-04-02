#include <qpainter.h>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QTextEdit>
#include <QLabel>
#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "Game.h"

class mainwindow : public QMainWindow
{
	Q_OBJECT

public:
	const int kFoundationRadius = 6;
	const int kSquareSize = 20;
	const int kHorizontalMarginSize = 170;
	const int kVerticalMarginSize = 70;
	const int kStartMenuItemsWidth = 150;
	const int kStartMenuItemsHeight = 50;
	const int kStartMenuItemsYOffset = 50;
	const int kInfoLabelsXOffset = 50;
	const int kInfoLabelsYOffset = 20;
	const int kInfoLabelsHeight = 50;
	const int kInfoLabelsWidth = 100;
	const QSize kStartMenuSize = QSize(820, 620);

public:
	mainwindow(std::string fileName, QWidget* parent = nullptr, const int& boardSize = 24);
	~mainwindow() = default;
	
private:
	virtual void mouseReleaseEvent(QMouseEvent* e);
	virtual void paintEvent(QPaintEvent* e) override;
	void DrawMainMenu(QPaintEvent* e);
	void DrawBoard(QPaintEvent* e);
	void DrawGui(QPaintEvent* e);

private:
	Game* m_game;
	int m_boardSizeRects;
	int m_boardSizePixels;
	int m_windowWidth;
	int m_windowHeight;
	bool m_gameStarted;
	std::string m_saveFilePath;

private:
	Ui::mainwindowClass* ui;
private: 
	QRect player1Name, player2Name;
	QRect player1Pilons, player2Pilons;
	QRect player1Bridges, player2Bridges;

	QRect titleText;
	QRect playButton;
	QRect quitButton;
	QRect backToMainButton;
	QRect saveGameButton;
	QRect loadGameButton;
	QRect switchSidesButton;
	QRect turnText;

	void SetGuiItems();
	void SetBoardSize(const int& size);
};




