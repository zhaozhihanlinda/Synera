#include "pages/rulepage.h"

#include <QDebug>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

RulePage::RulePage(QWidget *parent)
    : QWidget(parent)
    , startButton(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(1280, 820);

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(96, 54, 96, 54);
    rootLayout->setSpacing(0);
    rootLayout->addStretch(1);

    auto *card = new QFrame(this);
    card->setObjectName("ruleCard");
    card->setMaximumWidth(840);
    auto *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(34);
    shadow->setOffset(0, 12);
    shadow->setColor(QColor(0, 0, 0, 100));
    card->setGraphicsEffect(shadow);

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(46, 42, 46, 36);
    cardLayout->setSpacing(14);

    auto *titleLabel = new QLabel(QStringLiteral("基础规则"), card);
    titleLabel->setObjectName("pageTitle");
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *subtitleLabel = new QLabel(QStringLiteral("Know the arena before the first round."), card);
    subtitleLabel->setObjectName("pageSubtitle");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    auto *rulesLabel = new QLabel(
        QStringLiteral("游戏目标：\n击败所有回合敌人，并保证玩家血量不归零。\n\n"
                       "回合流程：\n商店 → 部署 → 战斗 → 结算。\n\n"
                       "核心资源：\n金币：用于购买角色。\n人口：决定可上场单位数量。\n生命值：归零则失败。\n\n"
                       "胜负条件：\n血量归零则失败。\n打完最终回合并获胜则胜利。"),
        card);
    rulesLabel->setObjectName("rulesBody");
    rulesLabel->setWordWrap(true);

    startButton = new QPushButton(QStringLiteral("开始游戏"), card);
    startButton->setObjectName("primaryButton");
    startButton->setCursor(Qt::PointingHandCursor);
    startButton->setMinimumHeight(56);

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(rulesLabel);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(startButton);

    auto *cardRow = new QHBoxLayout;
    cardRow->addStretch(1);
    cardRow->addWidget(card);
    cardRow->addStretch(1);
    rootLayout->addLayout(cardRow);
    rootLayout->addStretch(1);

    setStyleSheet(R"(
        RulePage { background-color: #090d15; }
        #ruleCard {
            background-color: rgba(11, 18, 31, 214);
            border: 2px solid rgba(176, 145, 91, 180);
            border-radius: 28px;
        }
        #pageTitle {
            color: #e4cd92;
            font-size: 34px;
            font-weight: 800;
        }
        #pageSubtitle {
            color: #96a6be;
            font-size: 16px;
            font-weight: 500;
        }
        #rulesBody {
            color: #dfe7f4;
            font-size: 19px;
            font-weight: 500;
            line-height: 1.45em;
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

    connect(startButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "Rule page start clicked";
        emit startGameClicked();
    });
}

void RulePage::paintEvent(QPaintEvent *event)
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
