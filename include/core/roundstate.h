#ifndef ROUNDSTATE_H
#define ROUNDSTATE_H

enum class GamePhase {
    Deploy,
    Battle
};

struct RoundState
{
    int currentRound = 1;
    int finalRound = 3;
    GamePhase phase = GamePhase::Deploy;
    bool deploymentLocked = false;
    bool boardInteractionLocked = false;
    int battleElapsedSeconds = 0;

    bool canDeploy() const
    {
        return phase == GamePhase::Deploy && !deploymentLocked && !boardInteractionLocked;
    }

    bool canInteractWithBoard() const
    {
        return !boardInteractionLocked;
    }

    void beginDeployPhase(int round)
    {
        currentRound = round;
        phase = GamePhase::Deploy;
        deploymentLocked = false;
        boardInteractionLocked = false;
        battleElapsedSeconds = 0;
    }

    void beginBattlePhase()
    {
        phase = GamePhase::Battle;
        deploymentLocked = true;
        boardInteractionLocked = true;
        battleElapsedSeconds = 0;
    }

    void tickBattleTimer()
    {
        if (phase == GamePhase::Battle) {
            ++battleElapsedSeconds;
        }
    }
};

#endif // ROUNDSTATE_H
