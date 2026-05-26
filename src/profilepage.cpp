#include "profilepage.h"

#include <QDebug>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QRandomGenerator>
#include <QStringList>
#include <QVBoxLayout>

namespace {

const QStringList kNicknames = {
    QStringLiteral("星火旅人"),
    QStringLiteral("银月指挥官"),
    QStringLiteral("暮光棋手"),
    QStringLiteral("钢铁先知"),
    QStringLiteral("棋盘术士"),
    QStringLiteral("烈焰统帅"),
    QStringLiteral("晨曦守卫"),
    QStringLiteral("影刃领主")
};

}

ProfilePage::ProfilePage(QWidget *parent)
    : QWidget(parent)
    , avatarLabel(nullptr)
    , nicknameLabel(nullptr)
    , rerollButton(nullptr)
    , confirmButton(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(1280, 820);

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(60, 50, 60, 50);
    rootLayout->setSpacing(0);
    rootLayout->addStretch(1);

    auto *card = new QFrame(this);
    card->setObjectName("profileCard");
    card->setMaximumWidth(720);
    auto *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(34);
    shadow->setOffset(0, 12);
    shadow->setColor(QColor(0, 0, 0, 110));
    card->setGraphicsEffect(shadow);

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(56, 48, 56, 42);
    cardLayout->setSpacing(18);

    auto *titleLabel = new QLabel(QStringLiteral("指挥官身份生成"), card);
    titleLabel->setObjectName("profileTitle");
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *subtitleLabel = new QLabel(QStringLiteral("Your commander is ready."), card);
    subtitleLabel->setObjectName("profileSubtitle");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    avatarLabel = new QLabel(card);
    avatarLabel->setObjectName("avatarLabel");
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setFixedSize(180, 180);

    nicknameLabel = new QLabel(card);
    nicknameLabel->setObjectName("nicknameLabel");
    nicknameLabel->setAlignment(Qt::AlignCenter);

    rerollButton = new QPushButton(QStringLiteral("重新随机"), card);
    rerollButton->setObjectName("secondaryButton");
    rerollButton->setCursor(Qt::PointingHandCursor);
    rerollButton->setMinimumHeight(56);

    confirmButton = new QPushButton(QStringLiteral("确认身份"), card);
    confirmButton->setObjectName("primaryButton");
    confirmButton->setCursor(Qt::PointingHandCursor);
    confirmButton->setMinimumHeight(56);

    auto *buttonRow = new QHBoxLayout;
    buttonRow->setContentsMargins(0, 10, 0, 0);
    buttonRow->setSpacing(16);
    buttonRow->addWidget(rerollButton);
    buttonRow->addWidget(confirmButton);

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(avatarLabel, 0, Qt::AlignCenter);
    cardLayout->addWidget(nicknameLabel);
    cardLayout->addLayout(buttonRow);

    auto *cardRow = new QHBoxLayout;
    cardRow->setContentsMargins(0, 0, 0, 0);
    cardRow->addStretch(1);
    cardRow->addWidget(card);
    cardRow->addStretch(1);

    rootLayout->addLayout(cardRow);
    rootLayout->addStretch(1);

    setStyleSheet(R"(
        ProfilePage {
            background-color: #090d15;
        }
        #profileCard {
            background-color: rgba(12, 18, 31, 215);
            border: 2px solid rgba(170, 140, 84, 185);
            border-radius: 30px;
        }
        #profileTitle {
            color: #e5d3a0;
            font-size: 34px;
            font-weight: 800;
        }
        #profileSubtitle {
            color: #98a7bc;
            font-size: 16px;
            font-weight: 500;
        }
        #avatarLabel {
            color: #f6edda;
            background-color: #2f3e68;
            border: 3px solid #d3b97b;
            border-radius: 90px;
            font-size: 28px;
            font-weight: 800;
        }
        #nicknameLabel {
            color: #f5e7c7;
            font-size: 24px;
            font-weight: 700;
        }
        #secondaryButton, #primaryButton {
            color: #ffffff;
            border-radius: 18px;
            font-size: 18px;
            font-weight: 700;
            padding: 12px 24px;
        }
        #secondaryButton {
            background-color: #28344f;
            border: 2px solid #7082ab;
        }
        #secondaryButton:hover {
            background-color: #354463;
        }
        #primaryButton {
            background-color: #7c5a2b;
            border: 2px solid #d8c182;
        }
        #primaryButton:hover {
            background-color: #96703a;
        }
    )");

    randomizeProfile();

    connect(rerollButton, &QPushButton::clicked, this, &ProfilePage::randomizeProfile);
    connect(confirmButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "Profile confirmed";
        emit profileConfirmed();
    });
}

void ProfilePage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(9, 13, 21));

    QRadialGradient pulse(width() * 0.5, height() * 0.35, width() * 0.45);
    pulse.setColorAt(0.0, QColor(58, 73, 117, 85));
    pulse.setColorAt(0.55, QColor(28, 35, 57, 40));
    pulse.setColorAt(1.0, QColor(9, 13, 21, 0));
    painter.fillRect(rect(), pulse);

    painter.setPen(QPen(QColor(132, 148, 184, 20), 1));
    const int gridSize = 48;
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}

void ProfilePage::randomizeProfile()
{
    currentAvatarLabel = randomAvatarLabel();
    currentNickname = randomNickname();
    refreshProfile();
}

QString ProfilePage::randomAvatarLabel() const
{
    const int avatarNumber = QRandomGenerator::global()->bounded(1, 9);
    return QStringLiteral("Avatar %1").arg(avatarNumber, 2, 10, QChar('0'));
}

QString ProfilePage::randomNickname() const
{
    const int index = QRandomGenerator::global()->bounded(kNicknames.size());
    return kNicknames[index];
}

void ProfilePage::refreshProfile()
{
    avatarLabel->setText(currentAvatarLabel);
    nicknameLabel->setText(QStringLiteral("昵称：%1").arg(currentNickname));
}
