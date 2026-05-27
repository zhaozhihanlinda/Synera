#ifndef SHOPPAGE_H
#define SHOPPAGE_H

#include <QWidget>

#include "core/playerprofile.h"

class QLabel;
class QPushButton;
class QGridLayout;

class ShopPage : public QWidget
{
    Q_OBJECT

public:
    explicit ShopPage(QWidget *parent = nullptr);

    void setGameInfo(int round,
                       int gold,
                       int hp,
                       int currentPopulation,
                       int maxPopulation);

signals:
    void enterDeployClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void populateShopCards();

    QLabel *roundValueLabel;
    QLabel *hpValueLabel;
    QLabel *goldValueLabel;
    QLabel *populationValueLabel;
    QPushButton *enterDeployButton;
    QGridLayout *shopCardsLayout;
};

#endif // SHOPPAGE_H
