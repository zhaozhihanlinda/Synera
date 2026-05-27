#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>

#include "core/board.h"
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

    void setPhase(GamePhase phase);
    GamePhase phase() const;
    const RoundState &roundState() const;
    bool canInteractWithBoard() const;
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
    PlayerProfile m_profile;
    Board m_board;
    int m_playerHp;
    int m_playerGold;
    int m_maxPopulation;
    RoundState m_roundState;
    BattleResult m_lastBattleResult;
};

#endif // GAMEMANAGER_H
