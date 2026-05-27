#ifndef DRAWPAGE_H
#define DRAWPAGE_H

#include <QVector>
#include <QWidget>

#include "core/enemyencounterinfo.h"
#include "core/unit.h"

class QLabel;
class QPushButton;
class QFrame;
class QGridLayout;

class DrawPage : public QWidget
{
    Q_OBJECT

public:
    explicit DrawPage(QWidget *parent = nullptr);

    void setEncounterOptions(const QVector<EnemyEncounterInfo> &options);
    void setOwnedUnits(const QVector<UnitPtr> &units, int capacity);

signals:
    void encounterSelected(const EnemyEncounterInfo &info);
    void enterShopClicked();
    void enterDeployClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void revealCard(int index);
    void refreshOwnedUnitsView();

    QLabel *roundValueLabel;
    QLabel *difficultyValueLabel;
    QLabel *styleValueLabel;
    QLabel *hintValueLabel;
    QLabel *capacityHintLabel;
    QPushButton *shopButton;
    QPushButton *deployButton;
    QPushButton *ownedUnitsButton;
    QVector<QPushButton *> cardButtons;
    QVector<EnemyEncounterInfo> concealedOptions;
    QVector<UnitPtr> ownedUnits;
    int ownedUnitsCapacity;
    bool hasRevealedCard;
    QFrame *ownedUnitsOverlay;
    QFrame *ownedUnitsCard;
    QGridLayout *ownedUnitsGridLayout;
};

#endif // DRAWPAGE_H
