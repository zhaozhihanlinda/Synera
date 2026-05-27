#include "core/unittemplate.h"

UnitPtr UnitTemplate::createUnit(const QString &instanceId, ControllerSide owner) const
{
    auto unit = std::make_shared<Unit>(instanceId, displayName, owner);
    unit->setTraits(traits);
    unit->setCost(cost);
    unit->setRoleDescription(roleDescription);
    unit->setSkillName(skillName);
    unit->setSkillDescription(skillDescription);
    unit->setEquipmentDescriptions(equipmentDescriptions);
    unit->setMaxHp(maxHp);
    unit->setHp(maxHp);
    unit->setAtk(atk);
    unit->setRange(range);
    unit->setAttackSpeed(attackSpeed);
    unit->setMaxMana(maxMana);
    unit->setManaGainOnAttack(manaGainOnAttack);
    unit->setManaGainOnHit(manaGainOnHit);
    unit->setMana(0);
    return unit;
}
