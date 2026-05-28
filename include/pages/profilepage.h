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
    QString randomAvatarLabel() const;
    QString randomNickname() const;
    QString avatarSymbolForId(const QString &avatarId) const;
    QString avatarStyleForId(const QString &avatarId) const;
    void refreshProfile();

    PlayerProfile currentProfile;
    QLabel *avatarLabel;
    QLabel *nicknameLabel;
    QPushButton *rerollButton;
    QPushButton *confirmButton;
};

#endif // PROFILEPAGE_H
