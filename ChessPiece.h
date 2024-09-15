#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QString>
#include <QPoint>
#include <QVector>

class ChessPiece
{
public:
    ChessPiece(bool isWhite) : isWhite(isWhite), isMove(false) {}
    virtual ~ChessPiece() {}

    virtual QString getType() const = 0;  // 返回棋子类型
    bool isWhitePiece() const { return isWhite; }
    bool isMoved() const { return isMove; }
    virtual QString getImagePath() const = 0;
    virtual bool isMoveValid(int startRow, int startCol, int endRow, int endCol, ChessPiece* board[8][8],
                             ChessPiece* lastMovedPiece, QPoint lastMoveStart, QPoint lastMoveEnd) = 0;

    virtual QVector<QPoint> getPossibleMoves(int startRow, int startCol, ChessPiece* board[8][8],
                                             ChessPiece* lastMovedPiece, QPoint lastMoveStart, QPoint lastMoveEnd) = 0;

    void setMoved() { isMove = true; }

protected:
    bool isWhite;
    bool isMove;
};

#endif // CHESSPIECE_H
