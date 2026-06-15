#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class GameManager;
class PageManager;
class StartPage;
class ProfilePage;
class InitInfoPage;
class DrawPage;
class ShopPage;
class BattleCountdownPage;
class MainGamePage;
class RoundResultPage;
class VictoryPage;
class DefeatPage;
class QTimer;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    GameManager *gameManager;
    PageManager *pageManager;
    StartPage *startPage;
    ProfilePage *profilePage;
    InitInfoPage *initInfoPage;
    DrawPage *drawPage;
    ShopPage *shopPage;
    BattleCountdownPage *battleCountdownPage;
    MainGamePage *mainGamePage;
    RoundResultPage *roundResultPage;
    VictoryPage *victoryPage;
    DefeatPage *defeatPage;
    QTimer *battleSimulationTimer;
    int activeBattleRound;

    void prepareInitInfoPage();
    void prepareDrawPage();
    void prepareShopPage();
    void prepareMainGamePage();
    void prepareRoundResultPage();
    void startNewSession();
    void showCurrentEnemyPreview();
    void tickBattleSimulation();
    void resolveBattleAndShowResult();
};

#endif // MAINWINDOW_H
