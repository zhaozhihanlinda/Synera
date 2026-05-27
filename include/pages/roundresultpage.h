#ifndef ROUNDRESULTPAGE_H
#define ROUNDRESULTPAGE_H

#include <QWidget>

#include "core/gamemanager.h"

class QLabel;
class QPushButton;

class RoundResultPage : public QWidget
{
    Q_OBJECT

public:
    explicit RoundResultPage(QWidget *parent = nullptr);

    void setResultInfo(int round, int hp, const BattleResult &result);

signals:
    void continueClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *resultTitleLabel;
    QLabel *roundValueLabel;
    QLabel *hpValueLabel;
    QLabel *rewardValueLabel;
    QLabel *damageValueLabel;
    QPushButton *continueButton;
};

#endif // ROUNDRESULTPAGE_H
