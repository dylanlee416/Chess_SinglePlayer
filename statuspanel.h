#ifndef STATUSPANEL_H
#define STATUSPANEL_H

#include <QWidget>
#include <QLCDNumber>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QTextEdit>  // For displaying move history

class ChessBoard;

class StatusPanel : public QWidget
{
    Q_OBJECT

public:
    explicit StatusPanel(QWidget* parent = nullptr);
    void setChessBoard(ChessBoard* _chessBoard) { chessBoard = _chessBoard; }

    // Method to start the game clock
    void startGame();
    // Method to reset the game
    void resetGame();
    void switchTurns();     // Switch turns between players
    void addMoveHistoryToStatusPlane(QPair<QPoint, QPoint> move);
    void addMoveToHistory(const QString& move, int step);  // Add a move to the history


private:
    ChessBoard* chessBoard;

    QLabel* statusLabel;      // Label to display game status information
    QComboBox* timeSelector;  // Dropdown for selecting time (5, 10, 15, 60 minutes)
    QTextEdit* moveHistory;   // TextEdit to display move history
    QPushButton* startButton; // Button to start the clock

    int whiteTime;            // White player's remaining time (in seconds)
    int blackTime;            // Black player's remaining time (in seconds)
    QLCDNumber* whiteClock;   // White player's clock display
    QLCDNumber* blackClock;   // Black player's clock display

    bool whiteTurn;           // True if it's white's turn, false for black
    QTimer* gameTimer;        // Timer to control the game clock

    void initializeUI();      // Method to initialize the UI elements
    void updateClocks();      // Update the clocks every second
    void updateClockDisplay();
    void showGameOverMessage(const QString& message);
    void showTimeOutMessage(bool whiteTurn);

    // Helper function to set clock styles
    void setClockStyles();
};

#endif // STATUSPANEL_H
