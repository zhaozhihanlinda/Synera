#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>

#include "core/battlesimulator.h"
#include "core/board.h"
#include "core/enemyencounterinfo.h"
#include "core/playerprofile.h"
#include "core/roundstate.h"

struct BattleResult
{
    bool win = false;
    int damage = 0;
    int rewardGold = 0;
};

class GameManager : public QObject
{
    Q_OBJECT

public:
    explicit GameManager(QObject *parent = nullptr);

    void setPlayerProfile(const PlayerProfile &profile);
    PlayerProfile playerProfile() const;

    void initNewGame();
    void nextRound();
    void drawCurrentRoundEncounter();
    QVector<EnemyEncounterInfo> currentRoundEncounterPool() const;
    void setCurrentEncounterInfo(const EnemyEncounterInfo &info);

    void setPhase(GamePhase phase);
    GamePhase phase() const;
    EnemyEncounterInfo currentEncounterInfo() const;
    const RoundState &roundState() const;
    bool canInteractWithBoard() const;
    QVector<UnitPtr> ownedPlayerUnits() const;
    bool canBuyUnit(const QString &templateId) const;
    bool buyUnit(const QString &templateId);
    bool canSellUnit(const QString &templateId) const;
    bool sellUnit(const QString &templateId);
    QVector<QString> sellableUnitTemplateIds() const;
    QVector<QString> currentShopTemplateIds() const;
    int shopRefreshCost() const;
    bool canRefreshShop() const;
    bool refreshShop();
    bool canDeployUnitFromBench(int slot, const BoardPosition &target) const;
    bool deployUnitFromBench(int slot, const BoardPosition &target);
    bool canReturnUnitToBench(const BoardPosition &position) const;
    bool returnUnitToBench(const BoardPosition &position);
    bool canReturnUnitToBenchSlot(const BoardPosition &position, int slot) const;
    bool returnUnitToBenchSlot(const BoardPosition &position, int slot);
    bool canRepositionUnit(const BoardPosition &from, const BoardPosition &to) const;
    bool repositionUnit(const BoardPosition &from, const BoardPosition &to);
    void beginBattlePhase();
    void tickBattleTimer();
    void advanceBattleSimulationTick();
    bool isBattleResolved() const;
    QStringList battleLog() const;

    BattleResult calculateBattleResult();
    void saveBattleResult(const BattleResult &result);
    BattleResult lastBattleResult() const;

    Board &board();
    const Board &board() const;

    int playerHp() const;
    int playerGold() const;
    int currentPopulation() const;
    int maxPopulation() const;
    int currentRound() const;
    int finalRound() const;

private:
    struct PurchasedUnitRecord
    {
        QString templateId;
        QString unitId;
    };

    struct PlayerBattleSnapshot
    {
        UnitPtr unit;
        BoardPosition position;
        int hp = 0;
        int mana = 0;
        UnitState state = UnitState::Idle;
    };

    void clearEnemyUnits();
    void loadCurrentEncounterFormation();
    void capturePlayerBattleSnapshot();
    void restorePlayerBattleSnapshot();
    void clearPlayerUnitsFromBoard();
    void ensureShopForCurrentRound();
    QVector<QString> generateShopTemplateIds() const;
    int combatPowerForSide(ControllerSide side) const;
    bool removePlayerUnitById(const QString &unitId);

    PlayerProfile m_profile;
    Board m_board;
    int m_playerHp;
    int m_playerGold;
    int m_maxPopulation;
    RoundState m_roundState;
    EnemyEncounterInfo m_currentEncounterInfo;
    BattleResult m_lastBattleResult;
    QVector<PurchasedUnitRecord> m_purchasedUnits;
    QVector<QString> m_shopTemplateIds;
    QVector<PlayerBattleSnapshot> m_playerBattleSnapshots;
    BattleSimulator m_battleSimulator;
    int m_shopRound;
};

#endif // GAMEMANAGER_H
