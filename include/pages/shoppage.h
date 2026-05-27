#ifndef SHOPPAGE_H
#define SHOPPAGE_H

#include <QWidget>
#include <QVector>

#include "core/playerprofile.h"
#include "core/unit.h"

class QLabel;
class QPushButton;
class QGridLayout;
class QFrame;

class ShopPage : public QWidget
{
    Q_OBJECT

public:
    explicit ShopPage(QWidget *parent = nullptr);

    void setGameInfo(int round,
                     int gold,
                     int currentPopulation,
                     int maxPopulation,
                     const QVector<UnitPtr> &ownedUnits);

signals:
    void enterDeployClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void populateShopCards();
    void refreshOwnedUnits();
    void showOwnedUnitDetail(const UnitPtr &unit);

    QLabel *roundValueLabel;
    QLabel *goldValueLabel;
    QLabel *populationValueLabel;
    QPushButton *enterDeployButton;
    QVector<UnitPtr> ownedUnits;
    QGridLayout *ownedUnitsLayout;
    QGridLayout *shopCardsLayout;
    QFrame *unitDetailOverlay;
    QLabel *unitDetailTitleLabel;
    QLabel *unitDetailBodyLabel;
};

#endif // SHOPPAGE_H
