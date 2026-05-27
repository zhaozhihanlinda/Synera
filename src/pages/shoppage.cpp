#include "pages/shoppage.h"

#include "core/playertemplatelibrary.h"

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

QFrame *createShopCard(const UnitTemplate &unitTemplate, QWidget *parent)
{
    auto *card = new QFrame(parent);
    card->setObjectName("shopCard");

    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(8);

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
    , hpValueLabel(new QLabel(QStringLiteral("100"), this))
    , goldValueLabel(new QLabel(QStringLiteral("30"), this))
    , populationValueLabel(new QLabel(QStringLiteral("0 / 3"), this))
    , enterDeployButton(nullptr)
    , shopCardsLayout(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(1280, 820);

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(72, 42, 72, 42);
    rootLayout->setSpacing(20);

    auto *titleLabel = new QLabel(QStringLiteral("商店"), this);
    titleLabel->setObjectName("pageTitle");
    auto *subtitleLabel = new QLabel(QStringLiteral("Placeholder shop before deployment."), this);
    subtitleLabel->setObjectName("pageSubtitle");

    auto *statsGrid = new QGridLayout;
    statsGrid->setHorizontalSpacing(16);
    statsGrid->setVerticalSpacing(16);
    statsGrid->addWidget(createInfoTile(QStringLiteral("当前回合"), roundValueLabel, this), 0, 0);
    statsGrid->addWidget(createInfoTile(QStringLiteral("玩家金币"), goldValueLabel, this), 0, 1);
    statsGrid->addWidget(createInfoTile(QStringLiteral("玩家血量"), hpValueLabel, this), 1, 0);
    statsGrid->addWidget(createInfoTile(QStringLiteral("玩家人口"), populationValueLabel, this), 1, 1);

    auto *shopPanel = new QFrame(this);
    shopPanel->setObjectName("shopPanel");
    auto *shopPanelLayout = new QVBoxLayout(shopPanel);
    shopPanelLayout->setContentsMargins(36, 32, 36, 32);
    shopPanelLayout->setSpacing(18);

    auto *shopTitle = new QLabel(QStringLiteral("初始角色池"), shopPanel);
    shopTitle->setObjectName("panelTitle");
    auto *shopBody = new QLabel(QStringLiteral("当前阶段只展示我方角色模板，不实现购买与刷新。"), shopPanel);
    shopBody->setObjectName("panelBody");
    shopBody->setWordWrap(true);

    shopCardsLayout = new QGridLayout;
    shopCardsLayout->setHorizontalSpacing(14);
    shopCardsLayout->setVerticalSpacing(14);
    populateShopCards();

    enterDeployButton = new QPushButton(QStringLiteral("进入部署"), shopPanel);
    enterDeployButton->setObjectName("primaryButton");
    enterDeployButton->setCursor(Qt::PointingHandCursor);
    enterDeployButton->setMinimumHeight(56);

    shopPanelLayout->addWidget(shopTitle);
    shopPanelLayout->addWidget(shopBody);
    shopPanelLayout->addLayout(shopCardsLayout);
    shopPanelLayout->addSpacing(8);
    shopPanelLayout->addWidget(enterDeployButton, 0, Qt::AlignLeft);

    rootLayout->addWidget(titleLabel);
    rootLayout->addWidget(subtitleLabel);
    rootLayout->addLayout(statsGrid);
    rootLayout->addWidget(shopPanel, 1);

    setStyleSheet(R"(
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
        #shopCard {
            background-color: rgba(16, 26, 40, 210);
            border: 2px solid rgba(111, 212, 222, 150);
            border-radius: 18px;
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
    )");

    connect(enterDeployButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "Enter deploy clicked";
        emit enterDeployClicked();
    });
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

void ShopPage::setGameInfo(int round, int gold, int hp, int currentPopulation, int maxPopulation)
{
    roundValueLabel->setText(QString::number(round));
    goldValueLabel->setText(QString::number(gold));
    hpValueLabel->setText(QString::number(hp));
    populationValueLabel->setText(QStringLiteral("%1 / %2").arg(currentPopulation).arg(maxPopulation));
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
    const int gridSize = 48;
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}
