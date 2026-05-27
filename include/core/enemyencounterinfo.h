#ifndef ENEMYENCOUNTERINFO_H
#define ENEMYENCOUNTERINFO_H

#include <QString>
#include <QVector>

#include "core/unittemplate.h"

struct EnemyEncounterInfo
{
    QString formationId;
    int round = 1;
    QString difficultyLabel;
    QString styleLabel;
    QString styleDescription;
};

struct EnemyFormationUnit
{
    UnitTemplate unitTemplate;
    BoardPosition position;
};

struct EnemyFormation
{
    EnemyEncounterInfo info;
    QVector<EnemyFormationUnit> enemyUnits;
};

QVector<EnemyEncounterInfo> enemyEncounterPoolForRound(int round);
EnemyFormation enemyFormationForId(const QString &formationId);

#endif // ENEMYENCOUNTERINFO_H
