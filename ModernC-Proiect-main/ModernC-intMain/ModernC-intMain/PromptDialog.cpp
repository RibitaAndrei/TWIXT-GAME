#include "PromptDialog.h"

PromptDialog::PromptDialog(QWidget* parent, Game::GameState state, std::string winnerName) : QDialog(parent)
{
    QFormLayout* formLayout = new QFormLayout(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    if (state == Game::GameState::None)
    {
        m_player1LineEdit = new QLineEdit(this);
        m_player2LineEdit = new QLineEdit(this);
        m_boardSizeLineEdit = new QLineEdit(this);
        m_nPilonsLineEdit = new QLineEdit(this);
        m_nBridgesLineEdit = new QLineEdit(this);

        setWindowTitle("Set game options");

        QLabel* player1Label = new QLabel("Player 1 name:", this);
        QLabel* player2Label = new QLabel("Player 2 name:", this);
        QLabel* boardSizeLabel = new QLabel("Board size:", this);
        QLabel* nPilonsLabel = new QLabel("Number of pilons:", this);
        QLabel* nBridgesLabel = new QLabel("Number of bridges:", this);

        formLayout->addRow(player1Label, m_player1LineEdit);
        formLayout->addRow(player2Label, m_player2LineEdit);
        formLayout->addRow(boardSizeLabel, m_boardSizeLineEdit);
        formLayout->addRow(nPilonsLabel, m_nPilonsLineEdit);
        formLayout->addRow(nBridgesLabel, m_nBridgesLineEdit);

        QPushButton* startButton = new QPushButton("Set options", this);
        QPushButton* cancelButton = new QPushButton("Cancel", this);

        connect(startButton, &QPushButton::clicked, this, &PromptDialog::getInfo);
        connect(startButton, &QPushButton::clicked, this, &PromptDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &PromptDialog::reject);
        formLayout->addRow(startButton, cancelButton);
    }

    else if (state == Game::GameState::Draw)
    {
        QLabel* drawLabel = new QLabel("It's a draw!");
        formLayout->addRow(drawLabel);
    }

    else
    {
        QLabel* winLabel = new QLabel(QString::fromStdString(winnerName) + " wins!");
        formLayout->addRow(winLabel);
    }

    mainLayout->addLayout(formLayout);

    setLayout(mainLayout);
}

std::string PromptDialog::GetPlayer1Name() const
{
    return m_player1Name;
}

std::string PromptDialog::GetPlayer2Name() const
{
    return m_player2Name;
}

int PromptDialog::GetBoardSize() const
{
    return m_boardSize;
}

int PromptDialog::GetNPilons() const
{
    return m_nPilons;
}

int PromptDialog::GetNBridges() const
{
    return m_nBridges;
}

void PromptDialog::getInfo()
{
    m_player1Name = m_player1LineEdit->text().toStdString();
    m_player2Name = m_player2LineEdit->text().toStdString();
    m_boardSize = m_boardSizeLineEdit->text().toInt();
    m_nPilons = m_nPilonsLineEdit->text().toInt();
    m_nBridges = m_nBridgesLineEdit->text().toInt();
}