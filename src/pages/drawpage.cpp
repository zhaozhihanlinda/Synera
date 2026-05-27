#include "pages/drawpage.h"

#include "core/uiscale.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QRandomGenerator>
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
    valueLabel->setWordWrap(true);

    layout->addWidget(label);
    layout->addWidget(valueLabel);
    return tile;
}

}

DrawPage::DrawPage(QWidget *parent)
    : QWidget(parent)
    , roundValueLabel(new QLabel(QStringLiteral("1"), this))
    , difficultyValueLabel(new QLabel(QStringLiteral("?"), this))
    , styleValueLabel(new QLabel(QStringLiteral("未翻牌"), this))
    , hintValueLabel(new QLabel(QStringLiteral("请从三张命运牌中单击翻开一张，以决定本轮敌军打法。"), this))
    , capacityHintLabel(new QLabel(QStringLiteral("当前总拥有单位上限：8"), this))
    , shopButton(nullptr)
    , deployButton(nullptr)
    , ownedUnitsButton(nullptr)
    , ownedUnitsCapacity(8)
    , hasRevealedCard(false)
    , ownedUnitsOverlay(nullptr)
    , ownedUnitsCard(nullptr)
    , ownedUnitsGridLayout(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(UiScale::size(1280, 820));

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(UiScale::margins(72, 42, 72, 42));
    rootLayout->setSpacing(UiScale::scaled(20));

    auto *titleLabel = new QLabel(QStringLiteral("敌军抽签"), this);
    titleLabel->setObjectName("pageTitle");
    auto *subtitleLabel = new QLabel(QStringLiteral("Reveal the tactical direction before you decide your route."), this);
    subtitleLabel->setObjectName("pageSubtitle");

    auto *statsRow = new QHBoxLayout;
    statsRow->setSpacing(UiScale::scaled(16));
    statsRow->addWidget(createInfoTile(QStringLiteral("当前回合"), roundValueLabel, this), 1);
    statsRow->addWidget(createInfoTile(QStringLiteral("难度等级"), difficultyValueLabel, this), 1);
    statsRow->addWidget(createInfoTile(QStringLiteral("抽到的打法"), styleValueLabel, this), 2);

    auto *hintPanel = new QFrame(this);
    hintPanel->setObjectName("hintPanel");
    auto *hintLayout = new QVBoxLayout(hintPanel);
    hintLayout->setContentsMargins(UiScale::margins(30, 28, 30, 28));
    hintLayout->setSpacing(UiScale::scaled(14));

    auto *hintTitle = new QLabel(QStringLiteral("打法提示"), hintPanel);
    hintTitle->setObjectName("panelTitle");
    hintValueLabel->setParent(hintPanel);
    hintValueLabel->setObjectName("panelBody");
    hintValueLabel->setWordWrap(true);

    auto *noteLabel = new QLabel(
        QStringLiteral("当前阶段只显示敌方难度与战术方向，不显示敌人具体单位和布局。"),
        hintPanel);
    noteLabel->setObjectName("hintText");
    noteLabel->setWordWrap(true);

    capacityHintLabel->setParent(hintPanel);
    capacityHintLabel->setObjectName("hintText");
    capacityHintLabel->setWordWrap(true);

    auto *cardsRow = new QHBoxLayout;
    cardsRow->setSpacing(UiScale::scaled(18));
    for (int index = 0; index < 3; ++index) {
        auto *cardButton = new QPushButton(QStringLiteral("命运牌\n%1").arg(index + 1), hintPanel);
        cardButton->setObjectName("drawCard");
        cardButton->setCursor(Qt::PointingHandCursor);
        cardButton->setMinimumSize(UiScale::size(180, 260));
        cardButton->setCheckable(true);
        cardsRow->addWidget(cardButton, 1);
        cardButtons.append(cardButton);
        connect(cardButton, &QPushButton::clicked, this, [this, index]() {
            revealCard(index);
        });
    }

    auto *buttonRow = new QHBoxLayout;
    buttonRow->setSpacing(UiScale::scaled(16));

    ownedUnitsButton = new QPushButton(QStringLiteral("查看已拥有角色"), hintPanel);
    shopButton = new QPushButton(QStringLiteral("进入商店"), hintPanel);
    deployButton = new QPushButton(QStringLiteral("直接部署"), hintPanel);
    ownedUnitsButton->setObjectName("secondaryButton");
    shopButton->setObjectName("secondaryButton");
    deployButton->setObjectName("primaryButton");
    ownedUnitsButton->setMinimumHeight(UiScale::height(56));
    shopButton->setMinimumHeight(UiScale::height(56));
    deployButton->setMinimumHeight(UiScale::height(56));
    ownedUnitsButton->setCursor(Qt::PointingHandCursor);
    shopButton->setCursor(Qt::PointingHandCursor);
    deployButton->setCursor(Qt::PointingHandCursor);

    buttonRow->addWidget(ownedUnitsButton);
    buttonRow->addWidget(shopButton);
    buttonRow->addWidget(deployButton);

    hintLayout->addWidget(hintTitle);
    hintLayout->addWidget(hintValueLabel);
    hintLayout->addWidget(noteLabel);
    hintLayout->addWidget(capacityHintLabel);
    hintLayout->addLayout(cardsRow);
    hintLayout->addSpacing(UiScale::scaled(8));
    hintLayout->addLayout(buttonRow);
    hintLayout->addStretch(1);

    rootLayout->addWidget(titleLabel);
    rootLayout->addWidget(subtitleLabel);
    rootLayout->addLayout(statsRow);
    rootLayout->addWidget(hintPanel, 1);

    ownedUnitsOverlay = new QFrame(this);
    ownedUnitsOverlay->setObjectName("ownedOverlay");
    ownedUnitsOverlay->hide();

    auto *overlayLayout = new QVBoxLayout(ownedUnitsOverlay);
    overlayLayout->setContentsMargins(UiScale::margins(0, 0, 0, 0));

    auto *overlayCenter = new QHBoxLayout;
    overlayCenter->addStretch(1);

    ownedUnitsCard = new QFrame(ownedUnitsOverlay);
    ownedUnitsCard->setObjectName("ownedCard");
    ownedUnitsCard->setMinimumSize(UiScale::size(1260, 920));
    ownedUnitsCard->setMaximumSize(UiScale::size(1260, 920));

    auto *ownedCardLayout = new QVBoxLayout(ownedUnitsCard);
    ownedCardLayout->setContentsMargins(UiScale::margins(28, 26, 28, 22));
    ownedCardLayout->setSpacing(UiScale::scaled(14));

    auto *ownedTitle = new QLabel(QStringLiteral("已拥有角色"), ownedUnitsCard);
    ownedTitle->setObjectName("panelTitle");
    auto *ownedSubtitle = new QLabel(QStringLiteral("当前总拥有单位不能超过 8 个。这里显示你目前已经拥有的角色及其静态属性。"), ownedUnitsCard);
    ownedSubtitle->setObjectName("hintText");
    ownedSubtitle->setWordWrap(true);

    auto *cardsContainer = new QWidget(ownedUnitsCard);
    ownedUnitsGridLayout = new QGridLayout(cardsContainer);
    ownedUnitsGridLayout->setContentsMargins(UiScale::margins(0, 0, 0, 0));
    ownedUnitsGridLayout->setHorizontalSpacing(UiScale::scaled(14));
    ownedUnitsGridLayout->setVerticalSpacing(UiScale::scaled(14));

    auto *closeOwnedButton = new QPushButton(QStringLiteral("关闭窗口"), ownedUnitsCard);
    closeOwnedButton->setObjectName("secondaryButton");
    closeOwnedButton->setCursor(Qt::PointingHandCursor);
    closeOwnedButton->setMinimumHeight(UiScale::height(52));

    ownedCardLayout->addWidget(ownedTitle);
    ownedCardLayout->addWidget(ownedSubtitle);
    ownedCardLayout->addWidget(cardsContainer, 1);
    ownedCardLayout->addWidget(closeOwnedButton, 0, Qt::AlignRight);

    overlayCenter->addWidget(ownedUnitsCard, 1);
    overlayCenter->addStretch(1);
    overlayLayout->addStretch(1);
    overlayLayout->addLayout(overlayCenter);
    overlayLayout->addStretch(1);

    setStyleSheet(UiScale::scaleStyleSheet(QStringLiteral(R"(
        DrawPage { background-color: #090d15; }
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
        #infoTile, #hintPanel {
            background-color: rgba(10, 16, 28, 215);
            border: 2px solid rgba(174, 150, 98, 180);
            border-radius: 24px;
        }
        #ownedOverlay {
            background-color: rgba(4, 7, 13, 165);
            border-radius: 24px;
        }
        #ownedCard {
            background-color: rgba(12, 18, 31, 236);
            border: 2px solid rgba(178, 151, 98, 205);
            border-radius: 24px;
        }
        #ownedUnitCard {
            background-color: rgba(16, 26, 40, 220);
            border: 2px solid rgba(111, 212, 222, 130);
            border-radius: 18px;
        }
        #ownedEmptyCard {
            background-color: rgba(12, 20, 32, 170);
            border: 2px dashed rgba(123, 141, 181, 110);
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
            font-size: 16px;
            font-weight: 500;
        }
        #hintText {
            color: #9cb3cf;
            font-size: 13px;
            font-weight: 600;
        }
        #cardTitle {
            color: #f5e4bd;
            font-size: 18px;
            font-weight: 800;
        }
        #drawCard {
            color: #f5e4bd;
            background-color: rgba(23, 34, 53, 228);
            border: 2px solid rgba(174, 150, 98, 180);
            border-radius: 22px;
            font-size: 22px;
            font-weight: 800;
            padding: 18px;
        }
        #drawCard:hover {
            background-color: rgba(35, 50, 77, 236);
            border-color: rgba(132, 191, 214, 220);
        }
        #drawCard:checked {
            background-color: rgba(52, 86, 116, 236);
            border-color: rgba(244, 215, 150, 230);
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
            background-color: #264d67;
            border: 2px solid #84bfd6;
        }
        #secondaryButton:hover {
            background-color: #34657f;
        }
    )")));

    connect(ownedUnitsButton, &QPushButton::clicked, this, [this]() {
        ownedUnitsOverlay->setGeometry(rect());
        ownedUnitsOverlay->show();
        ownedUnitsOverlay->raise();
    });
    connect(closeOwnedButton, &QPushButton::clicked, this, [this]() {
        ownedUnitsOverlay->hide();
    });
    connect(shopButton, &QPushButton::clicked, this, &DrawPage::enterShopClicked);
    connect(deployButton, &QPushButton::clicked, this, &DrawPage::enterDeployClicked);
}

void DrawPage::setEncounterOptions(const QVector<EnemyEncounterInfo> &options)
{
    concealedOptions = options;
    hasRevealedCard = false;

    if (!concealedOptions.isEmpty()) {
        for (int index = concealedOptions.size() - 1; index > 0; --index) {
            const int swapIndex = QRandomGenerator::global()->bounded(index + 1);
            concealedOptions.swapItemsAt(index, swapIndex);
        }
        roundValueLabel->setText(QString::number(concealedOptions.first().round));
    } else {
        roundValueLabel->setText(QStringLiteral("1"));
    }

    difficultyValueLabel->setText(QStringLiteral("?"));
    styleValueLabel->setText(QStringLiteral("未翻牌"));
    hintValueLabel->setText(QStringLiteral("请从三张命运牌中单击翻开一张，以决定本轮敌军打法。"));
    shopButton->setEnabled(false);
    deployButton->setEnabled(false);
    ownedUnitsOverlay->hide();

    for (int index = 0; index < cardButtons.size(); ++index) {
        cardButtons.at(index)->setChecked(false);
        cardButtons.at(index)->setEnabled(index < concealedOptions.size());
        cardButtons.at(index)->setText(QStringLiteral("命运牌\n%1").arg(index + 1));
    }
}

void DrawPage::setOwnedUnits(const QVector<UnitPtr> &units, int capacity)
{
    ownedUnits = units;
    ownedUnitsCapacity = capacity;
    const bool benchFull = ownedUnits.size() >= ownedUnitsCapacity;
    capacityHintLabel->setText(
        benchFull
            ? QStringLiteral("当前已拥有 %1 / %2 个单位，总上限固定为 8 格。已满员，当前不能进入商店，只能直接部署。")
                  .arg(ownedUnits.size())
                  .arg(ownedUnitsCapacity)
            : QStringLiteral("当前已拥有 %1 / %2 个单位，总上限固定为 8 格。")
                  .arg(ownedUnits.size())
                  .arg(ownedUnitsCapacity));
    refreshOwnedUnitsView();
}

void DrawPage::revealCard(int index)
{
    if (hasRevealedCard || index < 0 || index >= concealedOptions.size()) {
        return;
    }

    const EnemyEncounterInfo info = concealedOptions.at(index);
    hasRevealedCard = true;
    difficultyValueLabel->setText(info.difficultyLabel);
    styleValueLabel->setText(info.styleLabel);
    hintValueLabel->setText(info.styleDescription);
    shopButton->setEnabled(ownedUnits.size() < ownedUnitsCapacity);
    deployButton->setEnabled(true);

    for (int cardIndex = 0; cardIndex < cardButtons.size(); ++cardIndex) {
        auto *cardButton = cardButtons.at(cardIndex);
        const bool chosen = cardIndex == index;
        cardButton->setChecked(chosen);
        cardButton->setEnabled(false);
        if (chosen) {
            cardButton->setText(QStringLiteral("%1\nDifficulty %2").arg(info.styleLabel, info.difficultyLabel));
        } else {
            cardButton->setText(QStringLiteral("未抽中"));
        }
    }

    emit encounterSelected(info);
}

void DrawPage::refreshOwnedUnitsView()
{
    if (!ownedUnitsGridLayout) {
        return;
    }

    QLayoutItem *child = nullptr;
    while ((child = ownedUnitsGridLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    for (int index = 0; index < ownedUnitsCapacity; ++index) {
        auto *card = new QFrame(ownedUnitsCard);
        const bool hasUnit = index < ownedUnits.size();
        const UnitPtr unit = hasUnit ? ownedUnits.at(index) : nullptr;
        card->setObjectName(hasUnit ? "ownedUnitCard" : "ownedEmptyCard");
        card->setMinimumSize(UiScale::size(280, 360));
        auto *layout = new QVBoxLayout(card);
        layout->setContentsMargins(UiScale::margins(14, 14, 14, 14));
        layout->setSpacing(UiScale::scaled(6));

        if (hasUnit) {
            auto *nameLabel = new QLabel(QStringLiteral("%1 | %2 金币").arg(unit->name()).arg(unit->cost()), card);
            nameLabel->setObjectName("cardTitle");
            auto *roleLabel = new QLabel(
                QStringLiteral("定位：%1").arg(unit->roleDescription().isEmpty() ? QStringLiteral("-") : unit->roleDescription()),
                card);
            roleLabel->setObjectName("hintText");
            roleLabel->setWordWrap(true);
            auto *statsLabel = new QLabel(
                QStringLiteral("HP %1  ATK %2  Range %3\nAS %4  Mana %5  Gain %6/%7")
                    .arg(unit->maxHp())
                    .arg(unit->atk())
                    .arg(unit->range())
                    .arg(unit->attackSpeed(), 0, 'f', 1)
                    .arg(unit->maxMana())
                    .arg(unit->manaGainOnAttack())
                    .arg(unit->manaGainOnHit()),
                card);
            statsLabel->setObjectName("panelBody");
            statsLabel->setWordWrap(true);
            auto *skillLabel = new QLabel(
                QStringLiteral("技能：%1\n%2")
                    .arg(unit->skillName().isEmpty() ? QStringLiteral("-") : unit->skillName(),
                         unit->skillDescription().isEmpty() ? QStringLiteral("暂无技能说明") : unit->skillDescription()),
                card);
            skillLabel->setObjectName("panelBody");
            skillLabel->setWordWrap(true);
            auto *traitsLabel = new QLabel(
                QStringLiteral("Traits：%1").arg(unit->traits().isEmpty() ? QStringLiteral("-")
                                                                          : unit->traits().join(QStringLiteral(", "))),
                card);
            traitsLabel->setObjectName("hintText");
            traitsLabel->setWordWrap(true);
            layout->addWidget(nameLabel);
            layout->addWidget(roleLabel);
            layout->addWidget(statsLabel);
            layout->addWidget(skillLabel);
            layout->addWidget(traitsLabel);
        } else {
            auto *emptyLabel = new QLabel(QStringLiteral("空槽位\nBench Capacity"), card);
            emptyLabel->setObjectName("hintText");
            emptyLabel->setAlignment(Qt::AlignCenter);
            layout->addStretch(1);
            layout->addWidget(emptyLabel);
            layout->addStretch(1);
        }

        ownedUnitsGridLayout->addWidget(card, index / 4, index % 4);
    }
}

void DrawPage::paintEvent(QPaintEvent *event)
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
