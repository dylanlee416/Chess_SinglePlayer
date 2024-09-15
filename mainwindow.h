#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ChessBoard;
class StatusPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    ChessBoard* chessBoard;
    StatusPanel* statusPanel;
};

#endif // MAINWINDOW_H
