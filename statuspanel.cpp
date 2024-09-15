#include "statuspanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

#include "chessboard.h"

StatusPanel::StatusPanel(QWidget* parent) : QWidget(parent), whiteTurn(true), whiteTime(0), blackTime(0)
{
    initializeUI();

    // Timer to update the clocks every second
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &StatusPanel::updateClocks);
}

void StatusPanel::initializeUI()
{
    // Create LCD displays for both players' clocks
    whiteClock = new QLCDNumber(this);
    blackClock = new QLCDNumber(this);

    // Set digit count to accommodate hours and minutes (hh:mm:ss format)
    whiteClock->setDigitCount(8); // hh:mm:ss format
    blackClock->setDigitCount(8); // hh:mm:ss format

    blackClock->setSegmentStyle(QLCDNumber::Filled);
    blackClock->setSegmentStyle(QLCDNumber::Filled);

    // Set default styles for clocks
    whiteClock->setStyleSheet("background-color: green; color: black; font-size: 24px;"); // Default style
    blackClock->setStyleSheet("background-color: gray; color: white; font-size: 24px;"); // Default style

    // Initialize clocks to 05:00
    whiteTime = 5 * 60; // 5 minutes
    blackTime = 5 * 60; // 5 minutes
    updateClockDisplay(); // Update display with initial values

    // Create the time selector dropdown
    timeSelector = new QComboBox(this);
    timeSelector->addItem("2 seconds", 2);
    timeSelector->addItem("5 minutes", 300);
    timeSelector->addItem("10 minutes", 600);
    timeSelector->addItem("15 minutes", 900);
    timeSelector->addItem("1 hour", 3600);
    timeSelector->addItem("2 hours", 7200);
    timeSelector->addItem("3 hours", 10800);
    timeSelector->addItem("6 hours", 21600);
    timeSelector->addItem("12 hours", 43200);
    timeSelector->addItem("24 hours", 86400);

    // Create the start button
    startButton = new QPushButton("Start Game", this);
    startButton->setFixedSize(100, 30);  // 设置按钮的固定宽度为100，高度为50
    connect(startButton, &QPushButton::clicked, this, &StatusPanel::startGame);

    // Create the move history text edit
    moveHistory = new QTextEdit(this);
    moveHistory->setReadOnly(true); // Make the move history read-only

    // Create layouts for player clocks with labels
    QHBoxLayout* whiteClockLayout = new QHBoxLayout();
    whiteClockLayout->addWidget(new QLabel("White Player", this));
    whiteClockLayout->addStretch(); // Stretchable space between label and clock
    whiteClockLayout->addWidget(whiteClock);

    // Create layout for the black player's clock
    QHBoxLayout* blackClockLayout = new QHBoxLayout();
    blackClockLayout->addWidget(new QLabel("Black Player", this));
    blackClockLayout->addStretch(); // Stretchable space between label and clock
    blackClockLayout->addWidget(blackClock);

    // Create a horizontal layout for the time selector
    QHBoxLayout* timeSelectorLayout = new QHBoxLayout();
    QLabel* timeLabel = new QLabel("Select Time:", this);
    timeSelectorLayout->addWidget(timeLabel);
    timeSelectorLayout->addWidget(timeSelector);

    // Create a vertical layout for the overall panel
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Add the time selector layout at the top
    mainLayout->addLayout(timeSelectorLayout);

    // Add the black clock layout
    mainLayout->addLayout(blackClockLayout);

    // Add the move history text edit
    mainLayout->addWidget(moveHistory);

    // Add the white clock layout
    mainLayout->addLayout(whiteClockLayout);

    // Create a horizontal layout to center the start button
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();  // Add stretchable space to the left
    buttonLayout->addWidget(startButton);  // Add the start button
    buttonLayout->addStretch();  // Add stretchable space to the right

    // Add the button layout to the main vertical layout
    mainLayout->addLayout(buttonLayout);

    // Set the main layout to the StatusPanel
    setLayout(mainLayout);
}

void StatusPanel::startGame()
{
    if (chessBoard) chessBoard->startGame();

    // Get selected time from the time selector
    int selectedTime = timeSelector->currentData().toInt();
    whiteTime = blackTime = selectedTime;

    // Update the clocks
    updateClockDisplay();

    // Start the timer
    gameTimer->start(1000);

    // Disable the start button and time selector
    startButton->setEnabled(false);
    timeSelector->setEnabled(false);
}

void StatusPanel::updateClockDisplay()
{
    // Function to format the display string
    auto formatTime = [](int seconds) -> QString {
        if (seconds >= 3600) {
            // Display in hh:mm:ss format
            return QString("%1:%2:%3")
                .arg(seconds / 3600, 2, 10, QChar('0'))
                .arg((seconds % 3600) / 60, 2, 10, QChar('0'))
                .arg(seconds % 60, 2, 10, QChar('0'));
        } else {
            // Display in mm:ss format if hours are zero
            return QString("%1:%2")
                .arg(seconds / 60, 2, 10, QChar('0'))
                .arg(seconds % 60, 2, 10, QChar('0'));
        }
    };

    // Update the display with the current time values
    whiteClock->display(formatTime(whiteTime));
    blackClock->display(formatTime(blackTime));
}

void StatusPanel::updateClocks()
{
    if (!chessBoard->getIsGaming()) return;

    // Decrease time for the current player
    if (whiteTurn)
    {
        whiteTime--;
        if (whiteTime < 0) whiteTime = 0; // Ensure time does not go negative

        // Update the display with the current time values
        updateClockDisplay();

        // Change color and background if the remaining time is low (e.g., less than 10 seconds)
        if (whiteTime <= 10) {
            whiteClock->setStyleSheet("background-color: red; color: white; font-size: 24px;"); // Emphasize with red background and white text
        }
        else
        {
            whiteClock->setStyleSheet("background-color: green; color: black; font-size: 24px;"); // Emphasize with green background and black text
        }
        blackClock->setStyleSheet("background-color: gray; color: white; font-size: 24px;"); // Default style
    }
    else
    {
        blackTime--;
        if (blackTime < 0) blackTime = 0; // Ensure time does not go negative

        // Update the display with the current time values
        updateClockDisplay();

        // Change color and background if the remaining time is low (e.g., less than 10 seconds)
        if (blackTime <= 10) {
            blackClock->setStyleSheet("background-color: red; color: white; font-size: 24px;"); // Emphasize with red background and white text
        }
        else
        {
            blackClock->setStyleSheet("background-color: green; color: black; font-size: 24px;"); // Emphasize with green background and black text
        }
        whiteClock->setStyleSheet("background-color: gray; color: white; font-size: 24px;"); // Default style
    }

    // Switch turns if time runs out and determine the result
    if (whiteTime == 0 || blackTime == 0)
    {
        gameTimer->stop();  // Stop the timer when time runs out

        // Determine the result of the game
        if (whiteTime == 0 && blackTime > 0) {
            // White loses due to timeout
            showTimeOutMessage(true);  // Pass true since it's white's time that ran out
        }
        else if (blackTime == 0 && whiteTime > 0) {
            // Black loses due to timeout
            showTimeOutMessage(false);  // Pass false since it's black's time that ran out
        }
        else if (whiteTime == 0 && blackTime == 0) {
            // Both players run out of time, it's a draw
            showGameOverMessage("It's a draw!");
        }
    }

}

void StatusPanel::showTimeOutMessage(bool whiteTurn)
{
    // 创建消息框
    QMessageBox msgBox;
    msgBox.setWindowTitle("Time's Up!");

    // 根据当前轮次设置文本 (例如，白方时间耗尽，黑方获胜)
    msgBox.setText(whiteTurn ? "White's time is up. Black wins!" : "Black's time is up. White wins!");
    msgBox.setIcon(QMessageBox::NoIcon);  // 去除图标

    // 调整字体大小并设置居中对齐
    msgBox.setStyleSheet("QLabel { font-size: 14px; font-weight: bold; text-align: left; } QPushButton { font-size: 10px; }");

    msgBox.setStandardButtons(QMessageBox::Ok);

    // 设置 QLabel 对齐方式为居中
    QLabel *label = msgBox.findChild<QLabel *>();
    if (label) {
        label->setAlignment(Qt::AlignCenter);
    }

    // 添加 spacer 来调整消息框大小
    QSpacerItem* spacer = new QSpacerItem(200, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(spacer, layout->rowCount(), 0, 1, layout->columnCount());

    // 显示消息框
    msgBox.exec();

    chessBoard -> timeRunOut();
}


void StatusPanel::showGameOverMessage(const QString& message)
{
    // Display a message box or update a label to show the game result
    QMessageBox::information(this, "Game Over", message);
}


void StatusPanel::switchTurns()
{
    whiteTurn = !whiteTurn;  // Switch between white and black turns
}

void StatusPanel::resetGame()
{
    gameTimer->stop();
    startButton->setEnabled(true);
    timeSelector->setEnabled(true);
}


void StatusPanel::addMoveToHistory(const QString& move, int step)
{
    moveHistory->insertPlainText(move);  // 插入文本，不自动换行
    if (step%2) moveHistory->append(QString());
}
