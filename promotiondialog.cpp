#include "promotiondialog.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"

PromotionDialog::PromotionDialog(QWidget* parent, bool isWhite)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::Dialog), // 使用 FramelessWindowHint 来隐藏状态栏
    selectedPiece(nullptr), isWhite(isWhite)
{
    // 设置对话框边框为绿色
    setStyleSheet("PromotionDialog { border: 2px solid green; border-radius: 10px; }");

    QHBoxLayout* layout = new QHBoxLayout(this);

    // 设置布局的上下左右内边距和控件之间的间距
    int padding = 2;  // 较小的边距使布局更加紧凑
    layout->setContentsMargins(padding, padding, padding, padding);
    layout->setSpacing(0);  // 减少按钮之间的间距

    // 创建按钮并设置图标
    queenButton = new QPushButton(this);
    rookButton = new QPushButton(this);
    bishopButton = new QPushButton(this);
    knightButton = new QPushButton(this);

    // 设置按钮大小和图标大小
    int buttonSize = 64;  // 更小的按钮尺寸
    queenButton->setFixedSize(buttonSize, buttonSize);
    queenButton->setIcon(QIcon(isWhite ? ":/images/white_queen.svg.png" : ":/images/black_queen.svg.png"));
    queenButton->setIconSize(QSize(buttonSize, buttonSize));

    rookButton->setFixedSize(buttonSize, buttonSize);
    rookButton->setIcon(QIcon(isWhite ? ":/images/white_rook.svg.png" : ":/images/black_rook.svg.png"));
    rookButton->setIconSize(QSize(buttonSize, buttonSize));

    bishopButton->setFixedSize(buttonSize, buttonSize);
    bishopButton->setIcon(QIcon(isWhite ? ":/images/white_bishop.svg.png" : ":/images/black_bishop.svg.png"));
    bishopButton->setIconSize(QSize(buttonSize, buttonSize));

    knightButton->setFixedSize(buttonSize, buttonSize);
    knightButton->setIcon(QIcon(isWhite ? ":/images/white_knight.svg.png" : ":/images/black_knight.svg.png"));
    knightButton->setIconSize(QSize(buttonSize, buttonSize));

    // 添加按钮到布局
    layout->addWidget(queenButton);
    layout->addWidget(rookButton);
    layout->addWidget(bishopButton);
    layout->addWidget(knightButton);

    setLayout(layout);

    // 调整对话框的固定大小，考虑边距
    int dialogWidth = 4 * buttonSize + 3 * layout->spacing(); // 4个按钮 + 按钮之间的间距 + 左右边距
    int dialogHeight = buttonSize + padding; // 按钮高度 + 上下边距
    setFixedSize(dialogWidth, dialogHeight);

    // 连接按钮的点击信号到对应的槽
    connect(queenButton, &QPushButton::clicked, this, [this, isWhite] {
        selectedPiece = new Queen(isWhite);  // 根据颜色实例化
        emit promotionSelected(selectedPiece);
        accept();  // 关闭对话框并返回 Accepted 状态
    });

    connect(rookButton, &QPushButton::clicked, this, [this, isWhite] {
        selectedPiece = new Rook(isWhite);
        emit promotionSelected(selectedPiece);
        accept();
    });

    connect(bishopButton, &QPushButton::clicked, this, [this, isWhite] {
        selectedPiece = new Bishop(isWhite);
        emit promotionSelected(selectedPiece);
        accept();
    });

    connect(knightButton, &QPushButton::clicked, this, [this, isWhite] {
        selectedPiece = new Knight(isWhite);
        emit promotionSelected(selectedPiece);
        accept();
    });
}

PromotionDialog::~PromotionDialog()
{
}

ChessPiece* PromotionDialog::getSelectedPiece() const
{
    return selectedPiece;
}
