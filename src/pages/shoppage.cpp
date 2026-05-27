#include "pages/shoppage.h"

#include "core/uiscale.h"
#include "core/playertemplatelibrary.h"

#include <QDebug>
#include <QFrame>
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

QFrame *createShopCard(const UnitTemplate &unitTemplate, QWidget *parent)
{
    auto *card = new QFrame(parent);
    card->setObjectName("shopCard");

    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(UiScale::margins(18, 18, 18, 18));
    layout->setSpacing(UiScale::scaled(8));

    auto *nameLabel = new QLabel(unitTemplate.displayName, card);
    nameLabel->setObjectName("cardTitle");
    auto *costLabel = new QLabel(QStringLiteral("%1 金币").arg(unitTemplate.cost), card);
    costLabel->setObjectName("cardCost");
    auto *skillLabel = new QLabel(
        unitTemplate.skillName.isEmpty()
            ? QStringLiteral("技能：-")
            : QStringLiteral("技能：%1").arg(unitTemplate.skillName),
        card);
    skillLabel->setObjectName("cardSkill");
    auto *roleLabel = new QLabel(unitTemplate.roleDescription, card);
    roleLabel->setObjectName("cardBody");
    roleLabel->setWordWrap(true);

    layout->addWidget(nameLabel);
    layout->addWidget(costLabel);
    layout->addWidget(skillLabel);
    layout->addWidget(roleLabel);
    layout->addStretch(1);
    return card;
}

}

ShopPage::ShopPage(QWidget *parent)
    : QWidget(parent)
    , roundValueLabel(new QLabel(QStringLiteral("1"), this))
    , goldValueLabel(new QLabel(QStringLiteral("30"), this))
    , populationValueLabel(new QLabel(QStringLiteral("0 / 3"), this))
    , enterDeployButton(nullptr)
    , ownedUnitsLayout(nullptr)
    , shopCardsLayout(nullptr)
    , unitDetailOverlay(nullptr)
    , unitDetailTitleLabel(nullptr)
    , unitDetailBodyLabel(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(UiScale::size(1280, 820));

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(UiScale::margins(72, 42, 72, 42));
    rootLayout->setSpacing(UiScale::scaled(20));

    auto *titleLabel = new QLabel(QStringLiteral("商店"), this);
    titleLabel->setObjectName("pageTitle");
    auto *subtitleLabel = new QLabel(QStringLiteral("Placeholder shop before deployment."), this);
    subtitleLabel->setObjectName("pageSubtitle");

    auto *statsRow = new QHBoxLayout;
    statsRow->setSpacing(UiScale::scaled(16));
    statsRow->addWidget(createInfoTile(QStringLiteral("玩家金币"), goldValueLabel, this), 1);
    statsRow->addWidget(createInfoTile(QStringLiteral("玩家人口"), populationValueLabel, this), 1);
    statsRow->addWidget(createInfoTile(QStringLiteral("当前回合"), roundValueLabel, this), 1);

    auto *shopPanel = new QFrame(this);
    shopPanel->setObjectName("shopPanel");
    auto *shopPanelLayout = new QVBoxLayout(shopPanel);
    shopPanelLayout->setContentsMargins(UiScale::margins(36, 32, 36, 32));
    shopPanelLayout->setSpacing(UiScale::scaled(18));

    auto *ownedTitle = new QLabel(QStringLiteral("已拥有角色"), shopPanel);
    ownedTitle->setObjectName("panelTitle");

    ownedUnitsLayout = new QGridLayout;
    ownedUnitsLayout->setHorizontalSpacing(UiScale::scaled(12));
    ownedUnitsLayout->setVerticalSpacing(UiScale::scaled(12));

    auto *shopTitle = new QLabel(QStringLiteral("可购买角色"), shopPanel);
    shopTitle->setObjectName("panelTitle");

    shopCardsLayout = new QGridLayout;
    shopCardsLayout->setHorizontalSpacing(UiScale::scaled(14));
    shopCardsLayout->setVerticalSpacing(UiScale::scaled(14));
    populateShopCards();

    enterDeployButton = new QPushButton(QStringLiteral("进入部署"), shopPanel);
    enterDeployButton->setObjectName("primaryButton");
    enterDeployButton->setCursor(Qt::PointingHandCursor);
    enterDeployButton->setMinimumHeight(UiScale::height(56));

    unitDetailOverlay = new QFrame(this);
    unitDetailOverlay->setObjectName("unitDetailOverlay");
    unitDetailOverlay->hide();
    auto *overlayLayout = new QVBoxLayout(unitDetailOverlay);
    overlayLayout->setContentsMargins(UiScale::margins(0, 0, 0, 0));
    auto *overlayCenter = new QHBoxLayout;
    overlayCenter->addStretch(1);

    auto *detailCard = new QFrame(unitDetailOverlay);
    detailCard->setObjectName("unitDetailCard");
    detailCard->setMaximumWidth(UiScale::width(620));
    auto *detailLayout = new QVBoxLayout(detailCard);
    detailLayout->setContentsMargins(UiScale::margins(30, 28, 30, 24));
    detailLayout->setSpacing(UiScale::scaled(14));
    unitDetailTitleLabel = new QLabel(QStringLiteral("角色信息"), detailCard);
    unitDetailTitleLabel->setObjectName("panelTitle");
    unitDetailBodyLabel = new QLabel(detailCard);
    unitDetailBodyLabel->setObjectName("panelBody");
    unitDetailBodyLabel->setWordWrap(true);
    auto *closeDetailButton = new QPushButton(QStringLiteral("关闭"), detailCard);
    closeDetailButton->setObjectName("primaryButton");
    closeDetailButton->setCursor(Qt::PointingHandCursor);
    closeDetailButton->setMinimumHeight(UiScale::height(52));
    detailLayout->addWidget(unitDetailTitleLabel);
    detailLayout->addWidget(unitDetailBodyLabel);
    detailLayout->addWidget(closeDetailButton, 0, Qt::AlignRight);

    overlayCenter->addWidget(detailCard);
    overlayCenter->addStretch(1);
    overlayLayout->addStretch(1);
    overlayLayout->addLayout(overlayCenter);
    overlayLayout->addStretch(1);

    shopPanelLayout->addWidget(ownedTitle);
    shopPanelLayout->addLayout(ownedUnitsLayout);
    shopPanelLayout->addSpacing(UiScale::scaled(10));
    shopPanelLayout->addWidget(shopTitle);
    shopPanelLayout->addLayout(shopCardsLayout);
    shopPanelLayout->addSpacing(UiScale::scaled(8));
    shopPanelLayout->addWidget(enterDeployButton, 0, Qt::AlignLeft);

    rootLayout->addWidget(titleLabel);
    rootLayout->addWidget(subtitleLabel);
    rootLayout->addLayout(statsRow);
    rootLayout->addWidget(shopPanel, 1);

    setStyleSheet(UiScale::scaleStyleSheet(QStringLiteral(R"(
        ShopPage { background-color: #090d15; }
        #pageTitle {
            color: #e3cd92;
            font-size: 38px;
            font-weight: 800;
        }
        #pageSubtitle {
            color: #99a8bd;
            font-size: 16px;
            font-weight: 500;
        }
        #infoTile, #shopPanel {
            background-color: rgba(10, 16, 28, 215);
            border: 2px solid rgba(174, 150, 98, 180);
            border-radius: 24px;
        }
        #unitDetailOverlay {
            background-color: rgba(4, 7, 13, 165);
            border-radius: 24px;
        }
        #unitDetailCard {
            background-color: rgba(12, 18, 31, 238);
            border: 2px solid rgba(178, 151, 98, 205);
            border-radius: 24px;
        }
        #shopCard {
            background-color: rgba(16, 26, 40, 210);
            border: 2px solid rgba(111, 212, 222, 150);
            border-radius: 18px;
        }
        #ownedUnitButton {
            color: #f5e4bd;
            background-color: rgba(16, 26, 40, 220);
            border: 2px solid rgba(111, 212, 222, 140);
            border-radius: 16px;
            font-size: 18px;
            font-weight: 800;
            padding: 12px 18px;
        }
        #ownedUnitButton:hover {
            background-color: rgba(28, 48, 67, 230);
            border-color: #dbc183;
        }
        #tileLabel {
            color: #91a5c7;
            font-size: 13px;
            font-weight: 700;
        }
        #tileValue {
            color: #f2f5fa;
            font-size: 22px;
            font-weight: 800;
        }
        #panelTitle {
            color: #f5e4bd;
            font-size: 30px;
            font-weight: 800;
        }
        #panelBody {
            color: #d5deed;
            font-size: 18px;
            font-weight: 500;
        }
        #hintText {
            color: #9cb3cf;
            font-size: 14px;
            font-weight: 600;
        }
        #cardTitle {
            color: #f5e4bd;
            font-size: 20px;
            font-weight: 800;
        }
        #cardCost {
            color: #77e6b0;
            font-size: 16px;
            font-weight: 700;
        }
        #cardSkill {
            color: #9cc8ff;
            font-size: 14px;
            font-weight: 700;
        }
        #cardBody {
            color: #d5deed;
            font-size: 15px;
            font-weight: 500;
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
    )")));

    connect(enterDeployButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "Enter deploy clicked";
        emit enterDeployClicked();
    });
    connect(closeDetailButton, &QPushButton::clicked, unitDetailOverlay, &QWidget::hide);
}

void ShopPage::populateShopCards()
{
    if (!shopCardsLayout) {
        return;
    }

    const QVector<UnitTemplate> shopTemplates = defaultShopTemplates();
    for (int index = 0; index < shopTemplates.size(); ++index) {
        shopCardsLayout->addWidget(createShopCard(shopTemplates.at(index), this), 0, index);
    }
}

void ShopPage::setGameInfo(int round,
                           int gold,
                           int currentPopulation,
                           int maxPopulation,
                           const QVector<UnitPtr> &units)
{
    roundValueLabel->setText(QString::number(round));
    goldValueLabel->setText(QString::number(gold));
    populationValueLabel->setText(QStringLiteral("%1 / %2").arg(currentPopulation).arg(maxPopulation));
    ownedUnits = units;
    refreshOwnedUnits();
}

void ShopPage::refreshOwnedUnits()
{
    if (!ownedUnitsLayout) {
        return;
    }

    QLayoutItem *child = nullptr;
    while ((child = ownedUnitsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    for (int index = 0; index < ownedUnits.size(); ++index) {
        const UnitPtr unit = ownedUnits.at(index);
        auto *unitButton = new QPushButton(unit ? unit->name() : QStringLiteral("-"), this);
        unitButton->setObjectName("ownedUnitButton");
        unitButton->setCursor(Qt::PointingHandCursor);
        unitButton->setMinimumHeight(UiScale::height(52));
        ownedUnitsLayout->addWidget(unitButton, index / 4, index % 4);
        connect(unitButton, &QPushButton::clicked, this, [this, unit]() {
            showOwnedUnitDetail(unit);
        });
    }
}

void ShopPage::showOwnedUnitDetail(const UnitPtr &unit)
{
    if (!unit || !unitDetailOverlay || !unitDetailTitleLabel || !unitDetailBodyLabel) {
        return;
    }

    unitDetailTitleLabel->setText(unit->name());
    unitDetailBodyLabel->setText(
        QStringLiteral("Cost: %1\nHP: %2 / %3\nMana: %4 / %5\nATK: %6\nRange: %7\nAS: %8\nMana Gain: %9 / %10\nSkill: %11\nSkill Detail: %12\nRole: %13")
            .arg(unit->cost())
            .arg(unit->hp())
            .arg(unit->maxHp())
            .arg(unit->mana())
            .arg(unit->maxMana())
            .arg(unit->atk())
            .arg(unit->range())
            .arg(unit->attackSpeed(), 0, 'f', 1)
            .arg(unit->manaGainOnAttack())
            .arg(unit->manaGainOnHit())
            .arg(unit->skillName().isEmpty() ? QStringLiteral("-") : unit->skillName())
            .arg(unit->skillDescription().isEmpty() ? QStringLiteral("-") : unit->skillDescription())
            .arg(unit->roleDescription().isEmpty() ? QStringLiteral("-") : unit->roleDescription()));
    unitDetailOverlay->setGeometry(rect());
    unitDetailOverlay->show();
    unitDetailOverlay->raise();
}

void ShopPage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(9, 13, 21));

    QLinearGradient glow(0, 0, width(), height());
    glow.setColorAt(0.0, QColor(17, 30, 56, 120));
    glow.setColorAt(0.55, QColor(8, 13, 23, 0));
    glow.setColorAt(1.0, QColor(52, 32, 70, 90));
    painter.fillRect(rect(), glow);

    painter.setPen(QPen(QColor(123, 141, 181, 22), 1));
    const int gridSize = UiScale::scaled(48);
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}
