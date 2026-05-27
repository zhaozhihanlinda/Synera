#include "pages/startpage.h"

#include <QFrame>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

StartPage::StartPage(QWidget *parent)
    : QWidget(parent)
    , startButton(nullptr)
    , exitButton(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(1280, 820);

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(80, 60, 80, 54);
    rootLayout->setSpacing(0);

    rootLayout->addStretch(1);

    auto *contentCard = new QFrame(this);
    contentCard->setObjectName("startPageCard");
    contentCard->setMaximumWidth(680);
    auto *shadow = new QGraphicsDropShadowEffect(contentCard);
    shadow->setBlurRadius(36);
    shadow->setOffset(0, 12);
    shadow->setColor(QColor(0, 0, 0, 100));
    contentCard->setGraphicsEffect(shadow);

    auto *cardLayout = new QVBoxLayout(contentCard);
    cardLayout->setContentsMargins(54, 56, 54, 42);
    cardLayout->setSpacing(18);

    auto *titleLabel = new QLabel(QStringLiteral("Synera"), contentCard);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("startTitle");

    auto *subtitleLabel = new QLabel(QStringLiteral("Synergy Auto-Arena"), contentCard);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setObjectName("startSubtitle");

    startButton = new QPushButton(QStringLiteral("开始游戏"), contentCard);
    startButton->setObjectName("startButton");
    startButton->setCursor(Qt::PointingHandCursor);
    startButton->setMinimumSize(220, 64);

    exitButton = new QPushButton(QStringLiteral("退出游戏"), contentCard);
    exitButton->setObjectName("secondaryButton");
    exitButton->setCursor(Qt::PointingHandCursor);
    exitButton->setMinimumSize(220, 64);

    auto *buttonColumn = new QVBoxLayout;
    buttonColumn->setSpacing(14);
    buttonColumn->addWidget(startButton, 0, Qt::AlignCenter);
    buttonColumn->addWidget(exitButton, 0, Qt::AlignCenter);

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addSpacing(14);
    cardLayout->addLayout(buttonColumn);

    auto *contentRow = new QHBoxLayout;
    contentRow->setContentsMargins(0, 0, 0, 0);
    contentRow->addStretch(1);
    contentRow->addWidget(contentCard);
    contentRow->addStretch(1);
    rootLayout->addLayout(contentRow);

    rootLayout->addStretch(1);

    auto *sloganLabel = new QLabel(QStringLiteral("Build. Battle. Adapt."), this);
    sloganLabel->setAlignment(Qt::AlignCenter);
    sloganLabel->setObjectName("startSlogan");
    rootLayout->addWidget(sloganLabel);

    setStyleSheet(R"(
        StartPage {
            background-color: #090d15;
        }
        #startPageCard {
            background-color: rgba(10, 16, 28, 205);
            border: 2px solid rgba(176, 144, 85, 190);
            border-radius: 28px;
        }
        #startTitle {
            color: #d9c58b;
            font-size: 54px;
            font-weight: 800;
            letter-spacing: 2px;
        }
        #startSubtitle {
            color: #aab4c7;
            font-size: 18px;
            font-weight: 500;
            letter-spacing: 1px;
        }
        #startButton, #secondaryButton {
            color: #ffffff;
            border-radius: 22px;
            font-size: 22px;
            font-weight: 700;
            padding: 14px 28px;
        }
        #startButton {
            background-color: #43548d;
            border: 2px solid #8da6eb;
        }
        #startButton:hover {
            background-color: #5367ab;
            border-color: #d8c083;
        }
        #startButton:pressed {
            background-color: #31416d;
        }
        #secondaryButton {
            background-color: #28344f;
            border: 2px solid #7082ab;
        }
        #secondaryButton:hover {
            background-color: #364566;
            border-color: #d8c083;
        }
        #startSlogan {
            color: #8391a8;
            font-size: 15px;
            font-weight: 600;
            letter-spacing: 2px;
        }
    )");

    connect(startButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "Start game clicked";
        emit startClicked();
    });
    connect(exitButton, &QPushButton::clicked, this, &StartPage::exitClicked);
}

void StartPage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(9, 13, 21));

    QLinearGradient glow(0, 0, width(), height());
    glow.setColorAt(0.0, QColor(22, 34, 58, 120));
    glow.setColorAt(0.5, QColor(8, 13, 26, 0));
    glow.setColorAt(1.0, QColor(52, 34, 70, 85));
    painter.fillRect(rect(), glow);

    painter.setPen(QPen(QColor(121, 138, 176, 22), 1));
    const int gridSize = 48;
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}
