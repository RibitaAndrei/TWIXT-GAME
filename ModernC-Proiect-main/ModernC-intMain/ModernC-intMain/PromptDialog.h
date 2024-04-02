#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "Game.h"

class PromptDialog : public QDialog {

public:
    PromptDialog(QWidget* parent = nullptr, Game::GameState state = Game::GameState::None, std::string winnerName = "");

    // Accessor functions to get the entered values
    std::string GetPlayer1Name() const;
    std::string GetPlayer2Name() const;
    int GetBoardSize() const;
    int GetNPilons() const;
    int GetNBridges() const;

private:
    QLineEdit* m_player1LineEdit;
    QLineEdit* m_player2LineEdit;
    QLineEdit* m_boardSizeLineEdit;
    QLineEdit* m_nPilonsLineEdit;
    QLineEdit* m_nBridgesLineEdit;

    std::string m_player1Name;
    std::string m_player2Name;
    int m_boardSize;
    int m_nPilons;
    int m_nBridges;

private slots:
    void getInfo();
};