#include "core/battlesimulator.h"

#include "core/unit.h"

#include <algorithm>
#include <cstdlib>
#include <QtGlobal>

namespace {

constexpr qreal kBattleTickSeconds = 1.0;
constexpr int kMaxBattleLogEntries = 5;

int distanceBetween(const BoardPosition &first, const BoardPosition &second)
{
    return std::abs(first.row - second.row) + std::abs(first.col - second.col);
}

}

void BattleSimulator::beginBattle()
{
    reset();
    appendLog(QStringLiteral("战斗开始，双方单位自动行动。"));
}

void BattleSimulator::reset()
{
    m_attackCooldowns.clear();
    m_battleLog.clear();
}

void BattleSimulator::advanceTick(Board &board)
{
    if (isResolved(board)) {
        return;
    }

    const QVector<UnitPtr> playerUnits = activeUnitsForSide(board, ControllerSide::PlayerCtrl);
    const QVector<UnitPtr> enemyUnits = activeUnitsForSide(board, ControllerSide::EnemyCtrl);

    for (const UnitPtr &unit : playerUnits) {
        if (!unit || !unit->isAlive()) {
            continue;
        }
        const UnitPtr target = nearestLivingTarget(unit, enemyUnits);
        if (!target) {
            continue;
        }
        if (distanceBetween(unit->boardPosition(), target->boardPosition()) <= unit->range()) {
            if (canUnitAttackThisTick(unit)) {
                unit->setState(UnitState::Attacking);
                target->applyDamage(unit->atk());
                unit->gainMana(unit->manaGainOnAttack());
                target->gainMana(target->manaGainOnHit());
                appendLog(QStringLiteral("%1 攻击 %2，造成 %3 伤害。")
                              .arg(unit->name(), target->name())
                              .arg(unit->atk()));
                resetUnitAttackCooldown(unit);
            } else {
                unit->setState(UnitState::Idle);
            }
        } else {
            moveUnitTowardTarget(board, unit, target);
        }
    }

    removeDeadUnits(board);

    const QVector<UnitPtr> remainingPlayerUnits = activeUnitsForSide(board, ControllerSide::PlayerCtrl);
    for (const UnitPtr &unit : activeUnitsForSide(board, ControllerSide::EnemyCtrl)) {
        if (!unit || !unit->isAlive()) {
            continue;
        }
        const UnitPtr target = nearestLivingTarget(unit, remainingPlayerUnits);
        if (!target) {
            continue;
        }
        if (distanceBetween(unit->boardPosition(), target->boardPosition()) <= unit->range()) {
            if (canUnitAttackThisTick(unit)) {
                unit->setState(UnitState::Attacking);
                target->applyDamage(unit->atk());
                unit->gainMana(unit->manaGainOnAttack());
                target->gainMana(target->manaGainOnHit());
                appendLog(QStringLiteral("%1 攻击 %2，造成 %3 伤害。")
                              .arg(unit->name(), target->name())
                              .arg(unit->atk()));
                resetUnitAttackCooldown(unit);
            } else {
                unit->setState(UnitState::Idle);
            }
        } else {
            moveUnitTowardTarget(board, unit, target);
        }
    }

    removeDeadUnits(board);
}

bool BattleSimulator::isResolved(const Board &board) const
{
    const BattleOutcome currentOutcome = outcome(board);
    return !currentOutcome.hasLivingPlayerUnits || !currentOutcome.hasLivingEnemyUnits;
}

BattleOutcome BattleSimulator::outcome(const Board &board) const
{
    BattleOutcome result;
    result.hasLivingPlayerUnits = !activeUnitsForSide(board, ControllerSide::PlayerCtrl).isEmpty();
    result.hasLivingEnemyUnits = !activeUnitsForSide(board, ControllerSide::EnemyCtrl).isEmpty();
    return result;
}

QStringList BattleSimulator::log() const
{
    return m_battleLog;
}

bool BattleSimulator::canUnitAttackThisTick(const UnitPtr &unit)
{
    if (!unit || unit->id().isEmpty()) {
        return false;
    }

    const QString unitId = unit->id();
    const qreal cooldown = std::max<qreal>(0.0, m_attackCooldowns.value(unitId, 0.0) - kBattleTickSeconds);
    m_attackCooldowns[unitId] = cooldown;
    return cooldown <= 0.0;
}

void BattleSimulator::resetUnitAttackCooldown(const UnitPtr &unit)
{
    if (!unit || unit->id().isEmpty()) {
        return;
    }

    m_attackCooldowns[unit->id()] = 1.0 / std::max<qreal>(0.1, unit->attackSpeed());
}

void BattleSimulator::appendLog(const QString &message)
{
    if (message.isEmpty()) {
        return;
    }

    m_battleLog.append(message);
    while (m_battleLog.size() > kMaxBattleLogEntries) {
        m_battleLog.removeFirst();
    }
}

QVector<UnitPtr> BattleSimulator::activeUnitsForSide(const Board &board, ControllerSide side) const
{
    QVector<UnitPtr> units;
    for (int row = 0; row < board.rowCount(); ++row) {
        for (int col = 0; col < board.columnCount(); ++col) {
            const UnitPtr unit = board.unitAt(row, col);
            if (unit && unit->owner() == side && unit->isAlive()) {
                units.append(unit);
            }
        }
    }
    return units;
}

UnitPtr BattleSimulator::nearestLivingTarget(const UnitPtr &attacker, const QVector<UnitPtr> &targets) const
{
    if (!attacker) {
        return nullptr;
    }

    UnitPtr bestTarget;
    int bestDistance = 0;
    for (const UnitPtr &target : targets) {
        if (!target || !target->isAlive()) {
            continue;
        }
        const int distance = distanceBetween(attacker->boardPosition(), target->boardPosition());
        const bool betterTarget = !bestTarget
            || distance < bestDistance
            || (distance == bestDistance && target->hp() < bestTarget->hp())
            || (distance == bestDistance && target->hp() == bestTarget->hp()
                && target->atk() > bestTarget->atk());
        if (betterTarget) {
            bestTarget = target;
            bestDistance = distance;
        }
    }
    return bestTarget;
}

BoardPosition BattleSimulator::nextStepTowardTarget(const Board &board,
                                                    const UnitPtr &unit,
                                                    const UnitPtr &target) const
{
    if (!unit || !target || !unit->isAlive() || !target->isAlive()) {
        return {};
    }

    const BoardPosition start = unit->boardPosition();
    const BoardPosition targetPosition = target->boardPosition();
    if (!board.isInside(start.row, start.col)
        || !board.isInside(targetPosition.row, targetPosition.col)) {
        return {};
    }

    QVector<QVector<bool>> visited(board.rowCount(), QVector<bool>(board.columnCount(), false));
    QVector<QVector<BoardPosition>> parent(board.rowCount(),
                                           QVector<BoardPosition>(board.columnCount(), BoardPosition{}));
    QVector<BoardPosition> queue;
    queue.append(start);
    visited[start.row][start.col] = true;

    const QVector<BoardPosition> directions = {
        {1, 0},
        {-1, 0},
        {0, 1},
        {0, -1}
    };

    for (int head = 0; head < queue.size(); ++head) {
        const BoardPosition current = queue.at(head);
        if (current != start && distanceBetween(current, targetPosition) <= unit->range()) {
            BoardPosition step = current;
            while (parent[step.row][step.col] != start) {
                step = parent[step.row][step.col];
            }
            return step;
        }

        for (const BoardPosition &direction : directions) {
            const BoardPosition next{current.row + direction.row, current.col + direction.col};
            if (!board.isInside(next.row, next.col) || visited[next.row][next.col]) {
                continue;
            }
            if (board.isOccupied(next.row, next.col)) {
                continue;
            }

            visited[next.row][next.col] = true;
            parent[next.row][next.col] = current;
            queue.append(next);
        }
    }

    return {};
}

void BattleSimulator::moveUnitTowardTarget(Board &board, const UnitPtr &unit, const UnitPtr &target)
{
    if (!unit || !target || !unit->isAlive() || !target->isAlive()) {
        return;
    }

    const BoardPosition from = unit->boardPosition();
    const BoardPosition next = nextStepTowardTarget(board, unit, target);
    if (next.isValid() && board.moveUnitDuringBattle(from.row, from.col, next.row, next.col)) {
        unit->setState(UnitState::Moving);
        appendLog(QStringLiteral("%1 向 %2 移动。").arg(unit->name(), target->name()));
        return;
    }

    unit->setState(UnitState::Idle);
}

void BattleSimulator::removeDeadUnits(Board &board)
{
    for (int row = 0; row < board.rowCount(); ++row) {
        for (int col = 0; col < board.columnCount(); ++col) {
            const UnitPtr unit = board.unitAt(row, col);
            if (unit && !unit->isAlive()) {
                appendLog(QStringLiteral("%1 被击败。").arg(unit->name()));
                board.removeUnit(row, col);
            }
        }
    }
}
