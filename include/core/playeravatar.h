#ifndef PLAYERAVATAR_H
#define PLAYERAVATAR_H

#include "core/uiscale.h"

#include <QString>

inline QString playerAvatarSymbolForId(const QString &avatarId)
{
    if (avatarId.endsWith(QStringLiteral("01"))) {
        return QStringLiteral("✦");
    }
    if (avatarId.endsWith(QStringLiteral("02"))) {
        return QStringLiteral("☽");
    }
    if (avatarId.endsWith(QStringLiteral("03"))) {
        return QStringLiteral("⚔");
    }
    if (avatarId.endsWith(QStringLiteral("04"))) {
        return QStringLiteral("♜");
    }
    if (avatarId.endsWith(QStringLiteral("05"))) {
        return QStringLiteral("✧");
    }
    if (avatarId.endsWith(QStringLiteral("06"))) {
        return QStringLiteral("❖");
    }
    if (avatarId.endsWith(QStringLiteral("07"))) {
        return QStringLiteral("✹");
    }
    return QStringLiteral("☼");
}

inline QString playerAvatarStyleForId(const QString &avatarId, int borderRadius, int fontSize = -1)
{
    const int resolvedFontSize = fontSize > 0 ? fontSize : 30;
    QString color = QStringLiteral("#ffe9b0");
    QString background = QStringLiteral("qradialgradient(cx:0.5, cy:0.42, radius:0.8, fx:0.46, fy:0.34, stop:0 #8c7c59, stop:0.5 #55452f, stop:1 #221a12)");
    QString borderColor = QStringLiteral("#efcd83");

    if (avatarId.endsWith(QStringLiteral("01"))) {
        color = QStringLiteral("#f6e7c0");
        background = QStringLiteral("qradialgradient(cx:0.5, cy:0.42, radius:0.78, fx:0.46, fy:0.34, stop:0 #5f6990, stop:0.48 #30466b, stop:1 #162131)");
        borderColor = QStringLiteral("#d9bf7a");
    } else if (avatarId.endsWith(QStringLiteral("02"))) {
        color = QStringLiteral("#d9edff");
        background = QStringLiteral("qradialgradient(cx:0.5, cy:0.4, radius:0.8, fx:0.42, fy:0.32, stop:0 #5277a0, stop:0.52 #263f67, stop:1 #121b2d)");
        borderColor = QStringLiteral("#8ab6e5");
    } else if (avatarId.endsWith(QStringLiteral("03"))) {
        color = QStringLiteral("#f4d6b1");
        background = QStringLiteral("qradialgradient(cx:0.5, cy:0.45, radius:0.8, fx:0.48, fy:0.34, stop:0 #7c5960, stop:0.5 #472b3b, stop:1 #1b1421)");
        borderColor = QStringLiteral("#d4a37b");
    } else if (avatarId.endsWith(QStringLiteral("04"))) {
        color = QStringLiteral("#e9e0cf");
        background = QStringLiteral("qradialgradient(cx:0.5, cy:0.43, radius:0.8, fx:0.45, fy:0.34, stop:0 #5b6774, stop:0.52 #33404f, stop:1 #151c25)");
        borderColor = QStringLiteral("#bfc9d6");
    } else if (avatarId.endsWith(QStringLiteral("05"))) {
        color = QStringLiteral("#f7e4c9");
        background = QStringLiteral("qradialgradient(cx:0.5, cy:0.42, radius:0.8, fx:0.46, fy:0.34, stop:0 #71639f, stop:0.5 #3f3567, stop:1 #18142a)");
        borderColor = QStringLiteral("#cfb0f1");
    } else if (avatarId.endsWith(QStringLiteral("06"))) {
        color = QStringLiteral("#d8fff0");
        background = QStringLiteral("qradialgradient(cx:0.5, cy:0.42, radius:0.8, fx:0.46, fy:0.34, stop:0 #4c7f78, stop:0.5 #27534d, stop:1 #122723)");
        borderColor = QStringLiteral("#8fd8c4");
    } else if (avatarId.endsWith(QStringLiteral("07"))) {
        color = QStringLiteral("#ffe3c4");
        background = QStringLiteral("qradialgradient(cx:0.5, cy:0.42, radius:0.8, fx:0.46, fy:0.34, stop:0 #8a6f4b, stop:0.5 #5b4328, stop:1 #241a11)");
        borderColor = QStringLiteral("#e2bc79");
    }

    return UiScale::scaleStyleSheet(QStringLiteral(
        "color: %1;"
        "background-color: %2;"
        "border: 3px solid %3;"
        "border-radius: %4px;"
        "font-size: %5px;"
        "font-weight: 900;"
        "padding-bottom: 4px;")
        .arg(color, background, borderColor)
        .arg(borderRadius)
        .arg(resolvedFontSize));
}

#endif // PLAYERAVATAR_H
