#include "pages/mainwindow.h"

#include "core/uiscale.h"
#include "pages/battlecountdownpage.h"
#include "pages/defeatpage.h"
#include "core/gamemanager.h"
#include "pages/drawpage.h"
#include "pages/initinfopage.h"
#include "pages/maingamepage.h"
#include "core/pagemanager.h"
#include "pages/profilepage.h"
#include "pages/roundresultpage.h"
#include "pages/shoppage.h"
#include "pages/startpage.h"
#include "pages/victorypage.h"
#include "./ui_mainwindow.h"

#include <QAbstractButton>
#include <QApplication>
#include <QMessageBox>
#include <QStackedWidget>
#include <QStatusBar>
#include <QStringList>
#include <QTimer>

namespace {

const int kBattleSimulationIntervalMs = 750;
const int kBattleSimulationMaxSeconds = 30;

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , stackedWidget(nullptr)
    , gameManager(nullptr)
    , pageManager(nullptr)
    , startPage(nullptr)
    , profilePage(nullptr)
    , initInfoPage(nullptr)
    , drawPage(nullptr)
    , shopPage(nullptr)
    , battleCountdownPage(nullptr)
    , mainGamePage(nullptr)
    , roundResultPage(nullptr)
    , victoryPage(nullptr)
    , defeatPage(nullptr)
    , battleSimulationTimer(new QTimer(this))
    , activeBattleRound(0)
{
    ui->setupUi(this);

    stackedWidget = new QStackedWidget(this);
    gameManager = new GameManager(this);
    pageManager = new PageManager(stackedWidget, this);

    startPage = new StartPage(stackedWidget);
    profilePage = new ProfilePage(stackedWidget);
    initInfoPage = new InitInfoPage(stackedWidget);
    drawPage = new DrawPage(stackedWidget);
    shopPage = new ShopPage(stackedWidget);
    battleCountdownPage = new BattleCountdownPage(stackedWidget);
    mainGamePage = new MainGamePage(stackedWidget);
    roundResultPage = new RoundResultPage(stackedWidget);
    victoryPage = new VictoryPage(stackedWidget);
    defeatPage = new DefeatPage(stackedWidget);

    pageManager->registerPage(PageId::Start, startPage);
    pageManager->registerPage(PageId::Profile, profilePage);
    pageManager->registerPage(PageId::InitInfo, initInfoPage);
    pageManager->registerPage(PageId::Draw, drawPage);
    pageManager->registerPage(PageId::Shop, shopPage);
    pageManager->registerPage(PageId::BattleCountdown, battleCountdownPage);
    pageManager->registerPage(PageId::MainGame, mainGamePage);
    pageManager->registerPage(PageId::RoundResult, roundResultPage);
    pageManager->registerPage(PageId::Victory, victoryPage);
    pageManager->registerPage(PageId::Defeat, defeatPage);

    connect(startPage, &StartPage::startClicked, this, [this]() {
        pageManager->switchTo(PageId::Profile);
    });

    connect(profilePage, &ProfilePage::confirmClicked, this, [this](const PlayerProfile &profile) {
        gameManager->setPlayerProfile(profile);
        gameManager->initNewGame();
        prepareInitInfoPage();
        pageManager->switchTo(PageId::InitInfo);
    });

    connect(initInfoPage, &InitInfoPage::enterGameClicked, this, [this]() {
        prepareDrawPage();
        pageManager->switchTo(PageId::Draw);
    });

    connect(drawPage, &DrawPage::encounterSelected, this, [this](const EnemyEncounterInfo &info) {
        gameManager->setCurrentEncounterInfo(info);
    });
    connect(drawPage, &DrawPage::enterShopClicked, this, [this]() {
        prepareShopPage();
        pageManager->switchTo(PageId::Shop);
    });
    connect(drawPage, &DrawPage::enterDeployClicked, this, [this]() {
        gameManager->setPhase(GamePhase::Deploy);
        prepareMainGamePage();
        pageManager->switchTo(PageId::MainGame);
    });

    connect(shopPage, &ShopPage::enterDeployClicked, this, [this]() {
        gameManager->setPhase(GamePhase::Deploy);
        prepareMainGamePage();
        pageManager->switchTo(PageId::MainGame);
    });
    connect(shopPage, &ShopPage::buyUnitClicked, this, [this](const QString &templateId) {
        if (gameManager->buyUnit(templateId)) {
            prepareShopPage();
        }
    });
    connect(shopPage, &ShopPage::cancelPurchaseClicked, this, [this](const QString &templateId) {
        if (gameManager->cancelPurchasedUnit(templateId)) {
            prepareShopPage();
        }
    });
    connect(shopPage, &ShopPage::sellUnitClicked, this, [this](const QString &unitId) {
        if (gameManager->sellUnitById(unitId)) {
            prepareShopPage();
        }
    });
    connect(shopPage, &ShopPage::refreshShopClicked, this, [this]() {
        if (gameManager->refreshShop()) {
            prepareShopPage();
        }
    });

    connect(mainGamePage, &MainGamePage::startBattleClicked, this, [this]() {
        if (!gameManager->canStartBattle()) {
            prepareMainGamePage();
            return;
        }
        pageManager->switchTo(PageId::BattleCountdown);
        showCurrentEnemyPreview();
        battleCountdownPage->startCountdown(3);
    });
    connect(battleCountdownPage, &BattleCountdownPage::countdownFinished, this, [this]() {
        if (!gameManager->canStartBattle()) {
            prepareMainGamePage();
            pageManager->switchTo(PageId::MainGame);
            return;
        }
        gameManager->beginBattlePhase();
        activeBattleRound = gameManager->currentRound();
        prepareMainGamePage();
        pageManager->switchTo(PageId::MainGame);
        battleSimulationTimer->start(kBattleSimulationIntervalMs);
    });
    connect(mainGamePage, &MainGamePage::returnShopClicked, this, [this]() {
        prepareShopPage();
        pageManager->switchTo(PageId::Shop);
    });

    connect(roundResultPage, &RoundResultPage::continueClicked, this, [this]() {
        if (gameManager->playerHp() <= 0) {
            pageManager->switchTo(PageId::Defeat);
        } else if (gameManager->currentRound() >= gameManager->finalRound()
                   && gameManager->lastBattleResult().win) {
            pageManager->switchTo(PageId::Victory);
        } else {
            gameManager->nextRound();
            prepareDrawPage();
            pageManager->switchTo(PageId::Draw);
        }
    });

    connect(victoryPage, &VictoryPage::restartClicked, this, [this]() {
        startNewSession();
    });
    connect(victoryPage, &VictoryPage::returnToStartClicked, this, [this]() {
        pageManager->switchTo(PageId::Start);
    });
    connect(victoryPage, &VictoryPage::exitClicked, qApp, &QApplication::quit);

    connect(defeatPage, &DefeatPage::restartClicked, this, [this]() {
        startNewSession();
    });
    connect(defeatPage, &DefeatPage::returnToStartClicked, this, [this]() {
        pageManager->switchTo(PageId::Start);
    });
    connect(defeatPage, &DefeatPage::exitClicked, qApp, &QApplication::quit);
    connect(battleSimulationTimer, &QTimer::timeout, this, &MainWindow::tickBattleSimulation);

    setCentralWidget(stackedWidget);
    setWindowTitle(QStringLiteral("Synera"));
    resize(UiScale::size(1520, 980));
    setMinimumSize(UiScale::size(1280, 820));
    statusBar()->hide();

    pageManager->switchTo(PageId::Start);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::prepareInitInfoPage()
{
    const PlayerProfile profile = gameManager->playerProfile();
    initInfoPage->setGameInfo(profile,
                              gameManager->playerHp(),
                              gameManager->playerGold(),
                              gameManager->maxPopulation());
}

void MainWindow::prepareDrawPage()
{
    drawPage->setEncounterOptions(gameManager->currentRoundEncounterPool());
    drawPage->setOwnedUnits(gameManager->ownedPlayerUnits(), gameManager->board().benchCapacity());
}

void MainWindow::prepareShopPage()
{
    shopPage->setGameInfo(gameManager->currentRound(),
                          gameManager->playerGold(),
                          gameManager->currentPopulation(),
                          gameManager->maxPopulation(),
                          gameManager->ownedPlayerUnits(),
                          gameManager->board().benchCapacity(),
                          gameManager->sellableUnitTemplateIds(),
                          gameManager->sellableUnitIds(),
                          gameManager->currentShopTemplateIds(),
                          gameManager->shopRefreshCost(),
                          gameManager->canRefreshShop());
}

void MainWindow::prepareMainGamePage()
{
    mainGamePage->setGameManager(gameManager);
    mainGamePage->refreshFromGameState();
}

void MainWindow::prepareRoundResultPage()
{
    roundResultPage->setResultInfo(gameManager->currentRound(),
                                   gameManager->playerHp(),
                                   gameManager->lastBattleResult());
}

void MainWindow::startNewSession()
{
    battleSimulationTimer->stop();
    activeBattleRound = 0;
    pageManager->switchTo(PageId::Profile);
}

void MainWindow::showCurrentEnemyPreview()
{
    if (!gameManager) {
        return;
    }

    QStringList enemyLines;
    const Board &board = gameManager->board();
    for (int row = 0; row < board.rowCount(); ++row) {
        for (int col = 0; col < board.columnCount(); ++col) {
            const UnitPtr unit = board.unitAt(row, col);
            if (!unit || unit->owner() != ControllerSide::EnemyCtrl) {
                continue;
            }

            enemyLines.append(QStringLiteral("%1  (%2,%3)\nHP %4  ATK %5  Range %6  AS %7\n技能：%8")
                                  .arg(unit->name())
                                  .arg(row)
                                  .arg(col)
                                  .arg(unit->maxHp())
                                  .arg(unit->atk())
                                  .arg(unit->range())
                                  .arg(unit->attackSpeed(), 0, 'f', 1)
                                  .arg(unit->skillName().isEmpty() ? QStringLiteral("-") : unit->skillName()));
        }
    }

    QMessageBox preview(this);
    preview.setWindowTitle(QStringLiteral("本轮敌军预览"));
    preview.setIcon(QMessageBox::Information);
    preview.setText(QStringLiteral("本轮遇到的敌军角色"));
    preview.setInformativeText(enemyLines.isEmpty()
                                   ? QStringLiteral("当前没有敌军信息。")
                                   : enemyLines.join(QStringLiteral("\n\n")));
    preview.setStandardButtons(QMessageBox::Ok);
    preview.button(QMessageBox::Ok)->setText(QStringLiteral("开始倒计时"));
    preview.exec();
}

void MainWindow::tickBattleSimulation()
{
    if (!gameManager || gameManager->phase() != GamePhase::Battle
        || gameManager->currentRound() != activeBattleRound) {
        battleSimulationTimer->stop();
        return;
    }

    gameManager->advanceBattleSimulationTick();
    prepareMainGamePage();

    if (gameManager->isBattleResolved()
        || gameManager->roundState().battleElapsedSeconds >= kBattleSimulationMaxSeconds) {
        battleSimulationTimer->stop();
        resolveBattleAndShowResult();
    }
}

void MainWindow::resolveBattleAndShowResult()
{
    battleSimulationTimer->stop();
    const BattleResult result = gameManager->calculateBattleResult();
    gameManager->saveBattleResult(result);
    prepareRoundResultPage();
    pageManager->switchTo(PageId::RoundResult);
}
