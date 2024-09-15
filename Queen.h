#ifndef QUEEN_H
#define QUEEN_H

#include "chesspiece.h"
#include "Bishop.h"
#include "Rook.h"

class Queen : public ChessPiece
{
public:
    Queen(bool isWhite) : ChessPiece(isWhite) {}

    QString getType() const override {
        return "Q";
    }

    QString getImagePath() const override {
        return isWhite ? ":/images/white_queen.svg.png" : ":/images/black_queen.svg.png";
    }

    bool isMoveValid(int startRow, int startCol, int endRow, int endCol, ChessPiece* board[8][8],
                     ChessPiece* lastMovedPiece, QPoint lastMoveStart, QPoint lastMoveEnd) override {

        QVector<QPoint> possibleMoves = getPossibleMoves(startRow, startCol, board, lastMovedPiece, lastMoveStart, lastMoveEnd);

        // Check if the end position is one of the possible moves
        for (const QPoint& move : possibleMoves) {
            if (move.x() == endRow && move.y() == endCol) {
                return true;
            }
        }
        return false;
    }



    QVector<QPoint> getPossibleMoves(int startRow, int startCol, ChessPiece* board[8][8],
                                     ChessPiece* lastMovedPiece, QPoint lastMoveStart, QPoint lastMoveEnd) override {
        QVector<QPoint> moves;

        // Combine Rook and Bishop's moves
        Rook rook(isWhite);
        Bishop bishop(isWhite);

        moves.append(rook.getPossibleMoves(startRow, startCol, board, lastMovedPiece, lastMoveStart, lastMoveEnd));
        moves.append(bishop.getPossibleMoves(startRow, startCol, board, lastMovedPiece, lastMoveStart, lastMoveEnd));

        return moves;
    }

};

#endif // QUEEN_H
