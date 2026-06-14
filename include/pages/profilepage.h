#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>

#include "core/playerprofile.h"

class QLabel;
class QPushButton;

class ProfilePage : public QWidget
{
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);

signals:
    void confirmClicked(const PlayerProfile &profile);

protected:
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void randomizeProfile();
    void refreshProfile();

    PlayerProfile currentProfile;
    int lastProfileIndex;
    QLabel *avatarLabel;
    QLabel *nicknameLabel;
    QPushButton *rerollButton;
    QPushButton *confirmButton;
};

#endif // PROFILEPAGE_H
