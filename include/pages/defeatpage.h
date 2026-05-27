#ifndef DEFEATPAGE_H
#define DEFEATPAGE_H

#include <QWidget>

class QPushButton;

class DefeatPage : public QWidget
{
    Q_OBJECT

public:
    explicit DefeatPage(QWidget *parent = nullptr);

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

#endif // DEFEATPAGE_H
