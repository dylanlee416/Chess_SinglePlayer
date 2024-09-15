#ifndef ROOK_H
#define ROOK_H

#include "chesspiece.h"

class Rook : public ChessPiece
{
public:
    Rook(bool isWhite) : ChessPiece(isWhite) {}

    QString getType() const override {
        return "R";
    }

    QString getImagePath() const override {
        return isWhite ? ":/images/white_rook.svg.png" : ":/images/black_rook.svg.png";
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

        // 水平和垂直方向
        for (int i = 1; i < 8; ++i) {
            if (startRow + i < 8 && board[startRow + i][startCol] == nullptr) {
                moves.append(QPoint(startRow + i, startCol));
            } else if (startRow + i < 8 && board[startRow + i][startCol]->isWhitePiece() != isWhite) {
                moves.append(QPoint(startRow + i, startCol));
                break;
            } else {
                break;
            }
        }
        for (int i = 1; i < 8; ++i) {
            if (startRow - i >= 0 && board[startRow - i][startCol] == nullptr) {
                moves.append(QPoint(startRow - i, startCol));
            } else if (startRow - i >= 0 && board[startRow - i][startCol]->isWhitePiece() != isWhite) {
                moves.append(QPoint(startRow - i, startCol));
                break;
            } else {
                break;
            }
        }
        for (int i = 1; i < 8; ++i) {
            if (startCol + i < 8 && board[startRow][startCol + i] == nullptr) {
                moves.append(QPoint(startRow, startCol + i));
            } else if (startCol + i < 8 && board[startRow][startCol + i]->isWhitePiece() != isWhite) {
                moves.append(QPoint(startRow, startCol + i));
                break;
            } else {
                break;
            }
        }
        for (int i = 1; i < 8; ++i) {
            if (startCol - i >= 0 && board[startRow][startCol - i] == nullptr) {
                moves.append(QPoint(startRow, startCol - i));
            } else if (startCol - i >= 0 && board[startRow][startCol - i]->isWhitePiece() != isWhite) {
                moves.append(QPoint(startRow, startCol - i));
                break;
            } else {
                break;
            }
        }

        return moves;
    }

};

#endif // ROOK_H
