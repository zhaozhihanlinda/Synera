#include "pages/battlecountdownpage.h"

#include "core/uiscale.h"

#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>

BattleCountdownPage::BattleCountdownPage(QWidget *parent)
    : QWidget(parent)
    , countdownLabel(new QLabel(QStringLiteral("3"), this))
    , captionLabel(new QLabel(QStringLiteral("战斗即将开始"), this))
    , countdownTimer(new QTimer(this))
    , remainingSeconds(3)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(UiScale::size(1280, 820));

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(UiScale::margins(72, 42, 72, 42));
    rootLayout->setSpacing(UiScale::scaled(18));
    rootLayout->addStretch(1);

    captionLabel->setObjectName("captionLabel");
    captionLabel->setAlignment(Qt::AlignCenter);
    countdownLabel->setObjectName("countdownLabel");
    countdownLabel->setAlignment(Qt::AlignCenter);

    rootLayout->addWidget(captionLabel);
    rootLayout->addWidget(countdownLabel);
    rootLayout->addStretch(1);

    setStyleSheet(UiScale::scaleStyleSheet(QStringLiteral(R"(
        BattleCountdownPage {
            background-color: #090d15;
        }
        #captionLabel {
            color: #f5e4bd;
            font-size: 30px;
            font-weight: 800;
        }
        #countdownLabel {
            color: #ffffff;
            font-size: 128px;
            font-weight: 900;
        }
    )")));

    countdownTimer->setInterval(1000);
    connect(countdownTimer, &QTimer::timeout, this, [this]() {
        --remainingSeconds;
        if (remainingSeconds <= 0) {
            countdownTimer->stop();
            emit countdownFinished();
            return;
        }
        refreshCountdownText();
    });
}

void BattleCountdownPage::startCountdown(int seconds)
{
    countdownTimer->stop();
    remainingSeconds = seconds;
    refreshCountdownText();
    countdownTimer->start();
}

void BattleCountdownPage::refreshCountdownText()
{
    countdownLabel->setText(QString::number(remainingSeconds));
}

void BattleCountdownPage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(9, 13, 21));

    QLinearGradient glow(0, 0, width(), height());
    glow.setColorAt(0.0, QColor(18, 35, 66, 135));
    glow.setColorAt(0.5, QColor(8, 13, 23, 0));
    glow.setColorAt(1.0, QColor(72, 39, 52, 115));
    painter.fillRect(rect(), glow);

    painter.setPen(QPen(QColor(123, 141, 181, 22), 1));
    const int gridSize = UiScale::scaled(48);
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}
