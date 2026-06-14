#ifndef BOARD_H
#define BOARD_H

#include <array>

#include "core/unit.h"

class Board
{
public:
    static constexpr int kDefaultRows = 8;
    static constexpr int kDefaultCols = 8;
    static constexpr int kDefaultBenchSlots = 8;

    Board();

    void clear();

    int rowCount() const;
    int columnCount() const;
    int benchCapacity() const;

    bool isInside(int row, int col) const;
    bool isBenchSlotValid(int slot) const;
    bool isPlayerHalf(int row, int col) const;
    bool isEnemyHalf(int row, int col) const;
    bool isDeployTileForSide(ControllerSide side, int row, int col) const;
    bool isOccupied(int row, int col) const;

    UnitPtr unitAt(int row, int col) const;
    UnitPtr benchUnitAt(int slot) const;

    bool canPlaceUnit(const UnitPtr &unit, int row, int col) const;
    bool canMoveUnit(int fromRow, int fromCol, int toRow, int toCol) const;
    bool canMoveUnitDuringBattle(int fromRow, int fromCol, int toRow, int toCol) const;
    bool canSwapUnits(int firstRow, int firstCol, int secondRow, int secondCol) const;
    bool placeUnit(const UnitPtr &unit, int row, int col);
    UnitPtr removeUnit(int row, int col);
    bool moveUnit(int fromRow, int fromCol, int toRow, int toCol);
    bool moveUnitDuringBattle(int fromRow, int fromCol, int toRow, int toCol);
    bool swapUnits(int firstRow, int firstCol, int secondRow, int secondCol);
    bool relocateOrSwapUnit(int fromRow, int fromCol, int toRow, int toCol);

    bool canAddBenchUnit() const;
    int firstEmptyBenchSlot() const;
    bool addBenchUnit(const UnitPtr &unit);
    bool setBenchUnit(int slot, const UnitPtr &unit);
    UnitPtr removeBenchUnit(int slot);

    bool moveBenchToBoard(int slot, int row, int col);
    bool canMoveBoardToBenchSlot(int row, int col, int slot) const;
    bool moveBoardToBenchSlot(int row, int col, int slot);
    bool canMoveBoardToBench(int row, int col) const;
    bool moveBoardToBench(int row, int col);

    int activePlayerUnitCount() const;
    int activeEnemyUnitCount() const;
    int totalHpForSide(ControllerSide side) const;

private:
    using GridRow = std::array<UnitPtr, kDefaultCols>;
    using Grid = std::array<GridRow, kDefaultRows>;
    using Bench = std::array<UnitPtr, kDefaultBenchSlots>;

    Grid m_grid;
    Bench m_bench;
};

#endif // BOARD_H
