#include "core/unit.h"

#include <algorithm>

namespace {

int clampAtLeastZero(int value)
{
    return std::max(0, value);
}

}

bool BoardPosition::isValid() const
{
    return row >= 0 && col >= 0;
}

bool BoardPosition::operator==(const BoardPosition &other) const
{
    return row == other.row && col == other.col;
}

bool BoardPosition::operator!=(const BoardPosition &other) const
{
    return !(*this == other);
}

Unit::Unit(QString id, QString name, ControllerSide owner)
    : m_id(std::move(id))
    , m_name(std::move(name))
    , m_owner(owner)
    , m_hp(100)
    , m_maxHp(100)
    , m_atk(20)
    , m_range(1)
    , m_cost(0)
    , m_attackSpeed(1.0)
    , m_manaGainOnAttack(0)
    , m_manaGainOnHit(0)
    , m_mana(0)
    , m_maxMana(60)
    , m_state(UnitState::Idle)
{
}

QString Unit::id() const
{
    return m_id;
}

QString Unit::name() const
{
    return m_name;
}

ControllerSide Unit::owner() const
{
    return m_owner;
}

QStringList Unit::traits() const
{
    return m_traits;
}

void Unit::setTraits(const QStringList &traits)
{
    m_traits = traits;
}

int Unit::hp() const
{
    return m_hp;
}

int Unit::maxHp() const
{
    return m_maxHp;
}

int Unit::atk() const
{
    return m_atk;
}

int Unit::range() const
{
    return m_range;
}

int Unit::cost() const
{
    return m_cost;
}

qreal Unit::attackSpeed() const
{
    return m_attackSpeed;
}

int Unit::manaGainOnAttack() const
{
    return m_manaGainOnAttack;
}

int Unit::manaGainOnHit() const
{
    return m_manaGainOnHit;
}

int Unit::mana() const
{
    return m_mana;
}

int Unit::maxMana() const
{
    return m_maxMana;
}

QString Unit::roleDescription() const
{
    return m_roleDescription;
}

QString Unit::skillName() const
{
    return m_skillName;
}

QString Unit::skillDescription() const
{
    return m_skillDescription;
}

void Unit::setMaxHp(int maxHp)
{
    m_maxHp = std::max(1, maxHp);
    m_hp = std::min(m_hp, m_maxHp);
}

void Unit::setHp(int hp)
{
    m_hp = std::clamp(hp, 0, m_maxHp);
    if (m_hp == 0) {
        m_state = UnitState::Dead;
    } else if (m_state == UnitState::Dead) {
        m_state = UnitState::Idle;
    }
}

void Unit::setAtk(int atk)
{
    m_atk = clampAtLeastZero(atk);
}

void Unit::setRange(int range)
{
    m_range = std::max(1, range);
}

void Unit::setCost(int cost)
{
    m_cost = clampAtLeastZero(cost);
}

void Unit::setAttackSpeed(qreal attackSpeed)
{
    m_attackSpeed = std::max<qreal>(0.1, attackSpeed);
}

void Unit::setManaGainOnAttack(int manaGainOnAttack)
{
    m_manaGainOnAttack = clampAtLeastZero(manaGainOnAttack);
}

void Unit::setManaGainOnHit(int manaGainOnHit)
{
    m_manaGainOnHit = clampAtLeastZero(manaGainOnHit);
}

void Unit::setMaxMana(int maxMana)
{
    m_maxMana = clampAtLeastZero(maxMana);
    m_mana = std::min(m_mana, m_maxMana);
}

void Unit::setMana(int mana)
{
    m_mana = std::clamp(mana, 0, m_maxMana);
}

void Unit::setRoleDescription(const QString &roleDescription)
{
    m_roleDescription = roleDescription;
}

void Unit::setSkillName(const QString &skillName)
{
    m_skillName = skillName;
}

void Unit::setSkillDescription(const QString &skillDescription)
{
    m_skillDescription = skillDescription;
}

void Unit::applyDamage(int damage)
{
    if (damage <= 0 || !isAlive()) {
        return;
    }
    setHp(m_hp - damage);
}

void Unit::heal(int amount)
{
    if (amount <= 0 || !isAlive()) {
        return;
    }
    setHp(m_hp + amount);
}

void Unit::gainMana(int amount)
{
    if (amount <= 0) {
        return;
    }
    setMana(m_mana + amount);
}

bool Unit::spendMana(int amount)
{
    if (amount < 0 || m_mana < amount) {
        return false;
    }
    setMana(m_mana - amount);
    return true;
}

bool Unit::isAlive() const
{
    return m_hp > 0;
}

UnitState Unit::state() const
{
    return m_state;
}

void Unit::setState(UnitState state)
{
    if (!isAlive()) {
        m_state = UnitState::Dead;
        return;
    }
    m_state = state;
}

BoardPosition Unit::boardPosition() const
{
    return m_boardPosition;
}

void Unit::setBoardPosition(const BoardPosition &position)
{
    m_boardPosition = position;
}

void Unit::clearBoardPosition()
{
    m_boardPosition = BoardPosition{};
}
