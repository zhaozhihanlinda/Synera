#ifndef SHOPPAGE_H
#define SHOPPAGE_H

#include <QWidget>
#include <QString>
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
                     const QVector<UnitPtr> &ownedUnits,
                     int ownedUnitCapacity,
                     const QVector<QString> &sellableTemplateIds,
                     const QVector<QString> &shopTemplateIds,
                     int refreshCost,
                     bool canRefreshShop);

signals:
    void enterDeployClicked();
    void buyUnitClicked(const QString &templateId);
    void sellUnitClicked(const QString &templateId);
    void refreshShopClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void populateShopCards();
    void refreshOwnedUnits();
    void showOwnedUnitDetail(const UnitPtr &unit);

    QLabel *roundValueLabel;
    QLabel *goldValueLabel;
    QLabel *populationValueLabel;
    QLabel *shopRuleLabel;
    QPushButton *refreshShopButton;
    QPushButton *enterDeployButton;
    QVector<UnitPtr> ownedUnits;
    int currentGold;
    int ownedUnitCapacity;
    QVector<QString> sellableTemplateIds;
    QVector<QString> currentShopTemplateIds;
    int currentRefreshCost;
    bool currentCanRefreshShop;
    QGridLayout *ownedUnitsLayout;
    QGridLayout *shopCardsLayout;
    QFrame *unitDetailOverlay;
    QLabel *unitDetailTitleLabel;
    QLabel *unitDetailBodyLabel;
};

#endif // SHOPPAGE_H
