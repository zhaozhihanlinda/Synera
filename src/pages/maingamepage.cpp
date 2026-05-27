#include "pages/maingamepage.h"

#include "core/uiscale.h"
#include "widgets/benchwidget.h"
#include "widgets/boardwidget.h"

#include <QDebug>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

namespace {

QFrame *createInfoTile(const QString &labelText, QLabel *valueLabel, QWidget *parent)
{
    auto *tile = new QFrame(parent);
    tile->setObjectName("infoTile");

    auto *layout = new QVBoxLayout(tile);
    layout->setContentsMargins(UiScale::margins(16, 14, 16, 14));
    layout->setSpacing(UiScale::scaled(4));

    auto *label = new QLabel(labelText, tile);
    label->setObjectName("tileLabel");
    valueLabel->setParent(tile);
    valueLabel->setObjectName("tileValue");

    layout->addWidget(label);
    layout->addWidget(valueLabel);
    return tile;
}

}

MainGamePage::MainGamePage(QWidget *parent)
    : QWidget(parent)
    , gameManager(nullptr)
    , selectedBenchSlot(-1)
    , playerHudLabel(new QLabel(QStringLiteral("Player HP 100"), this))
    , roundHudLabel(new QLabel(QStringLiteral("Round 1 - 00:00"), this))
    , enemyHudLabel(new QLabel(QStringLiteral("Enemy HP 0"), this))
    , phaseTitleLabel(new QLabel(this))
    , phaseDescriptionLabel(new QLabel(this))
    , battleMetaLabel(new QLabel(QStringLiteral("Player Units 0 | Enemy Units 0"), this))
    , resourceInfoLabel(new QLabel(QStringLiteral("Gold 30 | Population 0 / 3"), this))
    , selectedNameLabel(new QLabel(QStringLiteral("未选中单位"), this))
    , selectedStatsLabel(new QLabel(QStringLiteral("点击棋盘单位以查看属性"), this))
    , selectedTraitsLabel(new QLabel(QStringLiteral("Traits: -"), this))
    , actionButton(nullptr)
    , battleTimer(new QTimer(this))
    , boardWidget(nullptr)
    , benchWidget(nullptr)
    , deployPanel(nullptr)
    , unitInfoPanel(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(UiScale::size(1280, 820));

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(UiScale::margins(72, 42, 72, 42));
    rootLayout->setSpacing(UiScale::scaled(20));

    auto *headerRow = new QHBoxLayout;
    headerRow->setSpacing(UiScale::scaled(18));

    auto *playerHudTile = createInfoTile(QStringLiteral("ALLY"), playerHudLabel, this);
    auto *roundHudTile = createInfoTile(QStringLiteral("BATTLEFLOW"), roundHudLabel, this);
    auto *enemyHudTile = createInfoTile(QStringLiteral("ENEMY"), enemyHudLabel, this);

    headerRow->addWidget(playerHudTile, 1);
    headerRow->addWidget(roundHudTile, 2);
    headerRow->addWidget(enemyHudTile, 1);

    auto *phasePanel = new QFrame(this);
    phasePanel->setObjectName("phasePanel");
    auto *phaseLayout = new QVBoxLayout(phasePanel);
    phaseLayout->setContentsMargins(UiScale::margins(36, 36, 36, 36));
    phaseLayout->setSpacing(UiScale::scaled(14));

    phaseTitleLabel->setObjectName("phaseTitle");
    phaseDescriptionLabel->setObjectName("phaseBody");
    phaseDescriptionLabel->setWordWrap(true);
    battleMetaLabel->setObjectName("panelBody");
    battleMetaLabel->setWordWrap(true);
    resourceInfoLabel->setObjectName("panelCaption");
    resourceInfoLabel->setWordWrap(true);

    actionButton = new QPushButton(phasePanel);
    actionButton->setObjectName("primaryButton");
    actionButton->setCursor(Qt::PointingHandCursor);
    actionButton->setMinimumHeight(UiScale::height(56));

    phaseLayout->addWidget(phaseTitleLabel);
    phaseLayout->addWidget(phaseDescriptionLabel);
    phaseLayout->addWidget(battleMetaLabel);
    phaseLayout->addWidget(resourceInfoLabel);
    phaseLayout->addSpacing(UiScale::scaled(8));
    phaseLayout->addWidget(actionButton, 0, Qt::AlignLeft);

    boardWidget = new BoardWidget(this);

    unitInfoPanel = new QFrame(this);
    unitInfoPanel->setObjectName("unitInfoPanel");
    auto *unitInfoLayout = new QVBoxLayout(unitInfoPanel);
    unitInfoLayout->setContentsMargins(UiScale::margins(22, 22, 22, 22));
    unitInfoLayout->setSpacing(UiScale::scaled(10));

    auto *unitInfoTitle = new QLabel(QStringLiteral("单位信息"), unitInfoPanel);
    unitInfoTitle->setObjectName("panelTitle");
    selectedNameLabel->setObjectName("selectedName");
    selectedStatsLabel->setObjectName("panelBody");
    selectedStatsLabel->setWordWrap(true);
    selectedTraitsLabel->setObjectName("panelCaption");
    selectedTraitsLabel->setWordWrap(true);

    unitInfoLayout->addWidget(unitInfoTitle);
    unitInfoLayout->addWidget(selectedNameLabel);
    unitInfoLayout->addWidget(selectedStatsLabel);
    unitInfoLayout->addWidget(selectedTraitsLabel);
    unitInfoLayout->addStretch(1);

    auto *battlefieldRow = new QHBoxLayout;
    battlefieldRow->setSpacing(UiScale::scaled(18));
    battlefieldRow->addWidget(boardWidget, 3);

    auto *sidePanel = new QVBoxLayout;
    sidePanel->setSpacing(UiScale::scaled(18));
    sidePanel->addWidget(phasePanel);
    sidePanel->addWidget(unitInfoPanel, 1);
    battlefieldRow->addLayout(sidePanel, 1);

    deployPanel = new QFrame(this);
    deployPanel->setObjectName("deployPanel");
    auto *deployLayout = new QVBoxLayout(deployPanel);
    deployLayout->setContentsMargins(UiScale::margins(14, 12, 14, 12));
    deployLayout->setSpacing(UiScale::scaled(10));

    benchWidget = new BenchWidget(deployPanel);
    deployLayout->addWidget(benchWidget);

    rootLayout->addLayout(headerRow);
    rootLayout->addLayout(battlefieldRow, 1);
    rootLayout->addWidget(deployPanel);

    setStyleSheet(UiScale::scaleStyleSheet(QStringLiteral(R"(
        MainGamePage { background-color: #090d15; }
        #pageTitle {
            color: #e4cd92;
            font-size: 38px;
            font-weight: 800;
        }
        #infoTile, #phasePanel, #unitInfoPanel, #deployPanel {
            background-color: rgba(10, 16, 28, 215);
            border: 2px solid rgba(174, 150, 98, 180);
            border-radius: 22px;
        }
        #tileLabel {
            color: #91a5c7;
            font-size: 13px;
            font-weight: 700;
        }
        #tileValue {
            color: #f2f5fa;
            font-size: 24px;
            font-weight: 800;
        }
        #phaseTitle {
            color: #f5e4bd;
            font-size: 32px;
            font-weight: 800;
        }
        #phaseBody {
            color: #d7e0ef;
            font-size: 20px;
            font-weight: 500;
        }
        #panelTitle {
            color: #e2cb92;
            font-size: 22px;
            font-weight: 800;
        }
        #selectedName {
            color: #f3f4f7;
            font-size: 24px;
            font-weight: 800;
        }
        #panelBody {
            color: #d7e0ef;
            font-size: 16px;
            font-weight: 500;
        }
        #panelCaption {
            color: #8eb0c9;
            font-size: 14px;
            font-weight: 600;
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

    connect(actionButton, &QPushButton::clicked, this, [this]() {
        if (gameManager && gameManager->phase() == GamePhase::Deploy) {
            qDebug() << "Start battle clicked";
            emit startBattleClicked();
        }
    });
    connect(boardWidget, &BoardWidget::selectionChanged, this, &MainGamePage::refreshSelectedUnitPanel);
    connect(boardWidget, &BoardWidget::tileActivated, this, &MainGamePage::handleBoardActivation);
    connect(boardWidget, &BoardWidget::benchUnitDroppedOnTile, this, [this](int slot, const BoardPosition &position) {
        if (!gameManager) {
            return;
        }
        if (gameManager->deployUnitFromBench(slot, position)) {
            clearBenchSelection();
        } else {
            refreshBoardWidgets();
        }
    });
    connect(boardWidget, &BoardWidget::boardUnitDroppedOnTile, this, [this](const BoardPosition &from, const BoardPosition &to) {
        if (!gameManager) {
            return;
        }
        if (gameManager->repositionUnit(from, to)) {
            boardWidget->setSelectedPosition(to);
        }
        refreshBoardWidgets();
    });
    connect(benchWidget, &BenchWidget::slotClicked, this, &MainGamePage::handleBenchSlotClick);
    connect(benchWidget, &BenchWidget::boardUnitDroppedToBench, this, [this](const BoardPosition &position, int slot) {
        if (!gameManager) {
            return;
        }
        if (gameManager->returnUnitToBenchSlot(position, slot)) {
            clearBoardSelection();
            refreshBoardWidgets();
        } else {
            refreshBoardWidgets();
        }
    });
    connect(battleTimer, &QTimer::timeout, this, &MainGamePage::tickBattleTimer);
    battleTimer->setInterval(1000);

    refreshFromGameState();
}

void MainGamePage::setGameManager(GameManager *manager)
{
    gameManager = manager;
    refreshFromGameState();
}

void MainGamePage::refreshFromGameState()
{
    if (gameManager) {
        boardWidget->setBoard(&gameManager->board());
        benchWidget->setBoard(&gameManager->board());
    } else {
        boardWidget->setBoard(nullptr);
        benchWidget->setBoard(nullptr);
    }

    refreshPhaseUi();
    refreshHud();
    refreshBoardWidgets();
    refreshSelectedUnitPanel();
}

void MainGamePage::refreshPhaseUi()
{
    const RoundState state = gameManager ? gameManager->roundState() : RoundState{};
    const EnemyEncounterInfo encounter = gameManager ? gameManager->currentEncounterInfo() : EnemyEncounterInfo{};

    if (state.phase == GamePhase::Deploy) {
        phaseTitleLabel->setText(QStringLiteral("当前阶段：部署阶段"));
        phaseDescriptionLabel->setText(QStringLiteral("部署阶段：整理阵型并确认上阵单位。"));
        battleMetaLabel->setText(QStringLiteral("Difficulty %1 | Style %2\n%3")
                                     .arg(encounter.difficultyLabel.isEmpty() ? QStringLiteral("-") : encounter.difficultyLabel)
                                     .arg(encounter.styleLabel.isEmpty() ? QStringLiteral("-") : encounter.styleLabel)
                                     .arg(encounter.styleDescription.isEmpty()
                                              ? QStringLiteral("未抽取敌方打法信息。")
                                              : encounter.styleDescription));
        resourceInfoLabel->setText(QStringLiteral("Gold %1 | Population %2 / %3\n当前阶段可根据敌方打法提示调整部署。")
                                       .arg(gameManager ? gameManager->playerGold() : 30)
                                       .arg(gameManager ? gameManager->currentPopulation() : 0)
                                       .arg(gameManager ? gameManager->maxPopulation() : 3));
        actionButton->setText(QStringLiteral("开始战斗"));
        actionButton->setEnabled(gameManager && gameManager->currentPopulation() > 0);
        deployPanel->show();
        battleTimer->stop();
    } else {
        phaseTitleLabel->setText(QStringLiteral("当前阶段：战斗阶段"));
        phaseDescriptionLabel->setText(QStringLiteral("战斗情报侧栏：观测局势、查看单位并等待自动结算。"));
        battleMetaLabel->setText(QStringLiteral("Battle Feed：Player Units %1 | Enemy Units %2")
                                     .arg(gameManager ? gameManager->board().activePlayerUnitCount() : 0)
                                     .arg(gameManager ? gameManager->board().activeEnemyUnitCount() : 0));
        resourceInfoLabel->setText(QStringLiteral("Engaged Population %1 / %2")
                                       .arg(gameManager ? gameManager->currentPopulation() : 0)
                                       .arg(gameManager ? gameManager->maxPopulation() : 3));
        actionButton->setText(QStringLiteral("战斗进行中"));
        actionButton->setEnabled(false);
        deployPanel->hide();
        if (gameManager && !battleTimer->isActive()) {
            battleTimer->start();
        }
    }
}

void MainGamePage::refreshSelectedUnitPanel()
{
    const UnitPtr unit = boardWidget->selectedUnit();
    if (!unit) {
        selectedNameLabel->setText(QStringLiteral("未选中单位"));
        selectedStatsLabel->setText(QStringLiteral("点击棋盘中的单位后，这里会显示基础属性和技能说明。"));
        selectedTraitsLabel->setText(QStringLiteral("Traits: -"));
        return;
    }

    selectedNameLabel->setText(unit->name());
    selectedStatsLabel->setText(
        QStringLiteral("Cost: %1\nHP: %2 / %3\nMana: %4 / %5\nATK: %6\nRange: %7\nAS: %8\nMana Gain: %9 / %10\nSkill: %11\nSkill Detail: %12\nSide: %13")
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
            .arg(unit->owner() == ControllerSide::PlayerCtrl ? QStringLiteral("Player") : QStringLiteral("Enemy")));
    selectedTraitsLabel->setText(QStringLiteral("%1\nTraits: %2")
                                     .arg(unit->roleDescription().isEmpty() ? QStringLiteral("Role: -")
                                                                            : QStringLiteral("Role: %1").arg(unit->roleDescription()))
                                     .arg(unit->traits().isEmpty()
                                              ? QStringLiteral("-")
                                              : unit->traits().join(QStringLiteral(", "))));
}

void MainGamePage::refreshBoardWidgets()
{
    benchWidget->setSelectedSlot(selectedBenchSlot);

    if (!gameManager) {
        boardWidget->clearPendingAction();
        refreshHud();
        refreshSelectedUnitPanel();
        return;
    }

    if (selectedBenchSlot >= 0) {
        boardWidget->setPendingPlacementUnit(gameManager->board().benchUnitAt(selectedBenchSlot));
    } else {
        const BoardPosition selectedPosition = boardWidget->selectedPosition();
        const UnitPtr selectedUnit = selectedPosition.isValid()
            ? gameManager->board().unitAt(selectedPosition.row, selectedPosition.col)
            : nullptr;
        if (selectedUnit && selectedUnit->owner() == ControllerSide::PlayerCtrl
            && gameManager->phase() == GamePhase::Deploy) {
            boardWidget->setPendingMoveUnit(selectedUnit, selectedPosition);
        } else {
            boardWidget->clearPendingAction();
        }
    }

    refreshHud();
    refreshSelectedUnitPanel();
}

void MainGamePage::clearBenchSelection()
{
    selectedBenchSlot = -1;
    refreshBoardWidgets();
}

void MainGamePage::clearBoardSelection()
{
    boardWidget->setSelectedPosition(BoardPosition{});
    refreshBoardWidgets();
}

void MainGamePage::handleBoardActivation(const BoardPosition &position, Qt::MouseButton button)
{
    if (!gameManager || !position.isValid()) {
        return;
    }

    if (button == Qt::LeftButton && selectedBenchSlot >= 0) {
        if (gameManager->deployUnitFromBench(selectedBenchSlot, position)) {
            clearBenchSelection();
        } else {
            refreshBoardWidgets();
        }
        return;
    }

    if (button == Qt::LeftButton && gameManager->phase() == GamePhase::Deploy) {
        const BoardPosition selectedPosition = boardWidget->selectedPosition();
        const UnitPtr clickedUnit = gameManager->board().unitAt(position.row, position.col);
        const UnitPtr selectedUnit = selectedPosition.isValid()
            ? gameManager->board().unitAt(selectedPosition.row, selectedPosition.col)
            : nullptr;

        if (!selectedUnit) {
            if (clickedUnit && clickedUnit->owner() == ControllerSide::PlayerCtrl) {
                boardWidget->setSelectedPosition(position);
                refreshBoardWidgets();
            }
            return;
        }

        if (selectedPosition == position) {
            refreshBoardWidgets();
            return;
        }

        if (gameManager->repositionUnit(selectedPosition, position)) {
            boardWidget->setSelectedPosition(position);
        }
        refreshBoardWidgets();
        return;
    }

    if (button == Qt::RightButton) {
        if (gameManager->returnUnitToBench(position)) {
            clearBoardSelection();
            refreshBoardWidgets();
        }
    }
}

void MainGamePage::handleBenchSlotClick(int slot)
{
    if (!gameManager || gameManager->phase() != GamePhase::Deploy) {
        return;
    }

    if (!gameManager->board().benchUnitAt(slot)) {
        clearBenchSelection();
        return;
    }

    clearBoardSelection();
    selectedBenchSlot = (selectedBenchSlot == slot) ? -1 : slot;
    refreshBoardWidgets();
}

void MainGamePage::refreshHud()
{
    const RoundState state = gameManager ? gameManager->roundState() : RoundState{};
    const int enemyHp = gameManager ? gameManager->board().totalHpForSide(ControllerSide::EnemyCtrl) : 0;
    const QString timerText = QStringLiteral("%1:%2")
                                  .arg(state.battleElapsedSeconds / 60, 2, 10, QChar('0'))
                                  .arg(state.battleElapsedSeconds % 60, 2, 10, QChar('0'));

    playerHudLabel->setText(QStringLiteral("HP %1").arg(gameManager ? gameManager->playerHp() : 100));
    roundHudLabel->setText(QStringLiteral("Round %1 - %2").arg(state.currentRound).arg(timerText));
    enemyHudLabel->setText(QStringLiteral("Enemy HP %1").arg(enemyHp));
}

void MainGamePage::tickBattleTimer()
{
    if (gameManager) {
        gameManager->tickBattleTimer();
    }
    refreshHud();
}

void MainGamePage::paintEvent(QPaintEvent *event)
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
