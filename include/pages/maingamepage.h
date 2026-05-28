#ifndef MAINGAMEPAGE_H
#define MAINGAMEPAGE_H

#include <QWidget>

#include "core/gamemanager.h"

class BenchWidget;
class BattleInfoPanel;
class BoardWidget;
class QFrame;
class QLabel;
class QPushButton;
class QTimer;

class MainGamePage : public QWidget
{
    Q_OBJECT

public:
    explicit MainGamePage(QWidget *parent = nullptr);

    void setGameManager(GameManager *gameManager);
    void refreshFromGameState();

signals:
    void startBattleClicked();
    void returnShopClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void refreshPhaseUi();
    void refreshSelectedUnitPanel();
    void showDeployUnitInfo(const UnitPtr &unit);
    void refreshBoardWidgets();
    void refreshHud();
    void clearBenchSelection();
    void clearBoardSelection();
    void handleBoardActivation(const BoardPosition &position, Qt::MouseButton button);
    void handleBenchSlotClick(int slot);
    void showDeployWarning(const QString &message);
    void tickBattleTimer();

    GameManager *gameManager;
    int selectedBenchSlot;
    UnitPtr inspectedDeployUnit;
    QLabel *playerHudLabel;
    QLabel *playerHudTitleLabel;
    QLabel *roundHudLabel;
    QLabel *roundHudTitleLabel;
    QLabel *enemyHudLabel;
    QLabel *enemyHudTitleLabel;
    QLabel *maxPopulationHudLabel;
    QLabel *maxPopulationHudTitleLabel;
    QFrame *maxPopulationHudTile;
    QWidget *headerButtonPanel;
    QLabel *phaseTitleLabel;
    QLabel *phaseDescriptionLabel;
    QLabel *battleMetaLabel;
    QLabel *resourceInfoLabel;
    QLabel *selectedNameLabel;
    QLabel *selectedStatsLabel;
    QLabel *selectedTraitsLabel;
    QLabel *deployWarningLabel;
    QPushButton *actionButton;
    QPushButton *returnShopButton;
    QTimer *battleTimer;
    BoardWidget *boardWidget;
    BenchWidget *benchWidget;
    QWidget *sidePanelWidget;
    QFrame *deployPanel;
    QFrame *unitInfoPanel;
    BattleInfoPanel *battleInfoPanel;
};

#endif // MAINGAMEPAGE_H
