#ifndef UISCALE_H
#define UISCALE_H

#include <QMargins>
#include <QRegularExpression>
#include <QSize>
#include <QString>
#include <QtMath>

namespace UiScale {

inline constexpr qreal kUiScale = 0.9;

inline int scaled(int value)
{
    return qMax(1, qRound(value * kUiScale));
}

inline QSize size(int width, int height)
{
    return QSize(scaled(width), scaled(height));
}

inline int width(int value)
{
    return scaled(value);
}

inline int height(int value)
{
    return scaled(value);
}

inline QMargins margins(int left, int top, int right, int bottom)
{
    return QMargins(scaled(left), scaled(top), scaled(right), scaled(bottom));
}

inline QString scaleStyleSheet(const QString &styleSheet)
{
    QString scaledSheet = styleSheet;

    const QList<QString> properties = {
        QStringLiteral("font-size"),
        QStringLiteral("border-radius"),
        QStringLiteral("padding"),
        QStringLiteral("padding-left"),
        QStringLiteral("padding-right"),
        QStringLiteral("padding-top"),
        QStringLiteral("padding-bottom"),
        QStringLiteral("margin"),
        QStringLiteral("margin-left"),
        QStringLiteral("margin-right"),
        QStringLiteral("margin-top"),
        QStringLiteral("margin-bottom"),
        QStringLiteral("letter-spacing")
    };

    for (const QString &property : properties) {
        QRegularExpression regex(QStringLiteral("(%1\\s*:\\s*)([^;]+)(;)").arg(QRegularExpression::escape(property)));
        QRegularExpressionMatchIterator iterator = regex.globalMatch(scaledSheet);
        QList<QRegularExpressionMatch> matches;
        while (iterator.hasNext()) {
            matches.append(iterator.next());
        }

        for (auto matchIt = matches.crbegin(); matchIt != matches.crend(); ++matchIt) {
            const QRegularExpressionMatch &match = *matchIt;
            QString valueBlock = match.captured(2);
            QRegularExpression numberRegex(QStringLiteral("(-?\\d+)px"));
            QRegularExpressionMatchIterator numberIterator = numberRegex.globalMatch(valueBlock);
            QList<QRegularExpressionMatch> numberMatches;
            while (numberIterator.hasNext()) {
                numberMatches.append(numberIterator.next());
            }

            for (auto numberIt = numberMatches.crbegin(); numberIt != numberMatches.crend(); ++numberIt) {
                const QRegularExpressionMatch &numberMatch = *numberIt;
                const int original = numberMatch.captured(1).toInt();
                const QString replacement = QStringLiteral("%1px").arg(scaled(original));
                valueBlock.replace(numberMatch.capturedStart(0), numberMatch.capturedLength(0), replacement);
            }

            scaledSheet.replace(match.capturedStart(2), match.capturedLength(2), valueBlock);
        }
    }

    return scaledSheet;
}

}

#endif // UISCALE_H
