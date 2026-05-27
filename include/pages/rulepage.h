#ifndef RULEPAGE_H
#define RULEPAGE_H

#include <QWidget>

class QPushButton;

class RulePage : public QWidget
{
    Q_OBJECT

public:
    explicit RulePage(QWidget *parent = nullptr);

signals:
    void startGameClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *startButton;
};

#endif // RULEPAGE_H
