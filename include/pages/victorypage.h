#ifndef VICTORYPAGE_H
#define VICTORYPAGE_H

#include <QWidget>

class QPushButton;

class VictoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit VictoryPage(QWidget *parent = nullptr);

signals:
    void restartClicked();
    void returnToStartClicked();
    void exitClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *restartButton;
    QPushButton *returnButton;
    QPushButton *exitButton;
};

#endif // VICTORYPAGE_H
