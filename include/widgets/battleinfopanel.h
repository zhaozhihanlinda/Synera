#ifndef BATTLEINFOPANEL_H
#define BATTLEINFOPANEL_H

#include <QFrame>

#include "core/unit.h"

class QLabel;

class BattleInfoPanel : public QFrame
{
    Q_OBJECT

public:
    explicit BattleInfoPanel(QWidget *parent = nullptr);

    void showUnit(const UnitPtr &unit);

signals:
    void closeClicked();

private:
    QLabel *nameLabel;
    QLabel *statsLabel;
    QLabel *traitsLabel;
};

#endif // BATTLEINFOPANEL_H
