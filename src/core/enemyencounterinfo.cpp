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

struct EnemyTemplateSet
{
    UnitTemplate guard;
    UnitTemplate striker;
    UnitTemplate archer;
    UnitTemplate adept;
};

EnemyTemplateSet createEnemyTemplatesForRound(int round)
{
    switch (round) {
    case 1:
        return {
            createEnemyTemplate(QStringLiteral("enemy_guard_r1"), QStringLiteral("敌方卫兵"), {QStringLiteral("前排")}, 350, 35, 1, 1.0),
            createEnemyTemplate(QStringLiteral("enemy_striker_r1"), QStringLiteral("敌方突击手"), {QStringLiteral("近战")}, 300, 45, 1, 1.1),
            createEnemyTemplate(QStringLiteral("enemy_archer_r1"), QStringLiteral("敌方射手"), {QStringLiteral("远程")}, 240, 42, 4, 1.4),
            createEnemyTemplate(QStringLiteral("enemy_adept_r1"), QStringLiteral("敌方术士"), {QStringLiteral("控制")}, 260, 36, 3, 0.9)
        };
    case 2:
        return {
            createEnemyTemplate(QStringLiteral("enemy_guard_r2"), QStringLiteral("敌方卫兵"), {QStringLiteral("前排")}, 500, 48, 1, 1.0),
            createEnemyTemplate(QStringLiteral("enemy_striker_r2"), QStringLiteral("敌方突击手"), {QStringLiteral("近战")}, 420, 64, 1, 1.1),
            createEnemyTemplate(QStringLiteral("enemy_archer_r2"), QStringLiteral("敌方射手"), {QStringLiteral("远程")}, 330, 58, 4, 1.4),
            createEnemyTemplate(QStringLiteral("enemy_adept_r2"), QStringLiteral("敌方术士"), {QStringLiteral("控制")}, 360, 50, 3, 0.9)
        };
    case 3:
        return {
            createEnemyTemplate(QStringLiteral("enemy_guard_r3"), QStringLiteral("敌方卫兵"), {QStringLiteral("前排")}, 680, 62, 1, 1.0),
            createEnemyTemplate(QStringLiteral("enemy_striker_r3"), QStringLiteral("敌方突击手"), {QStringLiteral("近战")}, 560, 82, 1, 1.1),
            createEnemyTemplate(QStringLiteral("enemy_archer_r3"), QStringLiteral("敌方射手"), {QStringLiteral("远程")}, 430, 76, 4, 1.4),
            createEnemyTemplate(QStringLiteral("enemy_adept_r3"), QStringLiteral("敌方术士"), {QStringLiteral("控制")}, 470, 66, 3, 0.9)
        };
    case 4:
        return {
            createEnemyTemplate(QStringLiteral("enemy_guard_r4"), QStringLiteral("敌方卫兵"), {QStringLiteral("前排")}, 850, 78, 1, 1.0),
            createEnemyTemplate(QStringLiteral("enemy_striker_r4"), QStringLiteral("敌方突击手"), {QStringLiteral("近战")}, 720, 104, 1, 1.1),
            createEnemyTemplate(QStringLiteral("enemy_archer_r4"), QStringLiteral("敌方射手"), {QStringLiteral("远程")}, 540, 96, 4, 1.4),
            createEnemyTemplate(QStringLiteral("enemy_adept_r4"), QStringLiteral("敌方术士"), {QStringLiteral("控制")}, 600, 82, 3, 0.9)
        };
    default:
        return {
            createEnemyTemplate(QStringLiteral("enemy_guard_r5"), QStringLiteral("敌方卫兵"), {QStringLiteral("前排")}, 1050, 95, 1, 1.0),
            createEnemyTemplate(QStringLiteral("enemy_striker_r5"), QStringLiteral("敌方突击手"), {QStringLiteral("近战")}, 900, 126, 1, 1.1),
            createEnemyTemplate(QStringLiteral("enemy_archer_r5"), QStringLiteral("敌方射手"), {QStringLiteral("远程")}, 660, 118, 4, 1.4),
            createEnemyTemplate(QStringLiteral("enemy_adept_r5"), QStringLiteral("敌方术士"), {QStringLiteral("控制")}, 730, 100, 3, 0.9)
        };
    }
}

QVector<EnemyFormation> allEnemyFormations()
{
    const EnemyTemplateSet r1 = createEnemyTemplatesForRound(1);
    const EnemyTemplateSet r2 = createEnemyTemplatesForRound(2);
    const EnemyTemplateSet r3 = createEnemyTemplatesForRound(3);
    const EnemyTemplateSet r4 = createEnemyTemplatesForRound(4);
    const EnemyTemplateSet r5 = createEnemyTemplatesForRound(5);

    return {
        {{QStringLiteral("r1_frontline"), 1, QStringLiteral("I"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军以简单前排推进为主，正面承压会更明显。")},
         {{r1.guard, {1, 3}}, {r1.guard, {1, 4}}}},
        {{QStringLiteral("r1_backline"), 1, QStringLiteral("I"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军依赖基础后排火力，需注意后排承伤。")},
         {{r1.guard, {1, 3}}, {r1.archer, {0, 4}}}},
        {{QStringLiteral("r1_control"), 1, QStringLiteral("I"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军混入基础辅助与干扰，阵型应保持平衡。")},
         {{r1.guard, {1, 4}}, {r1.adept, {0, 3}}}},

        {{QStringLiteral("r2_frontline"), 2, QStringLiteral("II"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军前后排开始配合，前排推进会更持续。")},
         {{r2.guard, {1, 2}}, {r2.guard, {1, 4}}, {r2.striker, {0, 3}}}},
        {{QStringLiteral("r2_backline"), 2, QStringLiteral("II"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军后排输出更稳定，需尽量缩短承火时间。")},
         {{r2.guard, {1, 3}}, {r2.archer, {0, 2}}, {r2.archer, {0, 5}}}},
        {{QStringLiteral("r2_control"), 2, QStringLiteral("II"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军协同更明确，阵容平衡性会更重要。")},
         {{r2.guard, {1, 3}}, {r2.adept, {0, 4}}, {r2.striker, {1, 5}}}},

        {{QStringLiteral("r3_frontline"), 3, QStringLiteral("III"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军前排压制更强，单薄阵线会更容易崩溃。")},
         {{r3.guard, {1, 2}}, {r3.guard, {1, 3}}, {r3.guard, {1, 4}}, {r3.striker, {0, 4}}}},
        {{QStringLiteral("r3_backline"), 3, QStringLiteral("III"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军开始拥有高攻速或范围火力，后排压力提升。")},
         {{r3.guard, {1, 3}}, {r3.striker, {1, 4}}, {r3.archer, {0, 2}}, {r3.archer, {0, 5}}}},
        {{QStringLiteral("r3_control"), 3, QStringLiteral("III"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军干扰能力提升，部署时要考虑续航与容错。")},
         {{r3.guard, {1, 2}}, {r3.adept, {0, 3}}, {r3.adept, {0, 5}}, {r3.striker, {1, 5}}}},

        {{QStringLiteral("r4_frontline"), 4, QStringLiteral("IV"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军形成明显正面压制，前排站位选择很关键。")},
         {{r4.guard, {1, 1}}, {r4.guard, {1, 3}}, {r4.guard, {1, 5}}, {r4.striker, {0, 3}}, {r4.striker, {0, 4}}}},
        {{QStringLiteral("r4_backline"), 4, QStringLiteral("IV"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军火力密度更高，需要更主动的反后排思路。")},
         {{r4.guard, {1, 2}}, {r4.guard, {1, 5}}, {r4.archer, {0, 1}}, {r4.archer, {0, 3}}, {r4.adept, {0, 5}}}},
        {{QStringLiteral("r4_control"), 4, QStringLiteral("IV"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军配合会制造更强站位陷阱，需谨慎分配人口。")},
         {{r4.guard, {1, 2}}, {r4.striker, {1, 4}}, {r4.adept, {0, 2}}, {r4.adept, {0, 4}}, {r4.archer, {0, 6}}}},

        {{QStringLiteral("r5_frontline"), 5, QStringLiteral("V"), QStringLiteral("Frontline Push"),
          QStringLiteral("敌军正面压迫达到最高等级，考验完整前排体系。")},
         {{r5.guard, {1, 1}}, {r5.guard, {1, 3}}, {r5.guard, {1, 5}}, {r5.striker, {0, 2}}, {r5.striker, {0, 4}}, {r5.adept, {0, 6}}}},
        {{QStringLiteral("r5_backline"), 5, QStringLiteral("V"), QStringLiteral("Backline Ranged Output"),
          QStringLiteral("敌军后排火力完整成型，必须重视切入或抗压。")},
         {{r5.guard, {1, 2}}, {r5.guard, {1, 5}}, {r5.striker, {1, 3}}, {r5.archer, {0, 1}}, {r5.archer, {0, 4}}, {r5.adept, {0, 6}}}},
        {{QStringLiteral("r5_control"), 5, QStringLiteral("V"), QStringLiteral("Control / Support Mixed"),
          QStringLiteral("敌军拥有完整协同，是最终轮最全面的战术检验。")},
         {{r5.guard, {1, 2}}, {r5.striker, {1, 4}}, {r5.archer, {0, 1}}, {r5.adept, {0, 3}}, {r5.adept, {0, 5}}, {r5.archer, {0, 6}}}}
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
