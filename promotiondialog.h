#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include "chesspiece.h"

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(QWidget* parent, bool isWhite);
    ~PromotionDialog();

    ChessPiece* getSelectedPiece() const;

signals:
    void promotionSelected(ChessPiece* piece);

private:
    bool isWhite;

    QPushButton* queenButton;
    QPushButton* rookButton;
    QPushButton* bishopButton;
    QPushButton* knightButton;

    ChessPiece* selectedPiece;
};

#endif // PROMOTIONDIALOG_H
