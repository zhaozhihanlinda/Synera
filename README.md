# Synera

Synera is a Qt Widgets / C++ auto-battler prototype. The current milestone is to
keep one complete PvE game loop runnable with temporary data before expanding
combat depth.

## Build and Run

From the project root:

```sh
cmake --build build
build/Synera.app/Contents/MacOS/Synera
```

The project is configured through `CMakeLists.txt` and links against Qt
Widgets.

## Project Structure

- `src/main.cpp`: application entry point.
- `include/core`, `src/core`: game data, board rules, player state, round state,
  enemy encounter data, and page routing helpers.
- `include/pages`, `src/pages`: Qt pages and page-level UI flow.
- `include/widgets`, `src/widgets`: reusable board, bench, and battle info
  widgets.

## Core Classes

- `GameManager`: owns the current game state, player resources, board, round
  state, shop buy/sell behavior, minimal automatic battle simulation, and
  settlement.
- `Board`: owns the 8 x 8 grid and 8-slot bench, including deployment-side
  restrictions and board/bench movement.
- `Unit` and `UnitTemplate`: represent unit runtime data and static unit design
  data.
- `EnemyEncounterInfo`: provides temporary fixed enemy encounter options and
  formations.
- `PageManager` and `MainWindow`: register pages and connect the current
  playable flow.

## Current Implemented Flow

1. Start page.
2. Profile page with randomized identity.
3. Initial info page.
4. Enemy draw page.
5. Shop or direct deployment.
6. Main game deployment on an 8 x 8 board.
7. Minimal automatic battle simulation and settlement.
8. Round result.
9. Next round, victory page, or defeat page.

## Current Rules and Data

- Board: fixed 8 x 8 grid.
- Bench: fixed 8 slots.
- Deployment: player units can only be deployed on the player half; enemy units
  are loaded into the enemy half by the selected encounter.
- Rounds: one run contains 5 rounds.
- Player HP: starts at 100 and does not recover automatically.
- Shop: uses the current static player unit template list and deducts/refunds
  gold through `GameManager`.
- Enemy encounters: temporary fixed formations are grouped by round and style.
- Battle: currently uses a minimal tick-based automatic simulation. Units choose
  the nearest living enemy, attack when in range, move one tile toward the target
  when out of range, and are removed from the board when defeated.
- Settlement: wins grant the current temporary round rewards; losses grant a
  small reward and deduct round-based HP.

## AI Usage

AI assistance was used for planning, risk checks, documentation wording, and
small implementation guidance. All code and document changes were reviewed in
the local project context before being kept. The current implementation keeps
AI-assisted work constrained to the playable-loop milestone and does not add
future systems such as full combat AI, skill resolution, buffs, equipment,
save/load, or event buses.

## Development Rule

Current implementation priority:

1. First make it playable.
2. Then make it fun.
3. Then make it look better.
4. Then make it more extensible.

Execution constraints:

1. Do not implement full systems that have not entered the current stage yet.
2. Prioritize a minimal runnable feature loop for the current task.
3. Small structural reservations are allowed only when they are necessary for near-term expansion.
4. If a structural reservation is added, explain:
   - why it must be reserved now
   - what future cost appears if it is not reserved now
5. Do not preemptively implement complex logic.
6. Empty interfaces, fields, or placeholder functions are allowed, but avoid over-engineering.

Do not expand these systems in advance unless explicitly requested and confirmed:

- skill system
- buff system
- equipment system
- AI behavior tree
- particle effects
- multithreading
- event bus
- JSON config system
- save/load system

If a requested feature clearly goes beyond the current stage, first explain:

- why it exceeds the current stage
- what the implementation cost or risk is
- what the smaller current-stage alternative is

Then wait for a decision before proceeding.

## Reserved Static Data

The current player unit templates may already contain static fields for:

- `cost`
- `skillName`
- `skillDescription`

These fields are intentionally reserved so the design data is not lost, but they do not enable any gameplay systems yet.

- skill casting behavior or skill resolution
- advanced targeting AI, projectiles, pathfinding, or animation timing
