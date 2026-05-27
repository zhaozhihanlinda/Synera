#include "widgets/battleinfopanel.h"

#include "core/uiscale.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

BattleInfoPanel::BattleInfoPanel(QWidget *parent)
    : QFrame(parent)
    , nameLabel(new QLabel(this))
    , statsLabel(new QLabel(this))
    , traitsLabel(new QLabel(this))
{
    setObjectName("battleInfoPanel");
    hide();

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(UiScale::margins(22, 18, 22, 18));
    layout->setSpacing(UiScale::scaled(8));

    auto *header = new QHBoxLayout;
    header->setSpacing(UiScale::scaled(12));

    nameLabel->setObjectName("selectedName");
    auto *closeButton = new QPushButton(QStringLiteral("X"), this);
    closeButton->setObjectName("closeInfoButton");
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setFixedSize(UiScale::size(40, 40));

    header->addWidget(nameLabel, 1);
    header->addWidget(closeButton);

    statsLabel->setObjectName("panelBody");
    statsLabel->setWordWrap(true);
    traitsLabel->setObjectName("panelCaption");
    traitsLabel->setWordWrap(true);

    layout->addLayout(header);
    layout->addWidget(statsLabel);
    layout->addWidget(traitsLabel);

    connect(closeButton, &QPushButton::clicked, this, &BattleInfoPanel::closeClicked);
}

void BattleInfoPanel::showUnit(const UnitPtr &unit)
{
    if (!unit) {
        hide();
        return;
    }

    nameLabel->setText(unit->name());
    statsLabel->setText(
        QStringLiteral("HP: %1 / %2    Mana: %3 / %4    ATK: %5    Skill: %6\n%7")
            .arg(unit->hp())
            .arg(unit->maxHp())
            .arg(unit->mana())
            .arg(unit->maxMana())
            .arg(unit->atk())
            .arg(unit->skillName().isEmpty() ? QStringLiteral("-") : unit->skillName())
            .arg(unit->skillDescription().isEmpty() ? QStringLiteral("-") : unit->skillDescription()));
    traitsLabel->setText(QStringLiteral("Traits: %1")
                             .arg(unit->traits().isEmpty()
                                      ? QStringLiteral("-")
                                      : unit->traits().join(QStringLiteral(", "))));
    show();
}
