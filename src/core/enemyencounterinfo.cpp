#include "core/enemyencounterinfo.h"

QVector<EnemyEncounterInfo> enemyEncounterPoolForRound(int round)
{
    switch (round) {
    case 1:
        return {
            {QStringLiteral("r1_frontline"), 1, QStringLiteral("I"), QStringLiteral("Frontline Push"),
             QStringLiteral("敌军以简单前排推进为主，正面承压会更明显。")},
            {QStringLiteral("r1_backline"), 1, QStringLiteral("I"), QStringLiteral("Backline Ranged Output"),
             QStringLiteral("敌军依赖基础后排火力，需注意后排承伤。")},
            {QStringLiteral("r1_control"), 1, QStringLiteral("I"), QStringLiteral("Control / Support Mixed"),
             QStringLiteral("敌军混入基础辅助与干扰，阵型应保持平衡。")}
        };
    case 2:
        return {
            {QStringLiteral("r2_frontline"), 2, QStringLiteral("II"), QStringLiteral("Frontline Push"),
             QStringLiteral("敌军前后排开始配合，前排推进会更持续。")},
            {QStringLiteral("r2_backline"), 2, QStringLiteral("II"), QStringLiteral("Backline Ranged Output"),
             QStringLiteral("敌军后排输出更稳定，需尽量缩短承火时间。")},
            {QStringLiteral("r2_control"), 2, QStringLiteral("II"), QStringLiteral("Control / Support Mixed"),
             QStringLiteral("敌军协同更明确，阵容平衡性会更重要。")}
        };
    case 3:
        return {
            {QStringLiteral("r3_frontline"), 3, QStringLiteral("III"), QStringLiteral("Frontline Push"),
             QStringLiteral("敌军前排压制更强，单薄阵线会更容易崩溃。")},
            {QStringLiteral("r3_backline"), 3, QStringLiteral("III"), QStringLiteral("Backline Ranged Output"),
             QStringLiteral("敌军开始拥有高攻速或范围火力，后排压力提升。")},
            {QStringLiteral("r3_control"), 3, QStringLiteral("III"), QStringLiteral("Control / Support Mixed"),
             QStringLiteral("敌军干扰能力提升，部署时要考虑续航与容错。")}
        };
    case 4:
        return {
            {QStringLiteral("r4_frontline"), 4, QStringLiteral("IV"), QStringLiteral("Frontline Push"),
             QStringLiteral("敌军形成明显正面压制，前排站位选择很关键。")},
            {QStringLiteral("r4_backline"), 4, QStringLiteral("IV"), QStringLiteral("Backline Ranged Output"),
             QStringLiteral("敌军火力密度更高，需要更主动的反后排思路。")},
            {QStringLiteral("r4_control"), 4, QStringLiteral("IV"), QStringLiteral("Control / Support Mixed"),
             QStringLiteral("敌军配合会制造更强站位陷阱，需谨慎分配人口。")}
        };
    case 5:
    default:
        return {
            {QStringLiteral("r5_frontline"), 5, QStringLiteral("V"), QStringLiteral("Frontline Push"),
             QStringLiteral("敌军正面压迫达到最高等级，考验完整前排体系。")},
            {QStringLiteral("r5_backline"), 5, QStringLiteral("V"), QStringLiteral("Backline Ranged Output"),
             QStringLiteral("敌军后排火力完整成型，必须重视切入或抗压。")},
            {QStringLiteral("r5_control"), 5, QStringLiteral("V"), QStringLiteral("Control / Support Mixed"),
             QStringLiteral("敌军拥有完整协同，是最终轮最全面的战术检验。")}
        };
    }
}
