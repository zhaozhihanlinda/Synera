#include "core/gamemanager.h"

#include "core/playertemplatelibrary.h"
#include "core/unit.h"
#include "core/unittemplate.h"

#include <QRandomGenerator>

namespace {

int winRewardForRound(int round)
{
    switch (round) {
    case 1:
        return 10;
    case 2:
        return 12;
    case 3:
        return 15;
    case 4:
        return 18;
    default:
        return 0;
    }
}

int loseDamageForRound(int round)
{
    switch (round) {
    case 1:
        return 15;
    case 2:
        return 20;
    case 3:
        return 25;
    case 4:
        return 30;
    default:
        return 40;
    }
}

int maxPopulationForRound(int round)
{
    switch (round) {
    case 1:
        return 3;
    case 2:
        return 4;
    case 3:
        return 5;
    case 4:
        return 6;
    default:
        return 7;
    }
}

constexpr int kShopSlotCount = 5;
constexpr int kShopRefreshCost = 1;

}

GameManager::GameManager(QObject *parent)
    : QObject(parent)
    , m_playerHp(100)
    , m_playerGold(30)
    , m_maxPopulation(maxPopulationForRound(1))
    , m_shopRound(0)
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
    m_maxPopulation = maxPopulationForRound(1);
    m_roundState.beginDeployPhase(1);
    m_roundState.finalRound = 5;
    m_lastBattleResult = BattleResult{};
    m_purchasedUnits.clear();
    m_shopTemplateIds.clear();
    m_playerBattleSnapshots.clear();
    m_battleSimulator.reset();
    m_shopRound = 0;
    drawCurrentRoundEncounter();

    const QVector<UnitTemplate> starterTemplates = defaultStarterBenchTemplates();
    for (int index = 0; index < starterTemplates.size(); ++index) {
        m_board.addBenchUnit(starterTemplates.at(index).createUnit(
            QStringLiteral("player_unit_%1").arg(index + 1, 3, 10, QChar('0')),
            ControllerSide::PlayerCtrl));
    }
}

void GameManager::nextRound()
{
    clearEnemyUnits();
    m_roundState.beginDeployPhase(m_roundState.currentRound + 1);
    m_maxPopulation = maxPopulationForRound(m_roundState.currentRound);
    m_shopTemplateIds.clear();
    m_shopRound = 0;
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
    loadCurrentEncounterFormation();
}

void GameManager::setPhase(GamePhase phase)
{
    if (phase == GamePhase::Battle) {
        beginBattlePhase();
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

bool GameManager::canBuyUnit(const QString &templateId) const
{
    const UnitTemplate unitTemplate = playerUnitTemplateById(templateId);
    return !unitTemplate.templateId.isEmpty()
        && m_playerGold >= unitTemplate.cost
        && ownedPlayerUnits().size() < m_board.benchCapacity()
        && m_board.canAddBenchUnit();
}

bool GameManager::buyUnit(const QString &templateId)
{
    if (!canBuyUnit(templateId)) {
        return false;
    }

    const UnitTemplate unitTemplate = playerUnitTemplateById(templateId);
    const QString instanceId = QStringLiteral("player_unit_%1_%2")
                                   .arg(unitTemplate.templateId)
                                   .arg(ownedPlayerUnits().size() + 1, 3, 10, QChar('0'));
    if (!m_board.addBenchUnit(unitTemplate.createUnit(instanceId, ControllerSide::PlayerCtrl))) {
        return false;
    }

    m_purchasedUnits.append({unitTemplate.templateId, instanceId});
    m_playerGold -= unitTemplate.cost;
    return true;
}

bool GameManager::canSellUnit(const QString &templateId) const
{
    const UnitTemplate unitTemplate = playerUnitTemplateById(templateId);
    if (unitTemplate.templateId.isEmpty()) {
        return false;
    }

    for (const PurchasedUnitRecord &record : m_purchasedUnits) {
        if (record.templateId == templateId) {
            return true;
        }
    }
    return false;
}

bool GameManager::sellUnit(const QString &templateId)
{
    if (!canSellUnit(templateId)) {
        return false;
    }

    const UnitTemplate unitTemplate = playerUnitTemplateById(templateId);
    QString unitIdToRemove;

    for (const PurchasedUnitRecord &record : m_purchasedUnits) {
        if (record.templateId == templateId) {
            unitIdToRemove = record.unitId;
            break;
        }
    }

    if (unitIdToRemove.isEmpty() || !removePlayerUnitById(unitIdToRemove)) {
        return false;
    }

    for (int index = 0; index < m_purchasedUnits.size(); ++index) {
        if (m_purchasedUnits.at(index).unitId == unitIdToRemove) {
            m_purchasedUnits.removeAt(index);
            break;
        }
    }

    m_playerGold += unitTemplate.cost;
    return true;
}

QVector<QString> GameManager::sellableUnitTemplateIds() const
{
    QVector<QString> templateIds;
    for (const UnitTemplate &unitTemplate : allPlayerUnitTemplates()) {
        if (canSellUnit(unitTemplate.templateId)) {
            templateIds.append(unitTemplate.templateId);
        }
    }
    return templateIds;
}

QVector<QString> GameManager::currentShopTemplateIds() const
{
    const_cast<GameManager *>(this)->ensureShopForCurrentRound();
    return m_shopTemplateIds;
}

int GameManager::shopRefreshCost() const
{
    return kShopRefreshCost;
}

bool GameManager::canRefreshShop() const
{
    return m_playerGold >= kShopRefreshCost;
}

bool GameManager::refreshShop()
{
    if (!canRefreshShop()) {
        return false;
    }

    const QVector<QString> previousShop = m_shopTemplateIds;
    m_playerGold -= kShopRefreshCost;
    m_shopTemplateIds = generateShopTemplateIds();

    if (m_shopTemplateIds == previousShop && !m_shopTemplateIds.isEmpty()) {
        const QVector<UnitTemplate> templates = allPlayerUnitTemplates();
        if (!templates.isEmpty()) {
            const int slot = QRandomGenerator::global()->bounded(m_shopTemplateIds.size());
            const int replacement = QRandomGenerator::global()->bounded(templates.size());
            m_shopTemplateIds[slot] = templates.at(replacement).templateId;
        }
    }

    m_shopRound = m_roundState.currentRound;
    return true;
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
    if (!canStartBattle()) {
        return;
    }

    capturePlayerBattleSnapshot();
    m_battleSimulator.beginBattle();
    m_roundState.beginBattlePhase();
}

void GameManager::tickBattleTimer()
{
    m_roundState.tickBattleTimer();
}

void GameManager::advanceBattleSimulationTick()
{
    if (m_roundState.phase != GamePhase::Battle || isBattleResolved()) {
        return;
    }

    m_battleSimulator.advanceTick(m_board);
}

bool GameManager::isBattleResolved() const
{
    return m_battleSimulator.isResolved(m_board);
}

bool GameManager::canStartBattle() const
{
    return m_roundState.phase == GamePhase::Deploy
        && m_board.activePlayerUnitCount() > 0
        && m_board.activeEnemyUnitCount() > 0;
}

QStringList GameManager::battleLog() const
{
    return m_battleSimulator.log();
}

BattleResult GameManager::calculateBattleResult()
{
    BattleResult result;
    const BattleOutcome outcome = m_battleSimulator.outcome(m_board);

    if (outcome.hasLivingPlayerUnits && !outcome.hasLivingEnemyUnits) {
        result.win = true;
    } else if (!outcome.hasLivingPlayerUnits && outcome.hasLivingEnemyUnits) {
        result.win = false;
    } else if (!outcome.hasLivingPlayerUnits && !outcome.hasLivingEnemyUnits) {
        result.win = false;
    } else {
        const int playerPower = combatPowerForSide(ControllerSide::PlayerCtrl);
        const int enemyPower = combatPowerForSide(ControllerSide::EnemyCtrl);
        result.win = playerPower >= enemyPower;
    }

    result.damage = result.win ? 0 : loseDamageForRound(m_roundState.currentRound);
    result.rewardGold = result.win ? winRewardForRound(m_roundState.currentRound) : 5;
    return result;
}

void GameManager::saveBattleResult(const BattleResult &result)
{
    m_lastBattleResult = result;
    m_playerGold += result.rewardGold;
    m_playerHp -= result.damage;
    if (m_playerHp < 0) {
        m_playerHp = 0;
    }
    restorePlayerBattleSnapshot();
    clearEnemyUnits();
    m_battleSimulator.reset();
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

void GameManager::clearEnemyUnits()
{
    for (int row = 0; row < m_board.rowCount(); ++row) {
        for (int col = 0; col < m_board.columnCount(); ++col) {
            const UnitPtr unit = m_board.unitAt(row, col);
            if (unit && unit->owner() == ControllerSide::EnemyCtrl) {
                m_board.removeUnit(row, col);
            }
        }
    }
}

void GameManager::loadCurrentEncounterFormation()
{
    clearEnemyUnits();

    const EnemyFormation formation = enemyFormationForId(m_currentEncounterInfo.formationId);
    for (int index = 0; index < formation.enemyUnits.size(); ++index) {
        const EnemyFormationUnit &formationUnit = formation.enemyUnits.at(index);
        m_board.placeUnit(formationUnit.unitTemplate.createUnit(
                              QStringLiteral("%1_enemy_%2")
                                  .arg(m_currentEncounterInfo.formationId)
                                  .arg(index + 1, 2, 10, QChar('0')),
                              ControllerSide::EnemyCtrl),
                          formationUnit.position.row,
                          formationUnit.position.col);
    }
}

void GameManager::capturePlayerBattleSnapshot()
{
    m_playerBattleSnapshots.clear();
    for (int row = 0; row < m_board.rowCount(); ++row) {
        for (int col = 0; col < m_board.columnCount(); ++col) {
            const UnitPtr unit = m_board.unitAt(row, col);
            if (unit && unit->owner() == ControllerSide::PlayerCtrl) {
                m_playerBattleSnapshots.append({unit, {row, col}, unit->hp(), unit->mana(), unit->state()});
            }
        }
    }
}

void GameManager::restorePlayerBattleSnapshot()
{
    if (m_playerBattleSnapshots.isEmpty()) {
        return;
    }

    clearPlayerUnitsFromBoard();
    for (const PlayerBattleSnapshot &snapshot : m_playerBattleSnapshots) {
        if (!snapshot.unit || !snapshot.position.isValid()) {
            continue;
        }
        snapshot.unit->setHp(snapshot.hp);
        snapshot.unit->setMana(snapshot.mana);
        snapshot.unit->setState(snapshot.state);
        m_board.placeUnit(snapshot.unit, snapshot.position.row, snapshot.position.col);
    }
    m_playerBattleSnapshots.clear();
}

void GameManager::clearPlayerUnitsFromBoard()
{
    for (int row = 0; row < m_board.rowCount(); ++row) {
        for (int col = 0; col < m_board.columnCount(); ++col) {
            const UnitPtr unit = m_board.unitAt(row, col);
            if (unit && unit->owner() == ControllerSide::PlayerCtrl) {
                m_board.removeUnit(row, col);
            }
        }
    }
}

void GameManager::ensureShopForCurrentRound()
{
    if (m_shopRound == m_roundState.currentRound && m_shopTemplateIds.size() == kShopSlotCount) {
        return;
    }

    m_shopTemplateIds = generateShopTemplateIds();
    m_shopRound = m_roundState.currentRound;
}

QVector<QString> GameManager::generateShopTemplateIds() const
{
    QVector<QString> templateIds;
    const QVector<UnitTemplate> templates = allPlayerUnitTemplates();
    if (templates.isEmpty()) {
        return templateIds;
    }

    templateIds.reserve(kShopSlotCount);
    for (int index = 0; index < kShopSlotCount; ++index) {
        const int templateIndex = QRandomGenerator::global()->bounded(templates.size());
        templateIds.append(templates.at(templateIndex).templateId);
    }
    return templateIds;
}

int GameManager::combatPowerForSide(ControllerSide side) const
{
    int power = 0;
    for (int row = 0; row < m_board.rowCount(); ++row) {
        for (int col = 0; col < m_board.columnCount(); ++col) {
            const UnitPtr unit = m_board.unitAt(row, col);
            if (unit && unit->owner() == side && unit->isAlive()) {
                power += unit->hp();
                power += unit->atk() * 8;
                power += unit->range() * 15;
            }
        }
    }
    return power;
}

bool GameManager::removePlayerUnitById(const QString &unitId)
{
    for (int slot = 0; slot < m_board.benchCapacity(); ++slot) {
        const UnitPtr unit = m_board.benchUnitAt(slot);
        if (unit && unit->owner() == ControllerSide::PlayerCtrl && unit->id() == unitId) {
            m_board.removeBenchUnit(slot);
            return true;
        }
    }

    for (int row = 0; row < m_board.rowCount(); ++row) {
        for (int col = 0; col < m_board.columnCount(); ++col) {
            const UnitPtr unit = m_board.unitAt(row, col);
            if (unit && unit->owner() == ControllerSide::PlayerCtrl && unit->id() == unitId) {
                m_board.removeUnit(row, col);
                return true;
            }
        }
    }
    return false;
}
