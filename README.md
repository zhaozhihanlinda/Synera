# Synera

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

## Current Placeholder Flow

The current build focuses on a complete playable loop with temporary data:

1. Start page -> profile page -> rule page -> initial info page.
2. Each round draws one temporary enemy encounter.
3. The player may enter the shop or go directly to deployment.
4. Deployment uses the 8 x 8 board and the bench.
5. Battle currently stays on the battle page briefly, then resolves through placeholder power comparison.
6. Round result applies temporary win rewards and lose damage.
7. The game advances to the next round, victory page, or defeat page.

The battle placeholder does not implement real movement, attacks, skill casting, or death-by-combat yet. It only keeps the full game flow runnable.

Not implemented yet:

- skill casting behavior or skill resolution
- real unit movement, attacks, and death resolution
