#include "pages/victorypage.h"

#include "core/uiscale.h"

#include <QApplication>
#include <QDebug>
#include <QFrame>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

VictoryPage::VictoryPage(QWidget *parent)
    : QWidget(parent)
    , restartButton(nullptr)
    , returnButton(nullptr)
    , exitButton(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(UiScale::size(1280, 820));

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(UiScale::margins(96, 54, 96, 54));
    rootLayout->addStretch(1);

    auto *card = new QFrame(this);
    card->setObjectName("endCard");
    card->setMaximumWidth(UiScale::width(760));

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(UiScale::margins(42, 40, 42, 34));
    cardLayout->setSpacing(UiScale::scaled(16));

    auto *titleLabel = new QLabel(QStringLiteral("游戏胜利"), card);
    titleLabel->setObjectName("pageTitle");
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *subtitleLabel = new QLabel(QStringLiteral("The arena bends to your strategy."), card);
    subtitleLabel->setObjectName("pageSubtitle");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    restartButton = new QPushButton(QStringLiteral("重新开始"), card);
    restartButton->setObjectName("primaryButton");
    returnButton = new QPushButton(QStringLiteral("返回开始页"), card);
    returnButton->setObjectName("secondaryButton");
    exitButton = new QPushButton(QStringLiteral("退出游戏"), card);
    exitButton->setObjectName("secondaryButton");

    for (QPushButton *button : {restartButton, returnButton, exitButton}) {
        button->setCursor(Qt::PointingHandCursor);
        button->setMinimumHeight(UiScale::height(54));
    }

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addSpacing(UiScale::scaled(8));
    cardLayout->addWidget(restartButton);
    cardLayout->addWidget(returnButton);
    cardLayout->addWidget(exitButton);

    auto *cardRow = new QHBoxLayout;
    cardRow->addStretch(1);
    cardRow->addWidget(card);
    cardRow->addStretch(1);
    rootLayout->addLayout(cardRow);
    rootLayout->addStretch(1);

    setStyleSheet(UiScale::scaleStyleSheet(QStringLiteral(R"(
        VictoryPage { background-color: #090d15; }
        #endCard {
            background-color: rgba(10, 16, 28, 218);
            border: 2px solid rgba(178, 151, 98, 190);
            border-radius: 30px;
        }
        #pageTitle {
            color: #f2dfaa;
            font-size: 40px;
            font-weight: 800;
        }
        #pageSubtitle {
            color: #a4b6ce;
            font-size: 17px;
            font-weight: 500;
        }
        #primaryButton, #secondaryButton {
            color: #ffffff;
            border-radius: 18px;
            font-size: 18px;
            font-weight: 700;
            padding: 12px 24px;
        }
        #primaryButton {
            background-color: #7d5a2c;
            border: 2px solid #dbc183;
        }
        #primaryButton:hover {
            background-color: #98723f;
        }
        #secondaryButton {
            background-color: #28344f;
            border: 2px solid #7082ab;
        }
        #secondaryButton:hover {
            background-color: #364566;
        }
    )")));

    connect(restartButton, &QPushButton::clicked, this, &VictoryPage::restartClicked);
    connect(returnButton, &QPushButton::clicked, this, &VictoryPage::returnToStartClicked);
    connect(exitButton, &QPushButton::clicked, this, &VictoryPage::exitClicked);
}

void VictoryPage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(9, 13, 21));

    QLinearGradient glow(0, 0, width(), height());
    glow.setColorAt(0.0, QColor(43, 62, 29, 110));
    glow.setColorAt(0.55, QColor(8, 13, 23, 0));
    glow.setColorAt(1.0, QColor(69, 49, 18, 100));
    painter.fillRect(rect(), glow);
}
