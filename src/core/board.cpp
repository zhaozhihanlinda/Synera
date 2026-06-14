#include "core/board.h"

Board::Board()
{
    clear();
}

void Board::clear()
{
    for (auto &row : m_grid) {
        row.fill(nullptr);
    }
    m_bench.fill(nullptr);
}

int Board::rowCount() const
{
    return kDefaultRows;
}

int Board::columnCount() const
{
    return kDefaultCols;
}

int Board::benchCapacity() const
{
    return kDefaultBenchSlots;
}

bool Board::isInside(int row, int col) const
{
    return row >= 0 && row < rowCount() && col >= 0 && col < columnCount();
}

bool Board::isBenchSlotValid(int slot) const
{
    return slot >= 0 && slot < benchCapacity();
}

bool Board::isPlayerHalf(int row, int col) const
{
    return isInside(row, col) && row >= rowCount() / 2;
}

bool Board::isEnemyHalf(int row, int col) const
{
    return isInside(row, col) && row < rowCount() / 2;
}

bool Board::isDeployTileForSide(ControllerSide side, int row, int col) const
{
    if (side == ControllerSide::PlayerCtrl) {
        return isPlayerHalf(row, col);
    }
    return isEnemyHalf(row, col);
}

bool Board::isOccupied(int row, int col) const
{
    return unitAt(row, col) != nullptr;
}

UnitPtr Board::unitAt(int row, int col) const
{
    if (!isInside(row, col)) {
        return nullptr;
    }
    return m_grid[row][col];
}

UnitPtr Board::benchUnitAt(int slot) const
{
    if (!isBenchSlotValid(slot)) {
        return nullptr;
    }
    return m_bench[slot];
}

bool Board::canPlaceUnit(const UnitPtr &unit, int row, int col) const
{
    if (!unit || !isInside(row, col) || isOccupied(row, col)) {
        return false;
    }

    return isDeployTileForSide(unit->owner(), row, col);
}

bool Board::canMoveUnit(int fromRow, int fromCol, int toRow, int toCol) const
{
    const UnitPtr unit = unitAt(fromRow, fromCol);
    if (!unit || (fromRow == toRow && fromCol == toCol) || isOccupied(toRow, toCol)) {
        return false;
    }

    return canPlaceUnit(unit, toRow, toCol);
}

bool Board::canMoveUnitDuringBattle(int fromRow, int fromCol, int toRow, int toCol) const
{
    const UnitPtr unit = unitAt(fromRow, fromCol);
    return unit && isInside(toRow, toCol) && (fromRow != toRow || fromCol != toCol)
        && !isOccupied(toRow, toCol);
}

bool Board::canSwapUnits(int firstRow, int firstCol, int secondRow, int secondCol) const
{
    if (!isInside(firstRow, firstCol) || !isInside(secondRow, secondCol)
        || (firstRow == secondRow && firstCol == secondCol)) {
        return false;
    }

    const UnitPtr first = unitAt(firstRow, firstCol);
    const UnitPtr second = unitAt(secondRow, secondCol);
    if (!first || !second || first->owner() != second->owner()) {
        return false;
    }

    return isDeployTileForSide(first->owner(), secondRow, secondCol)
        && isDeployTileForSide(second->owner(), firstRow, firstCol);
}

bool Board::placeUnit(const UnitPtr &unit, int row, int col)
{
    if (!canPlaceUnit(unit, row, col)) {
        return false;
    }

    const BoardPosition oldPosition = unit->boardPosition();
    if (oldPosition.isValid() && isInside(oldPosition.row, oldPosition.col)
        && m_grid[oldPosition.row][oldPosition.col] == unit) {
        m_grid[oldPosition.row][oldPosition.col] = nullptr;
    }

    m_grid[row][col] = unit;
    unit->setBoardPosition({row, col});
    return true;
}

UnitPtr Board::removeUnit(int row, int col)
{
    if (!isInside(row, col) || !m_grid[row][col]) {
        return nullptr;
    }

    UnitPtr unit = m_grid[row][col];
    m_grid[row][col] = nullptr;
    unit->clearBoardPosition();
    return unit;
}

bool Board::moveUnit(int fromRow, int fromCol, int toRow, int toCol)
{
    UnitPtr unit = unitAt(fromRow, fromCol);
    if (!unit || !canMoveUnit(fromRow, fromCol, toRow, toCol)) {
        return false;
    }

    m_grid[fromRow][fromCol] = nullptr;
    m_grid[toRow][toCol] = unit;
    unit->setBoardPosition({toRow, toCol});
    return true;
}

bool Board::moveUnitDuringBattle(int fromRow, int fromCol, int toRow, int toCol)
{
    UnitPtr unit = unitAt(fromRow, fromCol);
    if (!unit || !canMoveUnitDuringBattle(fromRow, fromCol, toRow, toCol)) {
        return false;
    }

    m_grid[fromRow][fromCol] = nullptr;
    m_grid[toRow][toCol] = unit;
    unit->setBoardPosition({toRow, toCol});
    return true;
}

bool Board::swapUnits(int firstRow, int firstCol, int secondRow, int secondCol)
{
    if (!canSwapUnits(firstRow, firstCol, secondRow, secondCol)) {
        return false;
    }

    UnitPtr first = m_grid[firstRow][firstCol];
    UnitPtr second = m_grid[secondRow][secondCol];
    m_grid[firstRow][firstCol] = second;
    m_grid[secondRow][secondCol] = first;
    first->setBoardPosition({secondRow, secondCol});
    second->setBoardPosition({firstRow, firstCol});
    return true;
}

bool Board::relocateOrSwapUnit(int fromRow, int fromCol, int toRow, int toCol)
{
    if (canMoveUnit(fromRow, fromCol, toRow, toCol)) {
        return moveUnit(fromRow, fromCol, toRow, toCol);
    }
    if (canSwapUnits(fromRow, fromCol, toRow, toCol)) {
        return swapUnits(fromRow, fromCol, toRow, toCol);
    }
    return false;
}

bool Board::canAddBenchUnit() const
{
    return firstEmptyBenchSlot() != -1;
}

int Board::firstEmptyBenchSlot() const
{
    for (int index = 0; index < benchCapacity(); ++index) {
        if (!m_bench[index]) {
            return index;
        }
    }
    return -1;
}

bool Board::addBenchUnit(const UnitPtr &unit)
{
    const int slot = firstEmptyBenchSlot();
    if (!unit || slot == -1) {
        return false;
    }
    return setBenchUnit(slot, unit);
}

bool Board::setBenchUnit(int slot, const UnitPtr &unit)
{
    if (!isBenchSlotValid(slot) || m_bench[slot] || !unit) {
        return false;
    }

    const BoardPosition oldPosition = unit->boardPosition();
    if (oldPosition.isValid() && isInside(oldPosition.row, oldPosition.col)
        && m_grid[oldPosition.row][oldPosition.col] == unit) {
        m_grid[oldPosition.row][oldPosition.col] = nullptr;
        unit->clearBoardPosition();
    }

    m_bench[slot] = unit;
    return true;
}

UnitPtr Board::removeBenchUnit(int slot)
{
    if (!isBenchSlotValid(slot) || !m_bench[slot]) {
        return nullptr;
    }

    UnitPtr unit = m_bench[slot];
    m_bench[slot] = nullptr;
    return unit;
}

bool Board::moveBenchToBoard(int slot, int row, int col)
{
    UnitPtr unit = benchUnitAt(slot);
    if (!unit || !canPlaceUnit(unit, row, col)) {
        return false;
    }

    m_bench[slot] = nullptr;
    m_grid[row][col] = unit;
    unit->setBoardPosition({row, col});
    return true;
}

bool Board::canMoveBoardToBenchSlot(int row, int col, int slot) const
{
    return isBenchSlotValid(slot) && !m_bench[slot] && unitAt(row, col) != nullptr;
}

bool Board::moveBoardToBenchSlot(int row, int col, int slot)
{
    if (!canMoveBoardToBenchSlot(row, col, slot)) {
        return false;
    }

    UnitPtr unit = removeUnit(row, col);
    if (!unit) {
        return false;
    }

    m_bench[slot] = unit;
    return true;
}

bool Board::canMoveBoardToBench(int row, int col) const
{
    return canAddBenchUnit() && unitAt(row, col) != nullptr;
}

bool Board::moveBoardToBench(int row, int col)
{
    if (!canMoveBoardToBench(row, col)) {
        return false;
    }

    UnitPtr unit = removeUnit(row, col);
    if (!unit) {
        return false;
    }

    const int slot = firstEmptyBenchSlot();
    m_bench[slot] = unit;
    return true;
}

int Board::activePlayerUnitCount() const
{
    int count = 0;
    for (const auto &row : m_grid) {
        for (const auto &unit : row) {
            if (unit && unit->owner() == ControllerSide::PlayerCtrl) {
                ++count;
            }
        }
    }
    return count;
}

int Board::activeEnemyUnitCount() const
{
    int count = 0;
    for (const auto &row : m_grid) {
        for (const auto &unit : row) {
            if (unit && unit->owner() == ControllerSide::EnemyCtrl) {
                ++count;
            }
        }
    }
    return count;
}

int Board::totalHpForSide(ControllerSide side) const
{
    int total = 0;
    for (const auto &row : m_grid) {
        for (const auto &unit : row) {
            if (unit && unit->owner() == side) {
                total += unit->hp();
            }
        }
    }
    return total;
}
