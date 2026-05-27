#ifndef UNITTEMPLATE_H
#define UNITTEMPLATE_H

#include "core/unit.h"

struct UnitTemplate
{
    QString templateId;
    QString displayName;
    QStringList traits;
    int cost = 0; // Reserved for future shop logic. No gold deduction is implemented yet.
    QString roleDescription;
    int maxHp = 100;
    int atk = 20;
    int range = 1;
    qreal attackSpeed = 1.0;
    int maxMana = 60;
    int manaGainOnAttack = 0;
    int manaGainOnHit = 0;
    QString skillName; // Reserved for future skill logic. Only static data is stored for now.
    QString skillDescription; // Reserved for future skill logic. No casting behavior is implemented.

    UnitPtr createUnit(const QString &instanceId, ControllerSide owner) const;
};

#endif // UNITTEMPLATE_H
