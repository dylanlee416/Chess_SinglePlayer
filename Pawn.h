#ifndef PAWN_H
#define PAWN_H

#include "chesspiece.h"

class Pawn : public ChessPiece
{
public:
    Pawn(bool isWhite) : ChessPiece(isWhite) {}

    QString getType() const override {
        return "P";
    }

    QString getImagePath() const override {
        return isWhite ? ":/images/white_pawn.svg.png" : ":/images/black_pawn.svg.png";
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

        int direction = isWhite ? -1 : 1;
        int nextRow = startRow + direction;

        // 检查前方一格是否为空
        if (nextRow >= 0 && nextRow < 8 && board[nextRow][startCol] == nullptr) {
            moves.append(QPoint(nextRow, startCol));

            // 如果在初始行，并且前方两格都为空，可以前进两格
            int initialRow = isWhite ? 6 : 1;
            if (startRow == initialRow && board[startRow + 2 * direction][startCol] == nullptr) {
                moves.append(QPoint(startRow + 2 * direction, startCol));
            }
        }

        // 检查是否可以吃掉对方棋子
        if (nextRow >= 0 && nextRow < 8) {
            if (startCol > 0 && board[nextRow][startCol - 1] != nullptr && board[nextRow][startCol - 1]->isWhitePiece() != isWhite) {
                moves.append(QPoint(nextRow, startCol - 1));
            }
            if (startCol < 7 && board[nextRow][startCol + 1] != nullptr && board[nextRow][startCol + 1]->isWhitePiece() != isWhite) {
                moves.append(QPoint(nextRow, startCol + 1));
            }
        }

        // 检查是否可以吃过路兵
        if (lastMovedPiece != nullptr && dynamic_cast<Pawn*>(lastMovedPiece) != nullptr && abs(lastMoveEnd.x() - lastMoveStart.x()) == 2) {
            int lastMovedRow = lastMoveEnd.x();
            int lastMovedCol = lastMoveEnd.y();

            // 检查左边的过路兵
            if (startCol > 0 && lastMovedCol == startCol - 1 && lastMovedRow == startRow &&
                board[startRow][startCol - 1] == lastMovedPiece && board[startRow + direction][startCol - 1] == nullptr) {
                moves.append(QPoint(startRow + direction, startCol - 1));
            }

            // 检查右边的过路兵
            if (startCol < 7 && lastMovedCol == startCol + 1 && lastMovedRow == startRow &&
                board[startRow][startCol + 1] == lastMovedPiece && board[startRow + direction][startCol + 1] == nullptr) {
                moves.append(QPoint(startRow + direction, startCol + 1));
            }
        }

        return moves;
    }


};

#endif // PAWN_H
