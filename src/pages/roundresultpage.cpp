#include "pages/roundresultpage.h"

#include <QDebug>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

namespace {

QFrame *createInfoTile(const QString &labelText, QLabel *valueLabel, QWidget *parent)
{
    auto *tile = new QFrame(parent);
    tile->setObjectName("infoTile");

    auto *layout = new QVBoxLayout(tile);
    layout->setContentsMargins(18, 16, 18, 16);
    layout->setSpacing(6);

    auto *label = new QLabel(labelText, tile);
    label->setObjectName("tileLabel");
    valueLabel->setParent(tile);
    valueLabel->setObjectName("tileValue");

    layout->addWidget(label);
    layout->addWidget(valueLabel);
    return tile;
}

}

RoundResultPage::RoundResultPage(QWidget *parent)
    : QWidget(parent)
    , resultTitleLabel(new QLabel(QStringLiteral("本回合胜利"), this))
    , roundValueLabel(new QLabel(QStringLiteral("1"), this))
    , hpValueLabel(new QLabel(QStringLiteral("100"), this))
    , rewardValueLabel(new QLabel(QStringLiteral("0"), this))
    , damageValueLabel(new QLabel(QStringLiteral("0"), this))
    , continueButton(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(1280, 820);

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(90, 54, 90, 54);
    rootLayout->setSpacing(0);
    rootLayout->addStretch(1);

    auto *card = new QFrame(this);
    card->setObjectName("resultCard");
    card->setMaximumWidth(860);

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(42, 38, 42, 34);
    cardLayout->setSpacing(16);

    resultTitleLabel->setObjectName("resultTitle");
    resultTitleLabel->setAlignment(Qt::AlignCenter);

    auto *subtitleLabel = new QLabel(QStringLiteral("Round summary"), card);
    subtitleLabel->setObjectName("pageSubtitle");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    auto *statsGrid = new QGridLayout;
    statsGrid->setHorizontalSpacing(16);
    statsGrid->setVerticalSpacing(16);
    statsGrid->addWidget(createInfoTile(QStringLiteral("当前回合"), roundValueLabel, card), 0, 0);
    statsGrid->addWidget(createInfoTile(QStringLiteral("剩余血量"), hpValueLabel, card), 0, 1);
    statsGrid->addWidget(createInfoTile(QStringLiteral("奖励金币"), rewardValueLabel, card), 1, 0);
    statsGrid->addWidget(createInfoTile(QStringLiteral("受到伤害"), damageValueLabel, card), 1, 1);

    continueButton = new QPushButton(QStringLiteral("继续"), card);
    continueButton->setObjectName("primaryButton");
    continueButton->setCursor(Qt::PointingHandCursor);
    continueButton->setMinimumHeight(56);

    cardLayout->addWidget(resultTitleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addLayout(statsGrid);
    cardLayout->addWidget(continueButton);

    auto *cardRow = new QHBoxLayout;
    cardRow->addStretch(1);
    cardRow->addWidget(card);
    cardRow->addStretch(1);
    rootLayout->addLayout(cardRow);
    rootLayout->addStretch(1);

    setStyleSheet(R"(
        RoundResultPage { background-color: #090d15; }
        #resultCard {
            background-color: rgba(10, 16, 28, 218);
            border: 2px solid rgba(178, 151, 98, 190);
            border-radius: 30px;
        }
        #resultTitle {
            color: #f4e6bd;
            font-size: 36px;
            font-weight: 800;
        }
        #pageSubtitle {
            color: #95a5bd;
            font-size: 16px;
            font-weight: 500;
        }
        #infoTile {
            background-color: rgba(13, 21, 37, 205);
            border: 1px solid rgba(122, 144, 180, 125);
            border-radius: 18px;
        }
        #tileLabel {
            color: #92a4bf;
            font-size: 13px;
            font-weight: 700;
        }
        #tileValue {
            color: #f2f5fb;
            font-size: 26px;
            font-weight: 800;
        }
        #primaryButton {
            color: #ffffff;
            background-color: #7d5a2c;
            border: 2px solid #dbc183;
            border-radius: 18px;
            font-size: 18px;
            font-weight: 700;
            padding: 12px 24px;
        }
        #primaryButton:hover {
            background-color: #98723f;
        }
    )");

    connect(continueButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "Round result continue clicked";
        emit continueClicked();
    });
}

void RoundResultPage::setResultInfo(int round, int hp, const BattleResult &result)
{
    resultTitleLabel->setText(result.win ? QStringLiteral("本回合胜利") : QStringLiteral("本回合失败"));
    roundValueLabel->setText(QString::number(round));
    hpValueLabel->setText(QString::number(hp));
    rewardValueLabel->setText(QString::number(result.rewardGold));
    damageValueLabel->setText(QString::number(result.damage));
}

void RoundResultPage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(9, 13, 21));

    QLinearGradient glow(0, 0, width(), height());
    glow.setColorAt(0.0, QColor(18, 31, 58, 125));
    glow.setColorAt(0.45, QColor(8, 13, 23, 0));
    glow.setColorAt(1.0, QColor(49, 29, 66, 95));
    painter.fillRect(rect(), glow);

    painter.setPen(QPen(QColor(124, 139, 177, 22), 1));
    const int gridSize = 48;
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}
