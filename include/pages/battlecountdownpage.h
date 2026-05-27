#ifndef BATTLECOUNTDOWNPAGE_H
#define BATTLECOUNTDOWNPAGE_H

#include <QWidget>

class QLabel;
class QTimer;

class BattleCountdownPage : public QWidget
{
    Q_OBJECT

public:
    explicit BattleCountdownPage(QWidget *parent = nullptr);

    void startCountdown(int seconds = 3);

signals:
    void countdownFinished();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void refreshCountdownText();

    QLabel *countdownLabel;
    QLabel *captionLabel;
    QTimer *countdownTimer;
    int remainingSeconds;
};

#endif // BATTLECOUNTDOWNPAGE_H
