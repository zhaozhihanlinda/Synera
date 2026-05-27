#include "core/gamemanager.h"

#include <memory>

#include "core/playertemplatelibrary.h"
#include "core/unit.h"
#include "core/unittemplate.h"
GameManager::GameManager(QObject *parent)
    : QObject(parent)
    , m_playerHp(100)
    , m_playerGold(30)
    , m_maxPopulation(3)
{
    m_roundState.beginDeployPhase(1);
    m_roundState.finalRound = 5;
}

void GameManager::setPlayerProfile(const PlayerProfile &profile)
{
    m_profile = profile;
}

PlayerProfile GameManager::playerProfile() const
{
    return m_profile;
}

void GameManager::initNewGame()
{
    m_board.clear();
    m_playerHp = 100;
    m_playerGold = 30;
    m_maxPopulation = 3;
    m_roundState.beginDeployPhase(1);
    m_roundState.finalRound = 5;
    m_lastBattleResult = BattleResult{};
    drawCurrentRoundEncounter();

    const QVector<UnitTemplate> starterTemplates = defaultStarterBenchTemplates();
    const UnitTemplate abyssServant{
        QStringLiteral("abyss_servant"),
        QStringLiteral("深渊侍从"),
        {QStringLiteral("虚空")},
        0,
        QStringLiteral("敌方测试单位"),
        260,
        26,
        1,
        1.0,
        50,
        10,
        5
    };

    for (int index = 0; index < starterTemplates.size(); ++index) {
        m_board.addBenchUnit(starterTemplates.at(index).createUnit(
            QStringLiteral("player_unit_%1").arg(index + 1, 3, 10, QChar('0')),
            ControllerSide::PlayerCtrl));
    }
    m_board.placeUnit(abyssServant.createUnit(QStringLiteral("enemy_unit_001"), ControllerSide::EnemyCtrl), 1, 3);
    m_board.placeUnit(abyssServant.createUnit(QStringLiteral("enemy_unit_002"), ControllerSide::EnemyCtrl), 1, 4);
}

void GameManager::nextRound()
{
    m_roundState.beginDeployPhase(m_roundState.currentRound + 1);
    drawCurrentRoundEncounter();
}

void GameManager::drawCurrentRoundEncounter()
{
    m_currentEncounterInfo = EnemyEncounterInfo{};
}

QVector<EnemyEncounterInfo> GameManager::currentRoundEncounterPool() const
{
    return enemyEncounterPoolForRound(m_roundState.currentRound);
}

void GameManager::setCurrentEncounterInfo(const EnemyEncounterInfo &info)
{
    m_currentEncounterInfo = info;
}

void GameManager::setPhase(GamePhase phase)
{
    if (phase == GamePhase::Battle) {
        m_roundState.beginBattlePhase();
    } else {
        m_roundState.beginDeployPhase(m_roundState.currentRound);
    }
}

GamePhase GameManager::phase() const
{
    return m_roundState.phase;
}

EnemyEncounterInfo GameManager::currentEncounterInfo() const
{
    return m_currentEncounterInfo;
}

const RoundState &GameManager::roundState() const
{
    return m_roundState;
}

bool GameManager::canInteractWithBoard() const
{
    return m_roundState.canInteractWithBoard();
}

QVector<UnitPtr> GameManager::ownedPlayerUnits() const
{
    QVector<UnitPtr> units;
    for (int slot = 0; slot < m_board.benchCapacity(); ++slot) {
        if (const UnitPtr unit = m_board.benchUnitAt(slot)) {
            if (unit->owner() == ControllerSide::PlayerCtrl) {
                units.append(unit);
            }
        }
    }

    for (int row = 0; row < m_board.rowCount(); ++row) {
        for (int col = 0; col < m_board.columnCount(); ++col) {
            if (const UnitPtr unit = m_board.unitAt(row, col)) {
                if (unit->owner() == ControllerSide::PlayerCtrl) {
                    units.append(unit);
                }
            }
        }
    }

    return units;
}

bool GameManager::canDeployUnitFromBench(int slot, const BoardPosition &target) const
{
    if (!m_roundState.canDeploy() || !target.isValid()) {
        return false;
    }

    const UnitPtr unit = m_board.benchUnitAt(slot);
    if (!unit) {
        return false;
    }

    if (unit->owner() == ControllerSide::PlayerCtrl && currentPopulation() >= m_maxPopulation) {
        return false;
    }

    return m_board.canPlaceUnit(unit, target.row, target.col);
}

bool GameManager::deployUnitFromBench(int slot, const BoardPosition &target)
{
    if (!canDeployUnitFromBench(slot, target)) {
        return false;
    }
    return m_board.moveBenchToBoard(slot, target.row, target.col);
}

bool GameManager::canReturnUnitToBench(const BoardPosition &position) const
{
    if (!m_roundState.canDeploy() || !position.isValid()) {
        return false;
    }

    const UnitPtr unit = m_board.unitAt(position.row, position.col);
    return unit && unit->owner() == ControllerSide::PlayerCtrl
        && m_board.canMoveBoardToBench(position.row, position.col);
}

bool GameManager::returnUnitToBench(const BoardPosition &position)
{
    if (!canReturnUnitToBench(position)) {
        return false;
    }
    return m_board.moveBoardToBench(position.row, position.col);
}

bool GameManager::canReturnUnitToBenchSlot(const BoardPosition &position, int slot) const
{
    if (!m_roundState.canDeploy() || !position.isValid()) {
        return false;
    }

    const UnitPtr unit = m_board.unitAt(position.row, position.col);
    return unit && unit->owner() == ControllerSide::PlayerCtrl
        && m_board.canMoveBoardToBenchSlot(position.row, position.col, slot);
}

bool GameManager::returnUnitToBenchSlot(const BoardPosition &position, int slot)
{
    if (!canReturnUnitToBenchSlot(position, slot)) {
        return false;
    }
    return m_board.moveBoardToBenchSlot(position.row, position.col, slot);
}

bool GameManager::canRepositionUnit(const BoardPosition &from, const BoardPosition &to) const
{
    if (!m_roundState.canDeploy() || !from.isValid() || !to.isValid() || from == to) {
        return false;
    }

    const UnitPtr unit = m_board.unitAt(from.row, from.col);
    if (!unit || unit->owner() != ControllerSide::PlayerCtrl) {
        return false;
    }

    return m_board.canMoveUnit(from.row, from.col, to.row, to.col)
        || m_board.canSwapUnits(from.row, from.col, to.row, to.col);
}

bool GameManager::repositionUnit(const BoardPosition &from, const BoardPosition &to)
{
    if (!canRepositionUnit(from, to)) {
        return false;
    }
    return m_board.relocateOrSwapUnit(from.row, from.col, to.row, to.col);
}

void GameManager::beginBattlePhase()
{
    m_roundState.beginBattlePhase();
}

void GameManager::tickBattleTimer()
{
    m_roundState.tickBattleTimer();
}

void GameManager::saveBattleResult(const BattleResult &result)
{
    m_lastBattleResult = result;
    m_playerGold += result.rewardGold;
    m_playerHp -= result.damage;
    if (m_playerHp < 0) {
        m_playerHp = 0;
    }
}

BattleResult GameManager::lastBattleResult() const
{
    return m_lastBattleResult;
}

Board &GameManager::board()
{
    return m_board;
}

const Board &GameManager::board() const
{
    return m_board;
}

int GameManager::playerHp() const
{
    return m_playerHp;
}

int GameManager::playerGold() const
{
    return m_playerGold;
}

int GameManager::currentPopulation() const
{
    return m_board.activePlayerUnitCount();
}

int GameManager::maxPopulation() const
{
    return m_maxPopulation;
}

int GameManager::currentRound() const
{
    return m_roundState.currentRound;
}

int GameManager::finalRound() const
{
    return m_roundState.finalRound;
}
