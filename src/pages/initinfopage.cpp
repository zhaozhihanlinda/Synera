#include "pages/initinfopage.h"

#include "core/playeravatar.h"
#include "core/uiscale.h"

#include <QDebug>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QHBoxLayout>
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
    layout->setContentsMargins(UiScale::margins(18, 16, 18, 16));
    layout->setSpacing(UiScale::scaled(6));

    auto *label = new QLabel(labelText, tile);
    label->setObjectName("tileLabel");

    valueLabel->setParent(tile);
    valueLabel->setObjectName("tileValue");

    layout->addWidget(label);
    layout->addWidget(valueLabel);
    return tile;
}

}

InitInfoPage::InitInfoPage(QWidget *parent)
    : QWidget(parent)
    , avatarValueLabel(new QLabel(QStringLiteral("Avatar 01"), this))
    , nicknameValueLabel(new QLabel(QStringLiteral("未命名指挥官"), this))
    , hpValueLabel(new QLabel(QStringLiteral("100"), this))
    , goldValueLabel(new QLabel(QStringLiteral("30"), this))
    , populationValueLabel(new QLabel(QStringLiteral("3"), this))
    , enterGameButton(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(UiScale::size(1280, 820));

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(UiScale::margins(80, 48, 80, 48));
    rootLayout->setSpacing(UiScale::scaled(24));
    rootLayout->addStretch(1);

    auto *card = new QFrame(this);
    card->setObjectName("initCard");
    card->setMaximumWidth(UiScale::width(860));
    auto *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(UiScale::scaled(36));
    shadow->setOffset(0, UiScale::scaled(14));
    shadow->setColor(QColor(0, 0, 0, 100));
    card->setGraphicsEffect(shadow);

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(UiScale::margins(44, 42, 44, 36));
    cardLayout->setSpacing(UiScale::scaled(18));

    auto *titleLabel = new QLabel(QStringLiteral("Prepare for Battle"), card);
    titleLabel->setObjectName("pageTitle");
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *profilePanel = new QFrame(card);
    profilePanel->setObjectName("profilePanel");
    auto *profileLayout = new QHBoxLayout(profilePanel);
    profileLayout->setContentsMargins(UiScale::margins(24, 24, 24, 24));
    profileLayout->setSpacing(UiScale::scaled(24));

    avatarValueLabel->setObjectName("avatarBadge");
    avatarValueLabel->setAlignment(Qt::AlignCenter);
    avatarValueLabel->setFixedSize(UiScale::size(180, 180));

    auto *identityLayout = new QVBoxLayout;
    identityLayout->setSpacing(UiScale::scaled(10));

    auto *nicknameTitle = new QLabel(QStringLiteral("玩家昵称"), profilePanel);
    nicknameTitle->setObjectName("sectionLabel");
    nicknameValueLabel->setObjectName("nicknameValue");

    identityLayout->addStretch(1);
    identityLayout->addWidget(nicknameTitle);
    identityLayout->addWidget(nicknameValueLabel);
    identityLayout->addStretch(1);

    profileLayout->addWidget(avatarValueLabel, 0, Qt::AlignTop);
    profileLayout->addLayout(identityLayout, 1);

    auto *statsGrid = new QGridLayout;
    statsGrid->setHorizontalSpacing(UiScale::scaled(16));
    statsGrid->setVerticalSpacing(UiScale::scaled(16));
    statsGrid->addWidget(createInfoTile(QStringLiteral("初始血量"), hpValueLabel, card), 0, 0);
    statsGrid->addWidget(createInfoTile(QStringLiteral("初始金币数"), goldValueLabel, card), 0, 1);
    statsGrid->addWidget(createInfoTile(QStringLiteral("初始人口上限"), populationValueLabel, card), 1, 0, 1, 2);

    enterGameButton = new QPushButton(QStringLiteral("进入游戏"), card);
    enterGameButton->setObjectName("primaryButton");
    enterGameButton->setCursor(Qt::PointingHandCursor);
    enterGameButton->setMinimumHeight(UiScale::height(58));

    cardLayout->addWidget(titleLabel);
    cardLayout->addSpacing(UiScale::scaled(12));
    cardLayout->addWidget(profilePanel);
    cardLayout->addLayout(statsGrid);
    cardLayout->addSpacing(UiScale::scaled(6));
    cardLayout->addWidget(enterGameButton);

    auto *cardRow = new QHBoxLayout;
    cardRow->addStretch(1);
    cardRow->addWidget(card);
    cardRow->addStretch(1);
    rootLayout->addLayout(cardRow);
    rootLayout->addStretch(1);

    setStyleSheet(UiScale::scaleStyleSheet(QStringLiteral(R"(
        InitInfoPage { background-color: #090d15; }
        #initCard {
            background-color: rgba(10, 16, 28, 218);
            border: 2px solid rgba(178, 151, 98, 190);
            border-radius: 30px;
        }
        #pageTitle {
            color: #e2cb92;
            font-size: 40px;
            font-weight: 800;
            letter-spacing: 2px;
        }
        #profilePanel {
            background-color: rgba(24, 34, 56, 180);
            border: 1px solid rgba(119, 145, 191, 140);
            border-radius: 24px;
        }
        #avatarBadge {
            color: #f6edda;
            background-color: #304067;
            border: 3px solid #d7bc7f;
            border-radius: 90px;
            font-size: 28px;
            font-weight: 800;
        }
        #sectionLabel {
            color: #8fa4c7;
            font-size: 14px;
            font-weight: 700;
            letter-spacing: 1px;
        }
        #nicknameValue {
            color: #f6e6be;
            font-size: 28px;
            font-weight: 800;
        }
        #bodyText {
            color: #dce6f7;
            font-size: 20px;
            font-weight: 600;
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

    connect(enterGameButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "Enter game clicked";
        emit enterGameClicked();
    });
}

void InitInfoPage::setGameInfo(const PlayerProfile &profile,
                               int hp,
                               int gold,
                               int populationLimit)
{
    avatarValueLabel->setText(playerAvatarSymbolForId(profile.avatarId));
    avatarValueLabel->setStyleSheet(playerAvatarStyleForId(profile.avatarId, 90, 52));
    nicknameValueLabel->setText(profile.nickname);
    hpValueLabel->setText(QString::number(hp));
    goldValueLabel->setText(QString::number(gold));
    populationValueLabel->setText(QString::number(populationLimit));
}

void InitInfoPage::paintEvent(QPaintEvent *event)
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
    const int gridSize = UiScale::scaled(48);
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}
