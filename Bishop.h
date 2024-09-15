#ifndef BISHOP_H
#define BISHOP_H

#include "chesspiece.h"

class Bishop : public ChessPiece
{
public:
    Bishop(bool isWhite) : ChessPiece(isWhite) {}

    QString getType() const override {
        return "B";
    }

    QString getImagePath() const override {
        return isWhite ? ":/images/white_bishop.svg.png" : ":/images/black_bishop.svg.png";
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

        // 四个对角线方向
        for (int i = 1; i < 8; ++i) {
            if (startRow + i < 8 && startCol + i < 8 && board[startRow + i][startCol + i] == nullptr) {
                moves.append(QPoint(startRow + i, startCol + i));
            } else if (startRow + i < 8 && startCol + i < 8 && board[startRow + i][startCol + i]->isWhitePiece() != isWhite) {
                moves.append(QPoint(startRow + i, startCol + i));
                break;
            } else {
                break;
            }
        }
        for (int i = 1; i < 8; ++i) {
            if (startRow - i >= 0 && startCol + i < 8 && board[startRow - i][startCol + i] == nullptr) {
                moves.append(QPoint(startRow - i, startCol + i));
            } else if (startRow - i >= 0 && startCol + i < 8 && board[startRow - i][startCol + i]->isWhitePiece() != isWhite) {
                moves.append(QPoint(startRow - i, startCol + i));
                break;
            } else {
                break;
            }
        }
        for (int i = 1; i < 8; ++i) {
            if (startRow + i < 8 && startCol - i >= 0 && board[startRow + i][startCol - i] == nullptr) {
                moves.append(QPoint(startRow + i, startCol - i));
            } else if (startRow + i < 8 && startCol - i >= 0 && board[startRow + i][startCol - i]->isWhitePiece() != isWhite) {
                moves.append(QPoint(startRow + i, startCol - i));
                break;
            } else {
                break;
            }
        }
        for (int i = 1; i < 8; ++i) {
            if (startRow - i >= 0 && startCol - i >= 0 && board[startRow - i][startCol - i] == nullptr) {
                moves.append(QPoint(startRow - i, startCol - i));
            } else if (startRow - i >= 0 && startCol - i >= 0 && board[startRow - i][startCol - i]->isWhitePiece() != isWhite) {
                moves.append(QPoint(startRow - i, startCol - i));
                break;
            } else {
                break;
            }
        }

        return moves;
    }

};

#endif // BISHOP_H
