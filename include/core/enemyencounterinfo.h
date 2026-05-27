#ifndef ENEMYENCOUNTERINFO_H
#define ENEMYENCOUNTERINFO_H

#include <QString>
#include <QVector>

struct EnemyEncounterInfo
{
    QString formationId;
    int round = 1;
    QString difficultyLabel;
    QString styleLabel;
    QString styleDescription;
};

QVector<EnemyEncounterInfo> enemyEncounterPoolForRound(int round);

#endif // ENEMYENCOUNTERINFO_H
