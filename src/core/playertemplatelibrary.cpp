#include "core/playertemplatelibrary.h"

namespace {

QVector<UnitTemplate> createTemplates()
{
    return {
        {QStringLiteral("knight"), QStringLiteral("骑士"), {QStringLiteral("前排"), QStringLiteral("坦克")}, 10,
         QStringLiteral("近战坦克，负责吸收伤害。"), 950, 60, 1, 1.0, 100, 10, 8,
         QStringLiteral("击晕"),
         QStringLiteral("对当前目标造成150%攻击力伤害，并暂停攻击1s。"),
         {QStringLiteral("长剑：4金币，+30攻击力，+0.3攻速"),
          QStringLiteral("盾牌：4金币，+100生命值")}},
        {QStringLiteral("heavy_warrior"), QStringLiteral("重甲战士"), {QStringLiteral("前排"), QStringLiteral("战士")}, 12,
         QStringLiteral("前排持续作战，兼顾承伤与输出。"), 800, 90, 1, 1.2, 90, 12, 8,
         QStringLiteral("横扫重击"),
         QStringLiteral("对前方扇形范围敌人造成180%攻击力伤害。"),
         {QStringLiteral("巨剑：5金币，+40攻击力，+0.2攻速"),
          QStringLiteral("重甲：5金币，+150生命值，-10%伤害")}},
        {QStringLiteral("berserker"), QStringLiteral("狂暴战士"), {QStringLiteral("近战"), QStringLiteral("爆发")}, 13,
         QStringLiteral("高爆发近战，擅长快速击杀。"), 700, 110, 1, 1.0, 70, 14, 5,
         QStringLiteral("狂暴斩击"),
         QStringLiteral("对当前目标造成220%攻击力伤害；若目标血量低于30%，则造成150%额外伤害。"),
         {QStringLiteral("战斧：5金币，+50攻击力，+0.2攻速"),
          QStringLiteral("锁子甲：5金币，+80生命值，-5%伤害")}},
        {QStringLiteral("fire_mage"), QStringLiteral("火法师"), {QStringLiteral("远程"), QStringLiteral("范围")}, 14,
         QStringLiteral("远程范围爆发，擅长压低敌群血线。"), 500, 130, 3, 1.0, 120, 15, 5,
         QStringLiteral("火焰爆裂"),
         QStringLiteral("对前方扇形目标区域造成250%攻击力范围伤害。"),
         {QStringLiteral("火法杖：6金币，+50攻击力"),
          QStringLiteral("火法袍：6金币，+50生命值，-20%伤害"),
          QStringLiteral("炽焰浆：6金币，+30攻击回蓝，+30%受击回蓝")}},
        {QStringLiteral("ice_mage"), QStringLiteral("冰法师"), {QStringLiteral("远程"), QStringLiteral("控制")}, 14,
         QStringLiteral("远程控制型法师，擅长干扰敌军。"), 500, 90, 3, 0.8, 120, 15, 5,
         QStringLiteral("寒冰冻结"),
         QStringLiteral("对前方扇形区域目标造成120%攻击力伤害，并暂停攻击1.5s。"),
         {QStringLiteral("冰法杖：6金币，+15攻击力"),
          QStringLiteral("冰法袍：6金币，+50生命值，-20%伤害"),
          QStringLiteral("寒川水：6金币，+30攻击回蓝，+30%受击回蓝")}},
        {QStringLiteral("archer"), QStringLiteral("弓箭手"), {QStringLiteral("远程"), QStringLiteral("持续输出")}, 12,
         QStringLiteral("高攻速后排，擅长单体持续输出。"), 500, 75, 5, 1.8, 80, 10, 4,
         QStringLiteral("连射"),
         QStringLiteral("对目标一次性射出3支箭，每支造成80%攻击力伤害。"),
         {QStringLiteral("毒箭：4金币，+25攻击力，+0.4攻速"),
          QStringLiteral("掩体：5金币，+150生命值")}},
        {QStringLiteral("catapult"), QStringLiteral("投石机"), {QStringLiteral("超远程"), QStringLiteral("范围")}, 18,
         QStringLiteral("超远程范围输出，攻击慢但压制力强。"), 750, 110, 6, 0.4, 120, 20, 5,
         QStringLiteral("燃弹重击"),
         QStringLiteral("向目标区域投掷燃烧石弹，造成280%攻击力伤害并附带减速。"),
         {QStringLiteral("强化投石臂：6金币，+70攻击力"),
          QStringLiteral("加固木架：5金币，+200生命值")}},
        {QStringLiteral("witch_doctor"), QStringLiteral("巫医"), {QStringLiteral("远程"), QStringLiteral("辅助")}, 13,
         QStringLiteral("治疗辅助，保障队伍续航。"), 600, 40, 4, 0.8, 100, 20, 5,
         QStringLiteral("回天之术"),
         QStringLiteral("优先治疗生命值最低友军，提供高额恢复。"),
         {QStringLiteral("毒药：5金币，+35攻击力"),
          QStringLiteral("治愈魔法：5金币，+50生命值")}},
        {QStringLiteral("assassin"), QStringLiteral("刺客"), {QStringLiteral("近战"), QStringLiteral("切后排")}, 13,
         QStringLiteral("高爆发切后排，生存较弱。"), 450, 130, 1, 1.0, 70, 16, 3,
         QStringLiteral("暗影突袭"),
         QStringLiteral("对敌军距离最远目标造成200%攻击力伤害。"),
         {QStringLiteral("匕首：6金币，+40攻击力，+0.3攻速"),
          QStringLiteral("隐身衣：6金币，+60生命值，+10%攻速")}},
        {QStringLiteral("engineer"), QStringLiteral("工程师"), {QStringLiteral("远程"), QStringLiteral("辅助")}, 13,
         QStringLiteral("远程辅助型单位，偏团队增益。"), 520, 70, 4, 0.8, 80, 14, 4,
         QStringLiteral("机械强化"),
         QStringLiteral("强化距离最近的友方机械单位，使其攻击力和生命值提升。"),
         {QStringLiteral("工具箱：6金币，+25攻击力，+15%技能效果"),
          QStringLiteral("机械护驾：6金币，+120生命值")}},
        {QStringLiteral("strategist"), QStringLiteral("军师"), {QStringLiteral("后排"), QStringLiteral("团队辅助")}, 15,
         QStringLiteral("后排策略辅助，提升整体作战能力。"), 500, 55, 4, 0.9, 90, 15, 5,
         QStringLiteral("战术号令"),
         QStringLiteral("为全体友军提供攻击速度与攻击力增益，持续一段时间。"),
         {QStringLiteral("指挥旗：6金币，+10%全队攻速加成"),
          QStringLiteral("近身护卫：5金币，+100生命值")}}
    };
}

}

QVector<UnitTemplate> allPlayerUnitTemplates()
{
    return createTemplates();
}

UnitTemplate playerUnitTemplateById(const QString &templateId)
{
    const QVector<UnitTemplate> templates = createTemplates();
    for (const UnitTemplate &unitTemplate : templates) {
        if (unitTemplate.templateId == templateId) {
            return unitTemplate;
        }
    }
    return UnitTemplate{};
}

QVector<UnitTemplate> defaultShopTemplates()
{
    return {
        playerUnitTemplateById(QStringLiteral("knight")),
        playerUnitTemplateById(QStringLiteral("heavy_warrior")),
        playerUnitTemplateById(QStringLiteral("fire_mage")),
        playerUnitTemplateById(QStringLiteral("archer")),
        playerUnitTemplateById(QStringLiteral("witch_doctor"))
    };
}
