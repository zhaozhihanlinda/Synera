#ifndef BATTLESIMULATOR_H
#define BATTLESIMULATOR_H

#include <QHash>
#include <QStringList>
#include <QVector>

#include "core/board.h"

class BattleSimulator
{
public:
    void beginBattle();
    void reset();
    void advanceTick(Board &board);
    bool isResolved(const Board &board) const;
    QStringList log() const;

private:
    bool canUnitAttackThisTick(const UnitPtr &unit);
    void resetUnitAttackCooldown(const UnitPtr &unit);
    void appendLog(const QString &message);
    QVector<UnitPtr> activeUnitsForSide(const Board &board, ControllerSide side) const;
    UnitPtr nearestLivingTarget(const UnitPtr &attacker, const QVector<UnitPtr> &targets) const;
    BoardPosition nextStepTowardTarget(const Board &board, const UnitPtr &unit, const UnitPtr &target) const;
    void moveUnitTowardTarget(Board &board, const UnitPtr &unit, const UnitPtr &target);
    void removeDeadUnits(Board &board);

    QHash<QString, qreal> m_attackCooldowns;
    QStringList m_battleLog;
};

#endif // BATTLESIMULATOR_H
