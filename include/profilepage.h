#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>

class QLabel;
class QPushButton;

class ProfilePage : public QWidget
{
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);

signals:
    void profileConfirmed();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void randomizeProfile();
    QString randomAvatarLabel() const;
    QString randomNickname() const;
    void refreshProfile();

    QString currentAvatarLabel;
    QString currentNickname;
    QLabel *avatarLabel;
    QLabel *nicknameLabel;
    QPushButton *rerollButton;
    QPushButton *confirmButton;
};

#endif // PROFILEPAGE_H
