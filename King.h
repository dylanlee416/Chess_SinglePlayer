#ifndef KING_H
#define KING_H

#include "chesspiece.h"
#include "rook.h"

class King : public ChessPiece
{
public:
    King(bool isWhite) : ChessPiece(isWhite) {}

    QString getType() const override {
        return "K";
    }

    QString getImagePath() const override {
        return isWhite ? ":/images/white_king.svg.png" : ":/images/black_king.svg.png";
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
        static const int rowOffsets[] = { -1, -1, -1,  0, 0,  1,  1,  1 };
        static const int colOffsets[] = { -1,  0,  1, -1, 1, -1,  0,  1 };

        // 普通移动：检查周围8格
        for (int i = 0; i < 8; ++i) {
            int newRow = startRow + rowOffsets[i];
            int newCol = startCol + colOffsets[i];

            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                if (board[newRow][newCol] == nullptr || board[newRow][newCol]->isWhitePiece() != isWhite) {
                    moves.append(QPoint(newRow, newCol));
                }
            }
        }

        // 王车易位逻辑
        if (!isMoved()) {
            // 0 表示白方的底线, 7 表示黑方的底线
            int baseRow = isWhitePiece() ? 7 : 0;

            if (startRow == baseRow) {
                // 王侧易位（Short castling）
                if (board[baseRow][7] != nullptr && dynamic_cast<Rook*>(board[baseRow][7]) != nullptr && !board[baseRow][7]->isMoved()) {
                    if (board[baseRow][5] == nullptr && board[baseRow][6] == nullptr) {
                        // 需要确保国王不会经过被攻击的格子，通常在其他地方会检查。
                        // 这里假设没有攻击检测（可以通过isKingAttacked函数来实现）。
                        moves.append(QPoint(baseRow, 6));  // 国王移动到g列 (6)
                    }
                }

                // 后侧易位（Long castling）
                if (board[baseRow][0] != nullptr && dynamic_cast<Rook*>(board[baseRow][0]) != nullptr && !board[baseRow][0]->isMoved()) {
                    if (board[baseRow][1] == nullptr && board[baseRow][2] == nullptr && board[baseRow][3] == nullptr) {
                        moves.append(QPoint(baseRow, 2));  // 国王移动到c列 (2)
                    }
                }
            }
        }

        return moves;
    }

};

#endif // KING_H
