#include "widgets/benchwidget.h"

#include "core/uiscale.h"

#include <QApplication>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>

namespace {

const char kBoardDragMime[] = "application/x-synera-board-pos";
const char kBenchDragMime[] = "application/x-synera-bench-slot";

}

BenchWidget::BenchWidget(QWidget *parent)
    : QWidget(parent)
    , m_board(nullptr)
    , m_selectedSlot(-1)
    , m_pressedSlot(-1)
{
    setMinimumHeight(UiScale::height(132));
    setAcceptDrops(true);
}

void BenchWidget::setBoard(Board *board)
{
    m_board = board;
    update();
}

void BenchWidget::setSelectedSlot(int slot)
{
    m_selectedSlot = slot;
    update();
}

void BenchWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRect panelRect = rect().adjusted(UiScale::scaled(4), UiScale::scaled(6), -UiScale::scaled(4), -UiScale::scaled(6));
    painter.setPen(QPen(QColor(182, 157, 104, 170), UiScale::scaled(2)));
    painter.setBrush(QColor(10, 16, 28, 212));
    painter.drawRoundedRect(panelRect, UiScale::scaled(20), UiScale::scaled(20));

    painter.setPen(QColor("#e6d09f"));
    painter.setFont(QFont(QStringLiteral("Helvetica"), UiScale::scaled(16), QFont::Bold));
    painter.drawText(panelRect.adjusted(UiScale::scaled(18), UiScale::scaled(10), -UiScale::scaled(18), 0),
                     Qt::AlignLeft | Qt::AlignTop, QStringLiteral("备战区"));

    if (!m_board) {
        return;
    }

    const int slotCount = m_board->benchCapacity();
    for (int index = 0; index < slotCount; ++index) {
        const QRect slotRect = this->slotRect(index);
        const UnitPtr unit = m_board->benchUnitAt(index);
        const bool selected = index == m_selectedSlot;

        painter.setPen(QPen(selected ? QColor("#f4d796") : (unit ? QColor("#6fd4de") : QColor(125, 145, 171, 120)),
                            selected ? UiScale::scaled(3) : (unit ? UiScale::scaled(2) : 1),
                            unit ? Qt::SolidLine : Qt::DashLine));
        painter.setBrush(selected ? QColor(48, 56, 74, 220) : (unit ? QColor(27, 51, 64, 210) : QColor(18, 26, 39, 170)));
        painter.drawRoundedRect(slotRect, UiScale::scaled(14), UiScale::scaled(14));

        painter.setPen(unit ? QColor("#f5f2e9") : QColor("#7f8da6"));
        painter.setFont(QFont(QStringLiteral("Helvetica"), UiScale::scaled(unit ? 13 : 12), unit ? QFont::Bold : QFont::Medium));
        painter.drawText(slotRect.adjusted(UiScale::scaled(6), UiScale::scaled(8), -UiScale::scaled(6), -UiScale::scaled(8)),
                         Qt::AlignCenter | Qt::TextWordWrap,
                         unit ? unit->name() : QStringLiteral("Bench %1").arg(index + 1));
    }
}

void BenchWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_board || event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    m_dragStartPos = event->position().toPoint();
    m_pressedSlot = slotAt(event->position().toPoint());
    if (m_pressedSlot >= 0) {
        emit unitPressed(m_board->benchUnitAt(m_pressedSlot));
        emit slotClicked(m_pressedSlot);
    } else {
        emit unitPressed(nullptr);
    }

    QWidget::mousePressEvent(event);
}

void BenchWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton) || !m_board || m_pressedSlot < 0
        || (event->position().toPoint() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance()) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    const UnitPtr unit = m_board->benchUnitAt(m_pressedSlot);
    if (unit) {
        auto *mimeData = new QMimeData;
        mimeData->setData(kBenchDragMime, QByteArray::number(m_pressedSlot));

        QPixmap pixmap(UiScale::size(96, 72));
        pixmap.fill(Qt::transparent);
        {
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(QColor("#6fd4de"), UiScale::scaled(2)));
            painter.setBrush(QColor(27, 51, 64, 220));
            painter.drawRoundedRect(pixmap.rect().adjusted(UiScale::scaled(3), UiScale::scaled(3),
                                                           -UiScale::scaled(3), -UiScale::scaled(3)),
                                    UiScale::scaled(14), UiScale::scaled(14));
            painter.setPen(QColor("#f5f2e9"));
            painter.setFont(QFont(QStringLiteral("Helvetica"), UiScale::scaled(11), QFont::Bold));
            painter.drawText(pixmap.rect().adjusted(UiScale::scaled(8), UiScale::scaled(8),
                                                    -UiScale::scaled(8), -UiScale::scaled(8)),
                             Qt::AlignCenter | Qt::TextWordWrap, unit->name());
        }

        QDrag drag(this);
        drag.setMimeData(mimeData);
        drag.setPixmap(pixmap);
        drag.setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
        drag.exec(Qt::MoveAction);
    }

    m_pressedSlot = -1;
    QWidget::mouseMoveEvent(event);
}

void BenchWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(kBoardDragMime)) {
        event->acceptProposedAction();
        return;
    }
    event->ignore();
}

void BenchWidget::dragMoveEvent(QDragMoveEvent *event)
{
    const int slot = slotAt(event->position().toPoint());
    if (slot >= 0 && event->mimeData()->hasFormat(kBoardDragMime) && !m_board->benchUnitAt(slot)) {
        event->acceptProposedAction();
        return;
    }
    event->ignore();
}

void BenchWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    update();
}

void BenchWidget::dropEvent(QDropEvent *event)
{
    const int slot = slotAt(event->position().toPoint());
    if (slot < 0 || !m_board || m_board->benchUnitAt(slot) || !event->mimeData()->hasFormat(kBoardDragMime)) {
        event->ignore();
        return;
    }

    const QList<QByteArray> parts = event->mimeData()->data(kBoardDragMime).split(',');
    if (parts.size() != 2) {
        event->ignore();
        return;
    }

    emit boardUnitDroppedToBench({parts.at(0).toInt(), parts.at(1).toInt()}, slot);
    event->acceptProposedAction();
}

QRect BenchWidget::slotRect(int index) const
{
    const QRect panelRect = rect().adjusted(UiScale::scaled(4), UiScale::scaled(6), -UiScale::scaled(4), -UiScale::scaled(6));
    const QRect contentRect = panelRect.adjusted(UiScale::scaled(16), UiScale::scaled(40),
                                                 -UiScale::scaled(16), -UiScale::scaled(16));
    const int gap = UiScale::scaled(10);
    const int slotCount = m_board ? m_board->benchCapacity() : Board::kDefaultBenchSlots;
    const int slotWidth = (contentRect.width() - (slotCount - 1) * gap) / slotCount;
    return QRect(contentRect.left() + index * (slotWidth + gap), contentRect.top(), slotWidth, contentRect.height());
}

int BenchWidget::slotAt(const QPoint &point) const
{
    const int slotCount = m_board ? m_board->benchCapacity() : Board::kDefaultBenchSlots;
    for (int index = 0; index < slotCount; ++index) {
        if (slotRect(index).contains(point)) {
            return index;
        }
    }
    return -1;
}
