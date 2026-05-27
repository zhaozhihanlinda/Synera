#ifndef MAINGAMEPAGE_H
#define MAINGAMEPAGE_H

#include <QWidget>

#include "core/gamemanager.h"

class BenchWidget;
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

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void refreshPhaseUi();
    void refreshSelectedUnitPanel();
    void refreshBoardWidgets();
    void refreshHud();
    void clearBenchSelection();
    void clearBoardSelection();
    void handleBoardActivation(const BoardPosition &position, Qt::MouseButton button);
    void handleBenchSlotClick(int slot);
    void tickBattleTimer();

    GameManager *gameManager;
    int selectedBenchSlot;
    QLabel *playerHudLabel;
    QLabel *roundHudLabel;
    QLabel *enemyHudLabel;
    QLabel *phaseTitleLabel;
    QLabel *phaseDescriptionLabel;
    QLabel *battleMetaLabel;
    QLabel *resourceInfoLabel;
    QLabel *selectedNameLabel;
    QLabel *selectedStatsLabel;
    QLabel *selectedTraitsLabel;
    QPushButton *actionButton;
    QTimer *battleTimer;
    BoardWidget *boardWidget;
    BenchWidget *benchWidget;
    QFrame *deployPanel;
    QFrame *unitInfoPanel;
};

#endif // MAINGAMEPAGE_H
