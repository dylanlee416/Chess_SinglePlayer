#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QPoint>
#include "chesspiece.h"
#include "statuspanel.h"

struct MoveHistoryEntry {
    ChessPiece* piece;       // The piece involved in the move
    QPair<QPoint, QPoint> move;  // The move: start and end positions
};

class ChessBoard : public QWidget {
    Q_OBJECT
public:
    ChessBoard(QWidget *parent = nullptr);

    void setStatusPanel(StatusPanel* _statusPanel) { statusPanel = _statusPanel; }
    void startGame() { isGaming = true; initialGameRecordFile(); }
    bool getIsGaming() { return isGaming; }
    void timeRunOut() { isGaming = false; }
    bool isSquareAttacked(QPoint square);

private:
    StatusPanel* statusPanel;
    QVector<QString> boardStates;  // 记录每一步的棋盘状态
    QVector<MoveHistoryEntry> moveHistory;

    QGridLayout *gridLayout;
    QPushButton* squares[8][8];
    ChessPiece* pieces[8][8];
    int squareSize = 64;


    int step;
    bool isGaming;
    int castleIndex;
    bool currentMoveColor;
    int eatOnePieceDistance;


    const QString whiteSquareColor = "background-color: white;";
    const QString blackSquareColor = "background-color: green;";
    const QString selectSquareColor = "background-color: yellow;";
    const QString possibleMoveSquareColor = "background-color: red;";

    QPoint selectedSquare;
    QVector<QPoint> highlightedSquares;  // 存储高亮的格子

    ChessPiece* lastMovedPiece; // 记录上一次移动的棋子
    QPoint lastMoveStart;  // 记录上一次移动的起始位置
    QPoint lastMoveEnd;    // 记录上一次移动的结束位置


    void setupBoard();
    void initializePieces();
    void onSquareClicked(int row, int col);

    bool isMoveValid(int startRow, int startCol, int endRow, int endCol);
    void movePiece(int startRow, int startCol, int endRow, int endCol);
    void switchMove(int startRow, int startCol, int endRow, int endCol, ChessPiece*& piece);
    void animatePieceMove(int startRow, int startCol, int endRow, int endCol, ChessPiece* piece);

    void resetSquareColor(int row, int col);
    void clearHighlightedSquares();  // 清除高亮

    void setPiece(ChessPiece* piece, int row, int col);

    bool isDraw();
    bool isFiftyMoveRule();
    bool isThreefoldRepetition();
    bool isStalemate();
    bool isKingAttacked();
    bool isCheckmate();
    void checkForCheckmateOrDraw();

    bool tryMovePiece(int startRow, int startCol, int endRow, int endCol);
    void moveRookForCastling(int row, int rookStartCol, int rookEndCol);
    bool handleCastling(int startRow, int startCol, int endRow, int endCol, ChessPiece* piece);
    bool handleEnPassant(int startRow, int startCol, int endRow, int endCol, ChessPiece* piece);
    bool handlePromotion(int endRow, int endCol, ChessPiece*& piece);
    ChessPiece* showPromotionDialog(ChessPiece* piece);

    QString gameRecordFileName;
    void initialGameRecordFile();
    QString getBoardState() const;
    void recordMoveHistory(ChessPiece* piece, QPair<QPoint, QPoint> move);
    void appendToGameRecordFile(const QString& content);
};

#endif // CHESSBOARD_H
