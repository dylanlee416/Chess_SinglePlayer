#include <QMessageBox>
#include <QPushButton>
#include <QVector>
#include <QIcon>
#include <QDir>
#include <QStringList>

#include "chessboard.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

#include "promotiondialog.h"

ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent), selectedSquare(-1, -1), lastMovedPiece(nullptr)
{
    gridLayout = new QGridLayout(this);

    // 设置布局的 padding（外边距）
    int padding = 20;  // 设定 padding 的大小，比如20像素
    gridLayout->setContentsMargins(padding, padding, padding, padding);

    // 设置方格之间的间距（可选）
    int spacing = 0;  // 设定格子之间的间距，比如5像素
    gridLayout->setSpacing(spacing);

    setupBoard();
    initializePieces();

    // 固定窗口大小，考虑 padding 和 spacing 的影响
    int boardSize = 8 * squareSize + 2 * padding + 7 * spacing;
    setFixedSize(boardSize, boardSize);  // 计算后的窗口大小

    currentMoveColor = true;
    isGaming = false;
    step = 1;
    castleIndex = 0;
    eatOnePieceDistance = 0;
}

void ChessBoard::initialGameRecordFile() {
    // Determine the file name and open the file for writing
    QDir dir("gameRecords");

    // Create the directory if it doesn't exist
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Failed to create directory 'gameRecords'";
        }
    }

    // List the files in the directory
    QStringList fileNames = dir.entryList(QDir::Files);
    int count = fileNames.size();

    // Create the file name based on the file count
    gameRecordFileName = QString("gameRecords/game_%1.txt").arg(count);

    // Debugging output to check file path
    qDebug() << "Game record file path:" << gameRecordFileName;

    // Check if the file opens for writing
    QFile file(gameRecordFileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // Get current date and time
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");

        // Get the game time from the status panel (e.g., 15 minutes)
        int gameTime = statusPanel->getGameTime();  // Assuming it returns a string like "15 minutes"

        // Write the header information to the file
        out << "Game Record\n";
        out << "Time Control: " << gameTime << "s\n";
        out << "Start Time: " << formattedDateTime << "\n\n";

        file.close();
    } else {
        qDebug() << "Failed to open file for writing:" << gameRecordFileName;
    }
}


void ChessBoard::setupBoard()
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            squares[row][col] = new QPushButton(this);
            squares[row][col]->setFixedSize(squareSize, squareSize);
            // 设置背景颜色
            if ((row + col) % 2 == 0) {
                squares[row][col]->setStyleSheet(whiteSquareColor);
            } else {
                squares[row][col]->setStyleSheet(blackSquareColor);
            }

            // 初始化时设置属性，禁用点击效果
            squares[row][col]->setFocusPolicy(Qt::NoFocus);
            squares[row][col]->setStyleSheet(squares[row][col]->styleSheet() + " border: none;");

            gridLayout->addWidget(squares[row][col], row, col);
            pieces[row][col] = nullptr; // 初始化棋盘为空

            // 连接信号槽
            connect(squares[row][col], &QPushButton::clicked, [=]() { onSquareClicked(row, col); });
        }
    }
}

void ChessBoard::initializePieces()
{
    // 设置棋子并将它们放置在棋盘上
    for (int col = 0; col < 8; ++col) {
        setPiece(new Pawn(false), 1, col);
        setPiece(new Pawn(true), 6, col);
    }

    setPiece(new Rook(false), 0, 0);
    setPiece(new Rook(false), 0, 7);
    setPiece(new Rook(true), 7, 0);
    setPiece(new Rook(true), 7, 7);

    setPiece(new Knight(false), 0, 1);
    setPiece(new Knight(false), 0, 6);
    setPiece(new Knight(true), 7, 1);
    setPiece(new Knight(true), 7, 6);

    setPiece(new Bishop(false), 0, 2);
    setPiece(new Bishop(false), 0, 5);
    setPiece(new Bishop(true), 7, 2);
    setPiece(new Bishop(true), 7, 5);

    setPiece(new Queen(false), 0, 3);
    setPiece(new Queen(true), 7, 3);

    setPiece(new King(false), 0, 4);
    setPiece(new King(true), 7, 4);
}

void ChessBoard::setPiece(ChessPiece* piece, int row, int col)
{
    if (pieces[row][col] != nullptr)
    {
        // 更新上一次吃子距离数
        if (pieces[row][col] -> isWhitePiece() != piece -> isWhitePiece()) eatOnePieceDistance = 0;
        delete pieces[row][col];
    }

    pieces[row][col] = piece;
    QPixmap pixmap(piece->getImagePath());
    squares[row][col]->setIcon(QIcon(pixmap));
    squares[row][col]->setIconSize(QSize(64, 64));
}

void ChessBoard::onSquareClicked(int row, int col)
{
    // 恢复上一次选中格子的颜色
    if (selectedSquare != QPoint(-1, -1)) {
        resetSquareColor(selectedSquare.x(), selectedSquare.y());
        clearHighlightedSquares();  // 清除之前的高亮

        if (pieces[selectedSquare.x()][selectedSquare.y()]) {
            if (isMoveValid(selectedSquare.x(), selectedSquare.y(), row, col))
            {
                // 进行棋子的移动
                movePiece(selectedSquare.x(), selectedSquare.y(), row, col);
                selectedSquare = QPoint(-1, -1);  // 重置选择的棋子位置
            }
            else if(pieces[row][col])
            {
                squares[row][col]->setStyleSheet(selectSquareColor);

                // 获取所有可能的移动位置并高亮
                QVector<QPoint> moves = pieces[row][col]->getPossibleMoves(row, col, pieces, lastMovedPiece, lastMoveStart, lastMoveEnd);
                for (const QPoint& move : moves) {
                    squares[move.x()][move.y()]->setStyleSheet(possibleMoveSquareColor);
                    highlightedSquares.append(move);
                }

                selectedSquare = QPoint(row, col);
            }
        }
    }
    // 上一次选中格子位置为空，直接高亮选中格子即可
    else if(pieces[row][col])
    {
        squares[row][col]->setStyleSheet(selectSquareColor);
        // 获取所有可能的移动位置并高亮
        QVector<QPoint> moves = pieces[row][col]->getPossibleMoves(row, col, pieces, lastMovedPiece, lastMoveStart, lastMoveEnd);

        for (const QPoint& move : moves) {
            squares[move.x()][move.y()]->setStyleSheet(possibleMoveSquareColor);
            highlightedSquares.append(move);
        }

        selectedSquare = QPoint(row, col);
    }
}

void ChessBoard::clearHighlightedSquares()
{
    for (const QPoint& square : highlightedSquares) {
        resetSquareColor(square.x(), square.y());
    }
    highlightedSquares.clear();
}

void ChessBoard::resetSquareColor(int row, int col)
{
    if ((row + col) % 2 == 0) {
        squares[row][col]->setStyleSheet(whiteSquareColor);
    } else {
        squares[row][col]->setStyleSheet(blackSquareColor);
    }
}



bool ChessBoard::isDraw() {
    // 1. 检查是否僵局（Stalemate）
    if (isStalemate()) {
        qDebug() << "Stalemate detected!";
        return true; // 棋局僵持，判定为和棋
    }

    // 2. 检查三次重复局面
    if (isThreefoldRepetition()) {
        qDebug() << "Threefold repetition detected!";
        return true; // 三次重复局面，判定为和棋
    }

    // 3. 检查50回合规则
    if (isFiftyMoveRule()) {
        qDebug() << "50-move rule detected!";
        return true; // 50 回合无吃子或兵移动，判定为和棋
    }

    return false; // 不是和棋
}

bool ChessBoard::isStalemate() {
    // 如果国王被将军，则不能判定为和棋
    if (isKingAttacked()) {
        return false;
    }

    // 遍历己方所有棋子，检查是否有合法的移动
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* piece = pieces[row][col];

            // 只检查己方棋子
            if (piece != nullptr && piece->isWhitePiece() == currentMoveColor) {
                // 获取该棋子的所有可能移动
                QVector<QPoint> possibleMoves = piece->getPossibleMoves(row, col, pieces, lastMovedPiece, lastMoveStart, lastMoveEnd);

                // 检查是否有一个合法的移动
                for (const QPoint& move : possibleMoves) {
                    int endRow = move.x();
                    int endCol = move.y();

                    // 保存棋盘当前状态
                    ChessPiece* originalEndPiece = pieces[endRow][endCol];

                    // 模拟移动
                    pieces[endRow][endCol] = piece;
                    pieces[row][col] = nullptr;

                    // 如果该移动后国王不被将军，说明至少有一个合法移动
                    if (!isKingAttacked()) {
                        // 恢复棋盘状态
                        pieces[row][col] = piece;
                        pieces[endRow][endCol] = originalEndPiece;

                        return false; // 有合法移动，不是和棋
                    }

                    // 恢复棋盘状态
                    pieces[row][col] = piece;
                    pieces[endRow][endCol] = originalEndPiece;
                }
            }
        }
    }

    // 没有合法的移动，且国王未被将军，判定为和棋
    return true;
}

bool ChessBoard::isFiftyMoveRule() {
    if (moveHistory.size() < 100) return false;
    if (eatOnePieceDistance < 100) return false;

    for (int i = moveHistory.size() - 1; i >= moveHistory.size() - 100; --i) {
        ChessPiece* movedPiece = moveHistory[i].piece;
        // Check if the piece moved is a pawn or if the end position of the move is occupied (indicating capture)
        if (dynamic_cast<Pawn*>(movedPiece) != nullptr) return false;
    }
    return true; // Not enough moves or rule does not apply
}

bool ChessBoard::isThreefoldRepetition() {
    if (boardStates.size() >= 6) {
        QString lastState = boardStates[boardStates.size() - 1];
        QString secondLastState = boardStates[boardStates.size() - 3];
        QString thirdLastState = boardStates[boardStates.size() - 5];

        // 确保行动方、棋局状态都一致
        if (lastState == secondLastState && secondLastState == thirdLastState) {
            return true; // 三次重复局面
        }
    }
    return false;
}

bool ChessBoard::isCheckmate() {
    // 找到当前玩家的国王位置
    King* king = nullptr;
    QPoint kingPos;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* piece = pieces[row][col];
            if (piece != nullptr && dynamic_cast<King*>(piece) != nullptr && piece->isWhitePiece() == currentMoveColor) {
                king = dynamic_cast<King*>(piece);
                kingPos = QPoint(row, col);
                break;
            }
        }
        if (king != nullptr) break;
    }

    // 如果找不到国王，说明逻辑出错
    if (king == nullptr) {
        qDebug() << "Error: King not found!";
        return false;
    }

    // 如果国王未被将军，则不是将杀
    if (!isKingAttacked()) {
        return false;
    }

    // 遍历己方所有棋子，尝试每一个可能的合法移动
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* piece = pieces[row][col];

            // 只检查己方的棋子
            if (piece != nullptr && piece->isWhitePiece() == currentMoveColor) {
                // 获取该棋子的所有可能移动
                QVector<QPoint> possibleMoves = piece->getPossibleMoves(row, col, pieces, lastMovedPiece, lastMoveStart, lastMoveEnd);

                // 尝试每一个可能的移动
                for (const QPoint& move : possibleMoves) {
                    int endRow = move.x();
                    int endCol = move.y();

                    // 保存棋盘当前状态
                    ChessPiece* originalEndPiece = pieces[endRow][endCol];

                    // 模拟移动
                    pieces[endRow][endCol] = piece;
                    pieces[row][col] = nullptr;

                    // 检查移动后国王是否仍然处于被将军状态
                    bool stillAttacked = isKingAttacked();

                    // 恢复棋盘状态
                    pieces[row][col] = piece;
                    pieces[endRow][endCol] = originalEndPiece;

                    // 如果有一种移动可以让国王不再被将军，说明不是将杀
                    if (!stillAttacked) {
                        return false;
                    }
                }
            }
        }
    }

    // 如果没有任何可以解救国王的移动，说明是将杀
    return true;
}

void ChessBoard::checkForCheckmateOrDraw() {
    if (isCheckmate()) {
        // 创建消息框
        QMessageBox msgBox;
        msgBox.setWindowTitle("Checkmate!");

        // 设置文本和去除图标
        msgBox.setText(currentMoveColor ? "Black wins." : "White wins.");
        msgBox.setIcon(QMessageBox::NoIcon);  // 去除图标

        // 调整字体大小并设置居中对齐
        msgBox.setStyleSheet("QLabel { font-size: 18px; font-weight: bold; text-align: center; } QPushButton { font-size: 10px; }");

        msgBox.setStandardButtons(QMessageBox::Ok);

        // 设置 QLabel 对齐方式为居中
        QLabel *label = msgBox.findChild<QLabel *>();
        if (label) {
            label->setAlignment(Qt::AlignCenter);
        }

        // 添加 spacer 来调整消息框大小
        QSpacerItem* spacer = new QSpacerItem(200, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(spacer, layout->rowCount(), 0, 1, layout->columnCount());

        msgBox.exec();

        isGaming = false;
    } else if (isDraw()) {
        // 创建和棋消息框
        QMessageBox msgBox;
        msgBox.setWindowTitle("Draw!");

        // 设置文本和去除图标
        msgBox.setText("Stalemate");
        msgBox.setIcon(QMessageBox::NoIcon);  // 去除图标

        // 调整字体大小并设置居中对齐
        msgBox.setStyleSheet("QLabel { font-size: 20px; font-weight: bold; text-align: center; } QPushButton { font-size: 10px; }");

        msgBox.setStandardButtons(QMessageBox::Ok);

        // 设置 QLabel 对齐方式为居中
        QLabel *label = msgBox.findChild<QLabel *>();
        if (label) {
            label->setAlignment(Qt::AlignCenter);
        }

        // 添加 spacer 来调整消息框大小
        QSpacerItem* spacer = new QSpacerItem(200, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(spacer, layout->rowCount(), 0, 1, layout->columnCount());

        msgBox.exec();

        isGaming = false;
    }
}

bool ChessBoard::isKingAttacked() {
    King* king = nullptr;
    QPoint kingPos;

    // 寻找国王的位置
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* piece = pieces[row][col];
            if (piece != nullptr && dynamic_cast<King*>(piece) != nullptr && piece->isWhitePiece() == currentMoveColor) {
                king = dynamic_cast<King*>(piece);
                kingPos = QPoint(row, col);
                break;
            }
        }
        if (king != nullptr) break;
    }

    if (isSquareAttacked(kingPos)) return true;

    return false;
}

bool ChessBoard::isSquareAttacked(QPoint square)
{
    int row = square.x();
    int col = square.y();

    // 遍历棋盘上的所有棋子
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            ChessPiece* piece = pieces[i][j];

            // 如果该位置有棋子，并且棋子属于敌方（即检查 byWhite 攻击时，棋子为白色，反之为黑色）
            if (piece != nullptr && piece->isWhitePiece() != currentMoveColor) {

                // 获取该棋子的所有可能的移动位置
                QVector<QPoint> possibleMoves = piece->getPossibleMoves(i, j, pieces, lastMovedPiece, lastMoveStart, lastMoveEnd);

                // 遍历棋子的所有可能移动位置，检查是否能攻击到目标格子
                for (const QPoint& move : possibleMoves) {
                    if (move.x() == row && move.y() == col) {
                        return true;  // 如果目标格子在该棋子的可能移动范围内，则该格子被攻击
                    }
                }
            }
        }
    }

    return false;  // 如果没有任何敌方棋子可以攻击目标格子，则返回false
}

bool ChessBoard::isMoveValid(int startRow, int startCol, int endRow, int endCol)
{
    if (startRow == endRow && startCol == endCol) return false;

    ChessPiece* piece = pieces[startRow][startCol];

    // 将这些信息传递给具体棋子的 isMoveValid 方法
    return piece->isMoveValid(startRow, startCol, endRow, endCol, pieces, lastMovedPiece, lastMoveStart, lastMoveEnd);
}


void ChessBoard::movePiece(int startRow, int startCol, int endRow, int endCol) {
    if (!isGaming) return;

    ChessPiece* piece = pieces[startRow][startCol];
    if (!piece || piece->isWhitePiece() != currentMoveColor) return;

    qDebug() << "It's" << (currentMoveColor ? "White'" : "Black'") <<"turn!";

    // 模拟移动并检测将军状态
    if (!tryMovePiece(startRow, startCol, endRow, endCol)) {
        qDebug() << "After Moving, King would be attacked, move canceled.";
        return; // 移动无效，取消
    }

    // 处理特殊移动
    if (!handleCastling(startRow, startCol, endRow, endCol, piece)) return;

    handleEnPassant(startRow, startCol, endRow, endCol, piece);

    // 成功完成移动后交换动子方
    switchMove(startRow, startCol, endRow, endCol, piece);

    recordMoveHistory(piece, QPair<QPoint, QPoint>(QPoint(startRow, startCol), QPoint(endRow, endCol)));

    // 检查是否和棋或被将杀
    checkForCheckmateOrDraw();
}

void ChessBoard::switchMove(int startRow, int startCol, int endRow, int endCol, ChessPiece*& piece) {
    // 记录移动信息
    lastMoveStart = QPoint(startRow, startCol);
    lastMoveEnd = QPoint(endRow, endCol);
    lastMovedPiece = piece;

    // 更新棋盘
    pieces[startRow][startCol] = nullptr;
    squares[startRow][startCol]->setIcon(QIcon());
    setPiece(piece, endRow, endCol);

    // 处理升变
    handlePromotion(endRow, endCol, piece);

    // 更新位置并交换当前行动方
    currentMoveColor = !currentMoveColor;
    statusPanel->switchTurns();
    piece->setMoved();

    ++ eatOnePieceDistance;
}

bool ChessBoard::tryMovePiece(int startRow, int startCol, int endRow, int endCol) {
    ChessPiece* piece = pieces[startRow][startCol];
    ChessPiece* capturedPiece = pieces[endRow][endCol];

    // 临时移动棋子
    pieces[endRow][endCol] = piece;
    pieces[startRow][startCol] = nullptr;

    bool kingAttacked = isKingAttacked();

    // 恢复原来的棋盘状态
    pieces[startRow][startCol] = piece;
    pieces[endRow][endCol] = capturedPiece;

    return !kingAttacked;
}

bool ChessBoard::handleCastling(int startRow, int startCol, int endRow, int endCol, ChessPiece* piece) {
    if (dynamic_cast<King*>(piece) == nullptr || startCol != 4 || (endCol != 6 && endCol != 2)) {
        return true;
    }

    int baseRow = piece->isWhitePiece() ? 7 : 0;
    if (isKingAttacked()) return false;

    if (endCol == 6 && !isSquareAttacked(QPoint(baseRow, 5))) { // 王侧易位
        qDebug() << "Short Castling.";
        castleIndex = 1;
        moveRookForCastling(baseRow, 7, 5);
    } else if (endCol == 2 && !isSquareAttacked(QPoint(baseRow, 3)) && !isSquareAttacked(QPoint(baseRow, 2))) { // 后侧易位
        qDebug() << "Long Castling.";
        castleIndex = 2;
        moveRookForCastling(baseRow, 0, 3);
    } else {
        qDebug() << "Castling not allowed: King would pass through or end in attacked squares.";
        return false;
    }

    return true;
}

bool ChessBoard::handleEnPassant(int startRow, int startCol, int endRow, int endCol, ChessPiece* piece) {
    if (dynamic_cast<Pawn*>(piece) != nullptr && pieces[endRow][endCol] == nullptr &&
        lastMovedPiece && dynamic_cast<Pawn*>(lastMovedPiece) != nullptr &&
        abs(lastMoveEnd.x() - lastMoveStart.x()) == 2 &&
        lastMoveEnd.y() == endCol && startRow == lastMoveEnd.x() &&
        abs(lastMoveEnd.y() - startCol) == 1) {

        qDebug() << "En Passant!";
        ChessPiece* deletePawn = pieces[lastMoveEnd.x()][lastMoveEnd.y()];
        pieces[lastMoveEnd.x()][lastMoveEnd.y()] = nullptr;
        squares[lastMoveEnd.x()][lastMoveEnd.y()]->setIcon(QIcon());
        delete deletePawn;

        eatOnePieceDistance = 0;
        return true;
    }
    return false;
}

bool ChessBoard::handlePromotion(int endRow, int endCol, ChessPiece*& piece) {
    if (dynamic_cast<Pawn*>(piece) != nullptr && (endRow == 0 || endRow == 7)) {
        PromotionDialog promotionDialog(this, piece->isWhitePiece());

        // 获取棋盘格的按钮位置
        QPoint piecePosition = squares[endRow][endCol]->mapToGlobal(QPoint(0, 0));

        // 调整对话框位置
        promotionDialog.move(piecePosition.x(), piecePosition.y());

        if (promotionDialog.exec() == QDialog::Accepted) {
            // 更新 piece 指针为用户选择的新棋子
            piece = promotionDialog.getSelectedPiece();
            setPiece(piece, endRow, endCol); // 确保棋盘设置了新棋子
        }

        return true;
    }
    return false;
}

void ChessBoard::moveRookForCastling(int row, int rookStartCol, int rookEndCol) {
    ChessPiece* rook = pieces[row][rookStartCol];
    pieces[row][rookStartCol] = nullptr;
    squares[row][rookStartCol]->setIcon(QIcon());
    setPiece(rook, row, rookEndCol);
    rook->setMoved();
}

QString ChessBoard::getBoardState() const {
    QString state;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* piece = pieces[row][col];
            if (piece) {
                // 用棋子的类型和颜色表示棋子，格式如 "WPA1" 代表白兵在A1
                state += QString(piece->isWhitePiece() ? 'w' : 'b') + piece->getType() + '\t';
            } else {
                state += "00\t"; // 表示空格
            }
        }
        state += "\n";
    }
    return state;
}

void ChessBoard::appendToGameRecordFile(const QString& content)
{
    QFile file(gameRecordFileName);

    // Attempt to open the file in append mode
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << content << "\n"; // Append content with a newline
        file.close();
    } else {
        // If the file fails to open, log an error message
        qDebug() << "Failed to open file for appending:" << gameRecordFileName;
    }
}

void ChessBoard::recordMoveHistory(ChessPiece* piece, QPair<QPoint, QPoint> move)
{
    MoveHistoryEntry entry = { piece, move };
    moveHistory.append(entry);

    QString currentState = getBoardState();
    boardStates.append(currentState);

    ++step;

    QPoint endPos = move.second;
    QString curMove, moveStr;
    QString pieceName = piece->getType();


    curMove = QString("%1%2%3").arg(pieceName).arg(QChar('a' + endPos.y())).arg(8 - endPos.x());
    if (step % 2 == 0)
    {
        moveStr = QString("%1.  %2%3%4").arg(step / 2).arg(pieceName).arg(QChar('a' + endPos.y())).arg(8 - endPos.x());

        if (castleIndex == 1)
        {
            moveStr = QString("%1.  O-O").arg(step / 2);
            curMove = QString("O-O");
        }
        if (castleIndex == 2)
        {
            moveStr = QString("%1.  O-O-O").arg(step / 2);
            curMove = QString("O-O-O");
        }
    }
    else
    {
        moveStr = QString("\t%1%2%3").arg(pieceName).arg(QChar('a' + endPos.y())).arg(8 - endPos.x());

        if (castleIndex == 1)
        {
            moveStr = QString("\tO-O");
            curMove = QString("\tO-O");
        }
        if (castleIndex == 2)
        {
            moveStr = QString("\tO-O-O");
            curMove = QString("\tO-O-O");
        }
    }

    castleIndex = 0;

    // Update the status panel with the new move
    statusPanel->addMoveToHistory(moveStr, step);

    QString contentToAppend = QString("Step %1: %2\n%3")
                                  .arg(moveHistory.size())
                                  .arg(curMove, currentState);

    // Append to game record file
    appendToGameRecordFile(contentToAppend);
}
