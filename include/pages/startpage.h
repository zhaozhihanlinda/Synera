#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>

class QPushButton;

class StartPage : public QWidget
{
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = nullptr);

signals:
    void startClicked();
    void exitClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *startButton;
    QPushButton *exitButton;
};

#endif // STARTPAGE_H
