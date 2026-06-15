#include "core/enemyencounterinfo.h"

namespace {

UnitTemplate createEnemyTemplate(const QString &templateId,
                                 const QString &displayName,
                                 const QStringList &traits,
                                 const QString &roleDescription,
                                 int maxHp,
                                 int atk,
                                 int range,
                                 qreal attackSpeed,
                                 int maxMana,
                                 int manaGainOnAttack,
                                 int manaGainOnHit,
                                 const QString &skillName,
                                 const QString &skillDescription)
{
    UnitTemplate unitTemplate;
    unitTemplate.templateId = templateId;
    unitTemplate.displayName = displayName;
    unitTemplate.traits = traits;
    unitTemplate.roleDescription = roleDescription;
    unitTemplate.maxHp = maxHp;
    unitTemplate.atk = atk;
    unitTemplate.range = range;
    unitTemplate.attackSpeed = attackSpeed;
    unitTemplate.maxMana = maxMana;
    unitTemplate.manaGainOnAttack = manaGainOnAttack;
    unitTemplate.manaGainOnHit = manaGainOnHit;
    unitTemplate.skillName = skillName;
    unitTemplate.skillDescription = skillDescription;
    return unitTemplate;
}

struct EnemyTemplateSet
{
    UnitTemplate ironWall;
    UnitTemplate charger;
    UnitTemplate crusher;
    UnitTemplate blackBow;
    UnitTemplate pyro;
    UnitTemplate frost;
    UnitTemplate plague;
    UnitTemplate shadow;
    UnitTemplate ballista;
    UnitTemplate warlord;
};

EnemyTemplateSet createEnemyTemplatesForRound(int round)
{
    const int r = qBound(1, round, 5);
    return {
        createEnemyTemplate(QStringLiteral("enemy_iron_wall_r%1").arg(r), QStringLiteral("铁壁守卫"),
                            {QStringLiteral("前排"), QStringLiteral("防御")},
                            QStringLiteral("近距离防御型，用于吸收伤害与阻挡推进。"),
                            520 + r * 120, 32 + r * 8, 1, 0.8, 100, 8, 10,
                            QStringLiteral("钢铁壁垒"),
                            QStringLiteral("受到伤害降低30%，持续5秒。当前版本仅展示说明。")),
        createEnemyTemplate(QStringLiteral("enemy_charger_r%1").arg(r), QStringLiteral("冲锋狂战"),
                            {QStringLiteral("近战"), QStringLiteral("突进")},
                            QStringLiteral("高速近战突进单位，擅长制造后排压力。"),
                            420 + r * 90, 55 + r * 12, 2, 1.3, 70, 14, 5,
                            QStringLiteral("野蛮冲撞"),
                            QStringLiteral("冲向距离最远敌人，造成180%攻击力伤害并击退目标。当前版本仅展示说明。")),
        createEnemyTemplate(QStringLiteral("enemy_crusher_r%1").arg(r), QStringLiteral("碎甲巨兽"),
                            {QStringLiteral("前排"), QStringLiteral("破甲")},
                            QStringLiteral("重型破甲单位，专门针对前排。"),
                            500 + r * 100, 70 + r * 14, 1, 0.9, 90, 12, 8,
                            QStringLiteral("破甲重击"),
                            QStringLiteral("对当前目标造成250%攻击力伤害，并降低20%防御。当前版本仅展示说明。")),
        createEnemyTemplate(QStringLiteral("enemy_black_bow_r%1").arg(r), QStringLiteral("黑羽弩手"),
                            {QStringLiteral("远程"), QStringLiteral("持续输出")},
                            QStringLiteral("超远程持续输出单位，高攻速压制。"),
                            280 + r * 55, 48 + r * 9, 6, 1.7, 80, 10, 4,
                            QStringLiteral("毒箭齐射"),
                            QStringLiteral("连续射出3支毒箭并附加持续伤害。当前版本仅展示说明。")),
        createEnemyTemplate(QStringLiteral("enemy_pyro_r%1").arg(r), QStringLiteral("爆炎术士"),
                            {QStringLiteral("远程"), QStringLiteral("范围")},
                            QStringLiteral("远程范围爆发单位，擅长惩罚聚团站位。"),
                            320 + r * 60, 70 + r * 13, 4, 0.9, 120, 16, 5,
                            QStringLiteral("地狱火雨"),
                            QStringLiteral("对目标区域造成260%攻击力范围伤害。当前版本仅展示说明。")),
        createEnemyTemplate(QStringLiteral("enemy_frost_r%1").arg(r), QStringLiteral("寒霜祭司"),
                            {QStringLiteral("远程"), QStringLiteral("控制")},
                            QStringLiteral("远程控制辅助单位，降低敌方攻速。"),
                            340 + r * 60, 42 + r * 8, 4, 0.8, 100, 15, 5,
                            QStringLiteral("冰霜领域"),
                            QStringLiteral("范围内敌军攻击速度降低35%，持续4秒。当前版本仅展示说明。")),
        createEnemyTemplate(QStringLiteral("enemy_plague_r%1").arg(r), QStringLiteral("瘟疫巫师"),
                            {QStringLiteral("远程"), QStringLiteral("削弱")},
                            QStringLiteral("持续削弱型辅助，擅长群体消耗。"),
                            380 + r * 65, 50 + r * 8, 5, 0.7, 110, 18, 5,
                            QStringLiteral("腐化诅咒"),
                            QStringLiteral("全体敌军每秒损失3%最大生命值，持续5秒。当前版本仅展示说明。")),
        createEnemyTemplate(QStringLiteral("enemy_shadow_r%1").arg(r), QStringLiteral("暗影刺客"),
                            {QStringLiteral("近战"), QStringLiteral("切后排")},
                            QStringLiteral("高爆发切后排单位，生存能力较低。"),
                            300 + r * 45, 80 + r * 12, 1, 1.2, 70, 16, 3,
                            QStringLiteral("瞬影袭杀"),
                            QStringLiteral("瞬移至最远敌人背后，造成220%攻击力伤害。当前版本仅展示说明。")),
        createEnemyTemplate(QStringLiteral("enemy_ballista_r%1").arg(r), QStringLiteral("重型弩炮"),
                            {QStringLiteral("超远程"), QStringLiteral("范围")},
                            QStringLiteral("超远程攻城单位，范围压制能力极强。"),
                            560 + r * 80, 105 + r * 15, 7, 0.4, 120, 20, 5,
                            QStringLiteral("爆裂炮击"),
                            QStringLiteral("对目标区域造成300%攻击力范围伤害，并降低40%攻速。当前版本仅展示说明。")),
        createEnemyTemplate(QStringLiteral("enemy_warlord_r%1").arg(r), QStringLiteral("战争领主"),
                            {QStringLiteral("Boss"), QStringLiteral("统帅")},
                            QStringLiteral("前排统帅型 Boss，最终轮协同核心。"),
                            1800, 125, 2, 1.0, 120, 15, 10,
                            QStringLiteral("战争怒吼"),
                            QStringLiteral("全体友军提升25%攻击力与25%攻击速度。当前版本仅展示说明。"))
    };
}

QVector<EnemyFormation> allEnemyFormations()
{
    const EnemyTemplateSet r1 = createEnemyTemplatesForRound(1);
    const EnemyTemplateSet r2 = createEnemyTemplatesForRound(2);
    const EnemyTemplateSet r3 = createEnemyTemplatesForRound(3);
    const EnemyTemplateSet r4 = createEnemyTemplatesForRound(4);
    const EnemyTemplateSet r5 = createEnemyTemplatesForRound(5);

    return {
        {{QStringLiteral("r1_frontline"), 1, QStringLiteral("I"), QStringLiteral("1A 前排推进型"),
          QStringLiteral("铁壁守卫与冲锋狂战组成教学推进阵型，教玩家理解前排压力。")},
         {{r1.ironWall, {3, 3}}, {r1.charger, {2, 4}}}},
        {{QStringLiteral("r1_backline"), 1, QStringLiteral("I"), QStringLiteral("1B 后排远程输出型"),
          QStringLiteral("铁壁守卫保护黑羽弩手，教玩家注意后排输出。")},
         {{r1.ironWall, {3, 3}}, {r1.blackBow, {1, 4}}}},
        {{QStringLiteral("r1_control"), 1, QStringLiteral("I"), QStringLiteral("1C 控制辅助混编型"),
          QStringLiteral("铁壁守卫配合寒霜祭司，教玩家认识控制辅助定位。")},
         {{r1.ironWall, {3, 3}}, {r1.frost, {1, 4}}}},

        {{QStringLiteral("r2_frontline"), 2, QStringLiteral("II"), QStringLiteral("2A 前排推进型"),
          QStringLiteral("双铁壁守卫加冲锋狂战形成中路推进压力。")},
         {{r2.ironWall, {3, 2}}, {r2.ironWall, {3, 5}}, {r2.charger, {2, 3}}}},
        {{QStringLiteral("r2_backline"), 2, QStringLiteral("II"), QStringLiteral("2B 后排远程输出型"),
          QStringLiteral("双黑羽弩手提供持续输出，玩家需要考虑切后排。")},
         {{r2.ironWall, {3, 3}}, {r2.blackBow, {1, 2}}, {r2.blackBow, {1, 5}}}},
        {{QStringLiteral("r2_control"), 2, QStringLiteral("II"), QStringLiteral("2C 控制辅助混编型"),
          QStringLiteral("寒霜祭司与瘟疫巫师开始形成控制加消耗配合。")},
         {{r2.ironWall, {3, 3}}, {r2.frost, {1, 2}}, {r2.plague, {1, 5}}}},

        {{QStringLiteral("r3_frontline"), 3, QStringLiteral("III"), QStringLiteral("3A 前排推进型"),
          QStringLiteral("碎甲巨兽和双冲锋狂战开始惩罚脆弱阵型。")},
         {{r3.ironWall, {3, 2}}, {r3.crusher, {3, 5}}, {r3.charger, {2, 3}}, {r3.charger, {2, 4}}}},
        {{QStringLiteral("r3_backline"), 3, QStringLiteral("III"), QStringLiteral("3B 后排远程输出型"),
          QStringLiteral("双前排保护黑羽弩手与爆炎术士，聚团站位更危险。")},
         {{r3.ironWall, {3, 3}}, {r3.ironWall, {3, 4}}, {r3.blackBow, {1, 2}}, {r3.pyro, {1, 5}}}},
        {{QStringLiteral("r3_control"), 3, QStringLiteral("III"), QStringLiteral("3C 控制辅助混编型"),
          QStringLiteral("暗影刺客、寒霜祭司与瘟疫巫师组合，制造后排和持续消耗压力。")},
         {{r3.ironWall, {3, 3}}, {r3.shadow, {2, 1}}, {r3.frost, {1, 4}}, {r3.plague, {1, 6}}}},

        {{QStringLiteral("r4_frontline"), 4, QStringLiteral("IV"), QStringLiteral("4A 前排推进型"),
          QStringLiteral("多方向推进形成棋盘压制，玩家前排承压明显。")},
         {{r4.ironWall, {3, 1}}, {r4.ironWall, {3, 6}}, {r4.crusher, {2, 3}}, {r4.charger, {2, 4}}, {r4.charger, {1, 5}}}},
        {{QStringLiteral("r4_backline"), 4, QStringLiteral("IV"), QStringLiteral("4B 后排远程输出型"),
          QStringLiteral("高火力后排与爆炎术士形成中后场危险区域。")},
         {{r4.ironWall, {3, 2}}, {r4.ironWall, {3, 5}}, {r4.blackBow, {1, 1}}, {r4.blackBow, {1, 6}}, {r4.pyro, {0, 4}}}},
        {{QStringLiteral("r4_control"), 4, QStringLiteral("IV"), QStringLiteral("4C 控制辅助混编型"),
          QStringLiteral("前排拖时间，后排持续削弱，并加入暗影刺客切入压力。")},
         {{r4.ironWall, {3, 3}}, {r4.crusher, {3, 4}}, {r4.frost, {1, 2}}, {r4.plague, {1, 5}}, {r4.shadow, {2, 6}}}},

        {{QStringLiteral("r5_frontline"), 5, QStringLiteral("V"), QStringLiteral("5A 前排推进型"),
          QStringLiteral("最终轮超高正面压力，考验整体承伤能力。")},
         {{r5.ironWall, {3, 1}}, {r5.ironWall, {3, 6}}, {r5.crusher, {3, 3}}, {r5.crusher, {3, 4}}, {r5.charger, {2, 2}}, {r5.charger, {2, 5}}}},
        {{QStringLiteral("r5_backline"), 5, QStringLiteral("V"), QStringLiteral("5B 后排远程输出型"),
          QStringLiteral("黑羽弩手、爆炎术士和重型弩炮组成最强远程火力。")},
         {{r5.ironWall, {3, 2}}, {r5.ironWall, {3, 5}}, {r5.blackBow, {1, 1}}, {r5.blackBow, {1, 6}}, {r5.pyro, {0, 3}}, {r5.ballista, {0, 4}}}},
        {{QStringLiteral("r5_control"), 5, QStringLiteral("V"), QStringLiteral("5C 控制辅助混编型"),
          QStringLiteral("完整控制链与战争领主协同，是最终 Boss 压力。")},
         {{r5.ironWall, {3, 2}}, {r5.crusher, {3, 5}}, {r5.frost, {1, 1}}, {r5.plague, {1, 6}}, {r5.shadow, {2, 0}}, {r5.warlord, {0, 4}}}}
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
