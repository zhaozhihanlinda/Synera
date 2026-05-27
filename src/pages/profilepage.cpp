#include "pages/profilepage.h"

#include "core/uiscale.h"

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
    setMinimumSize(UiScale::size(1280, 820));

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(UiScale::margins(60, 50, 60, 50));
    rootLayout->setSpacing(0);
    rootLayout->addStretch(1);

    auto *card = new QFrame(this);
    card->setObjectName("profileCard");
    card->setMaximumWidth(UiScale::width(720));
    auto *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(UiScale::scaled(34));
    shadow->setOffset(0, UiScale::scaled(12));
    shadow->setColor(QColor(0, 0, 0, 110));
    card->setGraphicsEffect(shadow);

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(UiScale::margins(56, 48, 56, 42));
    cardLayout->setSpacing(UiScale::scaled(18));

    auto *titleLabel = new QLabel(QStringLiteral("PREPARE FOR BATTLE"), card);
    titleLabel->setObjectName("profileTitle");
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *subtitleLabel = new QLabel(QStringLiteral("COMMANDER"), card);
    subtitleLabel->setObjectName("profileSubtitle");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    avatarLabel = new QLabel(card);
    avatarLabel->setObjectName("avatarLabel");
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setFixedSize(UiScale::size(108, 108));
    avatarLabel->setText(QStringLiteral("✦"));

    nicknameLabel = new QLabel(card);
    nicknameLabel->setObjectName("nicknameLabel");
    nicknameLabel->setAlignment(Qt::AlignCenter);

    rerollButton = new QPushButton(QStringLiteral("重新随机"), card);
    rerollButton->setObjectName("secondaryButton");
    rerollButton->setCursor(Qt::PointingHandCursor);
    rerollButton->setMinimumHeight(UiScale::height(56));

    confirmButton = new QPushButton(QStringLiteral("确认身份"), card);
    confirmButton->setObjectName("primaryButton");
    confirmButton->setCursor(Qt::PointingHandCursor);
    confirmButton->setMinimumHeight(UiScale::height(56));

    auto *buttonRow = new QHBoxLayout;
    buttonRow->setContentsMargins(UiScale::margins(0, 10, 0, 0));
    buttonRow->setSpacing(UiScale::scaled(16));
    buttonRow->addWidget(rerollButton);
    buttonRow->addWidget(confirmButton);

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addSpacing(UiScale::scaled(10));
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

    setStyleSheet(UiScale::scaleStyleSheet(QStringLiteral(R"(
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
            font-size: 40px;
            font-weight: 800;
            letter-spacing: 2px;
        }
        #profileSubtitle {
            color: #8fa4c7;
            font-size: 14px;
            font-weight: 800;
            letter-spacing: 3px;
        }
        #avatarLabel {
            color: #f6e8c3;
            background-color: qradialgradient(
                cx: 0.5, cy: 0.45, radius: 0.75,
                fx: 0.45, fy: 0.38,
                stop: 0 #45597f,
                stop: 0.5 #2c3a5b,
                stop: 1 #172133
            );
            border: 3px solid #d3b97b;
            border-radius: 54px;
            font-size: 34px;
            font-weight: 900;
            padding-bottom: 4px;
        }
        #avatarLabel:hover {
            border-color: #ecd8a0;
        }
        #nicknameLabel {
            color: #f5e7c7;
            font-size: 36px;
            font-weight: 800;
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
    )")));

    randomizeProfile();

    connect(rerollButton, &QPushButton::clicked, this, &ProfilePage::randomizeProfile);
    connect(confirmButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "Profile confirmed";
        emit confirmClicked(currentProfile);
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
    const int gridSize = UiScale::scaled(48);
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}

void ProfilePage::randomizeProfile()
{
    currentProfile.avatarId = randomAvatarLabel();
    currentProfile.nickname = randomNickname();
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
    avatarLabel->setText(avatarSymbolForId(currentProfile.avatarId));
    avatarLabel->setStyleSheet(avatarStyleForId(currentProfile.avatarId));
    nicknameLabel->setText(currentProfile.nickname);
}

QString ProfilePage::avatarSymbolForId(const QString &avatarId) const
{
    if (avatarId.endsWith(QStringLiteral("01"))) {
        return QStringLiteral("✦");
    }
    if (avatarId.endsWith(QStringLiteral("02"))) {
        return QStringLiteral("☽");
    }
    if (avatarId.endsWith(QStringLiteral("03"))) {
        return QStringLiteral("⚔");
    }
    if (avatarId.endsWith(QStringLiteral("04"))) {
        return QStringLiteral("♜");
    }
    if (avatarId.endsWith(QStringLiteral("05"))) {
        return QStringLiteral("✧");
    }
    if (avatarId.endsWith(QStringLiteral("06"))) {
        return QStringLiteral("❖");
    }
    if (avatarId.endsWith(QStringLiteral("07"))) {
        return QStringLiteral("✹");
    }
    return QStringLiteral("☼");
}

QString ProfilePage::avatarStyleForId(const QString &avatarId) const
{
    if (avatarId.endsWith(QStringLiteral("01"))) {
        return UiScale::scaleStyleSheet(QStringLiteral(
            "color: #f6e7c0;"
            "background-color: qradialgradient(cx:0.5, cy:0.42, radius:0.78, fx:0.46, fy:0.34, stop:0 #5f6990, stop:0.48 #30466b, stop:1 #162131);"
            "border: 3px solid #d9bf7a;"
            "border-radius: 54px;"
            "font-size: 34px;"
            "font-weight: 900;"
            "padding-bottom: 4px;"));
    }
    if (avatarId.endsWith(QStringLiteral("02"))) {
        return UiScale::scaleStyleSheet(QStringLiteral(
            "color: #d9edff;"
            "background-color: qradialgradient(cx:0.5, cy:0.4, radius:0.8, fx:0.42, fy:0.32, stop:0 #5277a0, stop:0.52 #263f67, stop:1 #121b2d);"
            "border: 3px solid #8ab6e5;"
            "border-radius: 54px;"
            "font-size: 34px;"
            "font-weight: 900;"
            "padding-bottom: 4px;"));
    }
    if (avatarId.endsWith(QStringLiteral("03"))) {
        return UiScale::scaleStyleSheet(QStringLiteral(
            "color: #f4d6b1;"
            "background-color: qradialgradient(cx:0.5, cy:0.45, radius:0.8, fx:0.48, fy:0.34, stop:0 #7c5960, stop:0.5 #472b3b, stop:1 #1b1421);"
            "border: 3px solid #d4a37b;"
            "border-radius: 54px;"
            "font-size: 30px;"
            "font-weight: 900;"
            "padding-bottom: 4px;"));
    }
    if (avatarId.endsWith(QStringLiteral("04"))) {
        return UiScale::scaleStyleSheet(QStringLiteral(
            "color: #e9e0cf;"
            "background-color: qradialgradient(cx:0.5, cy:0.43, radius:0.8, fx:0.45, fy:0.34, stop:0 #5b6774, stop:0.52 #33404f, stop:1 #151c25);"
            "border: 3px solid #bfc9d6;"
            "border-radius: 54px;"
            "font-size: 30px;"
            "font-weight: 900;"
            "padding-bottom: 4px;"));
    }
    if (avatarId.endsWith(QStringLiteral("05"))) {
        return UiScale::scaleStyleSheet(QStringLiteral(
            "color: #f7e4c9;"
            "background-color: qradialgradient(cx:0.5, cy:0.42, radius:0.8, fx:0.46, fy:0.34, stop:0 #71639f, stop:0.5 #3f3567, stop:1 #18142a);"
            "border: 3px solid #cfb0f1;"
            "border-radius: 54px;"
            "font-size: 32px;"
            "font-weight: 900;"
            "padding-bottom: 4px;"));
    }
    if (avatarId.endsWith(QStringLiteral("06"))) {
        return UiScale::scaleStyleSheet(QStringLiteral(
            "color: #d8fff0;"
            "background-color: qradialgradient(cx:0.5, cy:0.42, radius:0.8, fx:0.46, fy:0.34, stop:0 #4c7f78, stop:0.5 #27534d, stop:1 #122723);"
            "border: 3px solid #8fd8c4;"
            "border-radius: 54px;"
            "font-size: 30px;"
            "font-weight: 900;"
            "padding-bottom: 4px;"));
    }
    if (avatarId.endsWith(QStringLiteral("07"))) {
        return UiScale::scaleStyleSheet(QStringLiteral(
            "color: #ffe3c4;"
            "background-color: qradialgradient(cx:0.5, cy:0.42, radius:0.8, fx:0.46, fy:0.34, stop:0 #8a6f4b, stop:0.5 #5b4328, stop:1 #241a11);"
            "border: 3px solid #e2bc79;"
            "border-radius: 54px;"
            "font-size: 30px;"
            "font-weight: 900;"
            "padding-bottom: 4px;"));
    }
    return UiScale::scaleStyleSheet(QStringLiteral(
        "color: #ffe9b0;"
        "background-color: qradialgradient(cx:0.5, cy:0.42, radius:0.8, fx:0.46, fy:0.34, stop:0 #8c7c59, stop:0.5 #55452f, stop:1 #221a12);"
        "border: 3px solid #efcd83;"
        "border-radius: 54px;"
        "font-size: 30px;"
        "font-weight: 900;"
        "padding-bottom: 4px;"));
}
