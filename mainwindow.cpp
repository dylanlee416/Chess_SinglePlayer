#include "mainwindow.h"
#include "chessboard.h"
#include "statuspanel.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    // Create a central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Instantiate the ChessBoard and StatusPanel
    chessBoard = new ChessBoard(this);
    statusPanel = new StatusPanel(this);

    chessBoard -> setStatusPanel(statusPanel);
    statusPanel -> setChessBoard(chessBoard);

    // Create a horizontal layout to hold the chessboard and status panel
    QHBoxLayout* layout = new QHBoxLayout(centralWidget);
    layout->addWidget(chessBoard);    // Add the chessboard on the left
    layout->addWidget(statusPanel);   // Add the status panel on the right

    // Set the layout to the central widget
    centralWidget->setLayout(layout);

    // Calculate the new minimum width as 1.5 times the chessboard width
    int newWidth = chessBoard->minimumSize().width() * 1.5;
    int newHeight = chessBoard->minimumSize().height(); // Keep the original height

    // Set the minimum size of the MainWindow
    setFixedSize(newWidth, newHeight);

    setWindowIcon(QIcon(":/images/chess_icon.jpg"));
}

MainWindow::~MainWindow()
{

}
