#ifndef KNIGHT_H
#define KNIGHT_H

#include "chesspiece.h"

class Knight : public ChessPiece
{
public:
    Knight(bool isWhite) : ChessPiece(isWhite) {}

    QString getType() const override {
        return "N";
    }

    QString getImagePath() const override {
        return isWhite ? ":/images/white_knight.svg.png" : ":/images/black_knight.svg.png";
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
        static const int rowOffsets[] = { -2, -1, 1, 2,  2,  1, -1, -2 };
        static const int colOffsets[] = {  1,  2, 2, 1, -1, -2, -2, -1 };

        for (int i = 0; i < 8; ++i) {
            int newRow = startRow + rowOffsets[i];
            int newCol = startCol + colOffsets[i];

            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                if (board[newRow][newCol] == nullptr || board[newRow][newCol]->isWhitePiece() != isWhite) {
                    moves.append(QPoint(newRow, newCol));
                }
            }
        }

        return moves;
    }

};

#endif // KNIGHT_H
