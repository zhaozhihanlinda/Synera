#ifndef UNIT_H
#define UNIT_H

#include <memory>

#include <QString>
#include <QStringList>

enum class ControllerSide {
    PlayerCtrl,
    EnemyCtrl
};

enum class UnitState {
    Idle,
    Moving,
    Attacking,
    Casting,
    Dead
};

struct BoardPosition
{
    int row = -1;
    int col = -1;

    bool isValid() const;

    bool operator==(const BoardPosition &other) const;
    bool operator!=(const BoardPosition &other) const;
};

class Unit
{
public:
    Unit(QString id, QString name, ControllerSide owner);

    QString id() const;
    QString name() const;
    ControllerSide owner() const;

    QStringList traits() const;
    void setTraits(const QStringList &traits);

    int hp() const;
    int maxHp() const;
    int atk() const;
    int range() const;
    int cost() const;
    qreal attackSpeed() const;
    int manaGainOnAttack() const;
    int manaGainOnHit() const;
    int mana() const;
    int maxMana() const;
    QString roleDescription() const;
    QString skillName() const;
    QString skillDescription() const;

    void setMaxHp(int maxHp);
    void setHp(int hp);
    void setAtk(int atk);
    void setRange(int range);
    void setCost(int cost);
    void setAttackSpeed(qreal attackSpeed);
    void setManaGainOnAttack(int manaGainOnAttack);
    void setManaGainOnHit(int manaGainOnHit);
    void setMaxMana(int maxMana);
    void setMana(int mana);
    void setRoleDescription(const QString &roleDescription);
    void setSkillName(const QString &skillName);
    void setSkillDescription(const QString &skillDescription);

    void applyDamage(int damage);
    void heal(int amount);
    void gainMana(int amount);
    bool spendMana(int amount);

    bool isAlive() const;

    UnitState state() const;
    void setState(UnitState state);

    BoardPosition boardPosition() const;
    void setBoardPosition(const BoardPosition &position);
    void clearBoardPosition();

private:
    QString m_id;
    QString m_name;
    ControllerSide m_owner;
    QStringList m_traits;
    int m_hp;
    int m_maxHp;
    int m_atk;
    int m_range;
    int m_cost;
    qreal m_attackSpeed;
    int m_manaGainOnAttack;
    int m_manaGainOnHit;
    int m_mana;
    int m_maxMana;
    QString m_roleDescription;
    QString m_skillName;
    QString m_skillDescription;
    UnitState m_state;
    BoardPosition m_boardPosition;
};

using UnitPtr = std::shared_ptr<Unit>;

#endif // UNIT_H
