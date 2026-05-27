#ifndef INITINFOPAGE_H
#define INITINFOPAGE_H

#include <QWidget>

#include "core/playerprofile.h"

class QLabel;
class QFrame;
class QPushButton;

class InitInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit InitInfoPage(QWidget *parent = nullptr);

    void setGameInfo(const PlayerProfile &profile,
                     int hp,
                     int gold,
                     int populationLimit);

signals:
    void enterGameClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    QLabel *avatarValueLabel;
    QLabel *nicknameValueLabel;
    QLabel *hpValueLabel;
    QLabel *goldValueLabel;
    QLabel *populationValueLabel;
    QPushButton *enterGameButton;
    QFrame *rulesOverlay;
    QPushButton *closeRulesButton;
};

#endif // INITINFOPAGE_H
