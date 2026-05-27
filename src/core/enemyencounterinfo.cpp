#include "core/enemyencounterinfo.h"

namespace {

UnitTemplate createEnemyTemplate(const QString &templateId,
                                 const QString &displayName,
                                 const QStringList &traits,
                                 int maxHp,
                                 int atk,
                                 int range,
                                 qreal attackSpeed)
{
    UnitTemplate unitTemplate;
    unitTemplate.templateId = templateId;
    unitTemplate.displayName = displayName;
    unitTemplate.traits = traits;
    unitTemplate.roleDescription = QStringLiteral("敌方固定阵容单位");
    unitTemplate.maxHp = maxHp;
    unitTemplate.atk = atk;
    unitTemplate.range = range;
    unitTemplate.attackSpeed = attackSpeed;
    unitTemplate.maxMana = 60;
    unitTemplate.manaGainOnAttack = 10;
    unitTemplate.manaGainOnHit = 5;
    return unitTemplate;
}

QVector<EnemyFormation> allEnemyFormations()
{
    const UnitTemplate guard = createEnemyTemplate(QStringLiteral("enemy_guard"),
                                                   QStringLiteral("敌方卫兵"),
                                                   {QStringLiteral("前排")},
                                                   300,
                                                   28,
                                                   1,
                                                   1.0);
    const UnitTemplate striker = createEnemyTemplate(QStringLiteral("enemy_striker"),
                                                     QStringLiteral("敌方突击手"),
                                                     {QStringLiteral("近战")},
                                                     240,
                                                     38,
                                                     1,
                                                     1.1);
    const UnitTemplate archer = createEnemyTemplate(QStringLiteral("enemy_archer"),
                                                    QStringLiteral("敌方射手"),
                                                    {QStringLiteral("远程")},
                                                    190,
                                                    36,
                                                    4,
                                                    1.4);
    const UnitTemplate adept = createEnemyTemplate(QStringLiteral("enemy_adept"),
                                                   QStringLiteral("敌方术士"),
                                                   {QStringLiteral("控制")},
                                                   210,
                                                   30,
                                                   3,
                                                   0.9);

    return {
        {{QStringLiteral("r1_frontline"), 1, QStringLiteral("I"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军以简单前排推进为主，正面承压会更明显。")},
         {{guard, {1, 3}}, {guard, {1, 4}}}},
        {{QStringLiteral("r1_backline"), 1, QStringLiteral("I"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军依赖基础后排火力，需注意后排承伤。")},
         {{guard, {1, 3}}, {archer, {0, 4}}}},
        {{QStringLiteral("r1_control"), 1, QStringLiteral("I"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军混入基础辅助与干扰，阵型应保持平衡。")},
         {{guard, {1, 4}}, {adept, {0, 3}}}},

        {{QStringLiteral("r2_frontline"), 2, QStringLiteral("II"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军前后排开始配合，前排推进会更持续。")},
         {{guard, {1, 2}}, {guard, {1, 4}}, {striker, {0, 3}}}},
        {{QStringLiteral("r2_backline"), 2, QStringLiteral("II"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军后排输出更稳定，需尽量缩短承火时间。")},
         {{guard, {1, 3}}, {archer, {0, 2}}, {archer, {0, 5}}}},
        {{QStringLiteral("r2_control"), 2, QStringLiteral("II"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军协同更明确，阵容平衡性会更重要。")},
         {{guard, {1, 3}}, {adept, {0, 4}}, {striker, {1, 5}}}},

        {{QStringLiteral("r3_frontline"), 3, QStringLiteral("III"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军前排压制更强，单薄阵线会更容易崩溃。")},
         {{guard, {1, 2}}, {guard, {1, 3}}, {guard, {1, 4}}, {striker, {0, 4}}}},
        {{QStringLiteral("r3_backline"), 3, QStringLiteral("III"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军开始拥有高攻速或范围火力，后排压力提升。")},
         {{guard, {1, 3}}, {striker, {1, 4}}, {archer, {0, 2}}, {archer, {0, 5}}}},
        {{QStringLiteral("r3_control"), 3, QStringLiteral("III"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军干扰能力提升，部署时要考虑续航与容错。")},
         {{guard, {1, 2}}, {adept, {0, 3}}, {adept, {0, 5}}, {striker, {1, 5}}}},

        {{QStringLiteral("r4_frontline"), 4, QStringLiteral("IV"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军形成明显正面压制，前排站位选择很关键。")},
         {{guard, {1, 1}}, {guard, {1, 3}}, {guard, {1, 5}}, {striker, {0, 3}}, {striker, {0, 4}}}},
        {{QStringLiteral("r4_backline"), 4, QStringLiteral("IV"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军火力密度更高，需要更主动的反后排思路。")},
         {{guard, {1, 2}}, {guard, {1, 5}}, {archer, {0, 1}}, {archer, {0, 3}}, {adept, {0, 5}}}},
        {{QStringLiteral("r4_control"), 4, QStringLiteral("IV"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军配合会制造更强站位陷阱，需谨慎分配人口。")},
         {{guard, {1, 2}}, {striker, {1, 4}}, {adept, {0, 2}}, {adept, {0, 4}}, {archer, {0, 6}}}},

        {{QStringLiteral("r5_frontline"), 5, QStringLiteral("V"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军正面压迫达到最高等级，考验完整前排体系。")},
         {{guard, {1, 1}}, {guard, {1, 3}}, {guard, {1, 5}}, {striker, {0, 2}}, {striker, {0, 4}}, {adept, {0, 6}}}},
        {{QStringLiteral("r5_backline"), 5, QStringLiteral("V"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军后排火力完整成型，必须重视切入或抗压。")},
         {{guard, {1, 2}}, {guard, {1, 5}}, {striker, {1, 3}}, {archer, {0, 1}}, {archer, {0, 4}}, {adept, {0, 6}}}},
        {{QStringLiteral("r5_control"), 5, QStringLiteral("V"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军拥有完整协同，是最终轮最全面的战术检验。")},
         {{guard, {1, 2}}, {striker, {1, 4}}, {archer, {0, 1}}, {adept, {0, 3}}, {adept, {0, 5}}, {archer, {0, 6}}}}
    };
}

}

QVector<EnemyEncounterInfo> enemyEncounterPoolForRound(int round)
{
    QVector<EnemyEncounterInfo> pool;
    for (const EnemyFormation &formation : allEnemyFormations()) {
        if (formation.info.round == round) {
            pool.append(formation.info);
        }
    }
    return pool;
}

EnemyFormation enemyFormationForId(const QString &formationId)
{
    for (const EnemyFormation &formation : allEnemyFormations()) {
        if (formation.info.formationId == formationId) {
            return formation;
        }
    }
    return {};
}
