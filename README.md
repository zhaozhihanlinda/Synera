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
- `equipmentDescriptions`

These fields are intentionally reserved so the design data is not lost, but they do not enable any gameplay systems yet.

Not implemented yet:

- gold deduction or purchase behavior
- equipment assignment or stat modification
- skill casting behavior or skill resolution
