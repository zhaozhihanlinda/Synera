#include "pages/defeatpage.h"

#include "core/uiscale.h"

#include <QFrame>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

DefeatPage::DefeatPage(QWidget *parent)
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

    auto *titleLabel = new QLabel(QStringLiteral("游戏失败"), card);
    titleLabel->setObjectName("pageTitle");
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *subtitleLabel = new QLabel(QStringLiteral("Recover, rethink, and return to the arena."), card);
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
        DefeatPage { background-color: #090d15; }
        #endCard {
            background-color: rgba(10, 16, 28, 218);
            border: 2px solid rgba(144, 99, 99, 185);
            border-radius: 30px;
        }
        #pageTitle {
            color: #f0b5b5;
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
            background-color: #7c3f3f;
            border: 2px solid #d68f8f;
        }
        #primaryButton:hover {
            background-color: #975252;
        }
        #secondaryButton {
            background-color: #28344f;
            border: 2px solid #7082ab;
        }
        #secondaryButton:hover {
            background-color: #364566;
        }
    )")));

    connect(restartButton, &QPushButton::clicked, this, &DefeatPage::restartClicked);
    connect(returnButton, &QPushButton::clicked, this, &DefeatPage::returnToStartClicked);
    connect(exitButton, &QPushButton::clicked, this, &DefeatPage::exitClicked);
}

void DefeatPage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(9, 13, 21));

    QLinearGradient glow(0, 0, width(), height());
    glow.setColorAt(0.0, QColor(70, 27, 27, 110));
    glow.setColorAt(0.55, QColor(8, 13, 23, 0));
    glow.setColorAt(1.0, QColor(54, 25, 38, 100));
    painter.fillRect(rect(), glow);
}
