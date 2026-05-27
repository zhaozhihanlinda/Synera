#include "widgets/boardwidget.h"

#include "core/uiscale.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QApplication>

namespace {

const int kBoardPadding = UiScale::scaled(24);
const int kMidGap = UiScale::scaled(16);
const int kCellGap = UiScale::scaled(6);
const char kBenchDragMime[] = "application/x-synera-bench-slot";
const char kBoardDragMime[] = "application/x-synera-board-pos";

QColor borderColorForSide(ControllerSide side)
{
    return side == ControllerSide::PlayerCtrl ? QColor("#6fd4de") : QColor("#d7748f");
}

QColor fillColorForSide(ControllerSide side)
{
    return side == ControllerSide::PlayerCtrl ? QColor(28, 57, 72, 220) : QColor(65, 28, 42, 220);
}

QString avatarText(const UnitPtr &unit)
{
    return unit && !unit->name().isEmpty() ? unit->name().left(1) : QStringLiteral("?");
}

}

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent)
    , m_board(nullptr)
    , m_pendingPlacementUnit(nullptr)
{
    setMinimumSize(UiScale::size(640, 640));
    setMouseTracking(true);
    setAcceptDrops(true);
}

void BoardWidget::setBoard(Board *board)
{
    m_board = board;
    if (m_selectedPosition.isValid() && (!m_board
        || !m_board->isInside(m_selectedPosition.row, m_selectedPosition.col)
        || !m_board->unitAt(m_selectedPosition.row, m_selectedPosition.col))) {
        m_selectedPosition = BoardPosition{};
        emit selectionChanged();
    }
    update();
}

void BoardWidget::setPendingPlacementUnit(const UnitPtr &unit)
{
    m_pendingPlacementUnit = unit;
    m_pendingMoveOrigin = BoardPosition{};
    update();
}

void BoardWidget::setPendingMoveUnit(const UnitPtr &unit, const BoardPosition &origin)
{
    m_pendingPlacementUnit = unit;
    m_pendingMoveOrigin = origin;
    update();
}

void BoardWidget::clearPendingAction()
{
    m_pendingPlacementUnit = nullptr;
    m_pendingMoveOrigin = BoardPosition{};
    update();
}

BoardPosition BoardWidget::selectedPosition() const
{
    return m_selectedPosition;
}

void BoardWidget::setSelectedPosition(const BoardPosition &position)
{
    if (position == m_selectedPosition) {
        return;
    }
    m_selectedPosition = position;
    emit selectionChanged();
    update();
}

UnitPtr BoardWidget::selectedUnit() const
{
    if (!m_board || !m_selectedPosition.isValid()) {
        return nullptr;
    }
    return m_board->unitAt(m_selectedPosition.row, m_selectedPosition.col);
}

void BoardWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const int outerInset = UiScale::scaled(8);
    const int panelRadius = UiScale::scaled(28);
    const int innerInset = UiScale::scaled(8);
    const int selectedPenWidth = UiScale::scaled(3);
    const int tileRadius = UiScale::scaled(12);
    painter.setPen(QPen(QColor(208, 183, 126, 180), UiScale::scaled(2)));
    const QRect panelRect = rect().adjusted(outerInset, outerInset, -outerInset, -outerInset);
    painter.setBrush(QColor(9, 15, 25, 208));
    painter.drawRoundedRect(panelRect, panelRadius, panelRadius);

    QLinearGradient glow(panelRect.topLeft(), panelRect.bottomRight());
    glow.setColorAt(0.0, QColor(34, 74, 111, 70));
    glow.setColorAt(0.5, QColor(8, 13, 21, 0));
    glow.setColorAt(1.0, QColor(70, 36, 56, 80));
    painter.fillRect(panelRect.adjusted(innerInset, innerInset, -innerInset, -innerInset), glow);

    if (!m_board) {
        painter.setPen(QColor("#d8deed"));
        painter.drawText(panelRect, Qt::AlignCenter, QStringLiteral("Board unavailable"));
        return;
    }

    for (int row = 0; row < m_board->rowCount(); ++row) {
        for (int col = 0; col < m_board->columnCount(); ++col) {
            const QRect tileRect = cellRect(row, col);
            const BoardPosition tilePosition{row, col};
            QColor fillColor = m_board->isEnemyHalf(row, col)
                ? QColor(57, 24, 39, 190)
                : QColor(24, 55, 68, 190);
            QColor strokeColor = m_board->isEnemyHalf(row, col)
                ? QColor(146, 90, 98, 120)
                : QColor(103, 157, 164, 120);
            bool illegalDropTarget = false;

            if (m_hoveredPosition == tilePosition) {
                if (m_pendingPlacementUnit) {
                    const bool legal = m_pendingMoveOrigin.isValid()
                        ? m_board->canMoveUnit(m_pendingMoveOrigin.row, m_pendingMoveOrigin.col, row, col)
                            || m_board->canSwapUnits(m_pendingMoveOrigin.row, m_pendingMoveOrigin.col, row, col)
                        : m_board->canPlaceUnit(m_pendingPlacementUnit, row, col);
                    fillColor = legal ? QColor(48, 118, 97, 225) : QColor(132, 34, 49, 235);
                    strokeColor = legal ? QColor("#73efc9") : QColor("#ff6b81");
                    illegalDropTarget = !legal;
                } else {
                    fillColor = m_board->isPlayerHalf(row, col)
                        ? QColor(39, 99, 95, 220)
                        : QColor(108, 47, 59, 220);
                    strokeColor = m_board->isPlayerHalf(row, col)
                        ? QColor("#75e0d0")
                        : QColor("#e0738b");
                }
            }

            if (m_selectedPosition == tilePosition) {
                strokeColor = QColor("#f4d796");
            }

            painter.setPen(QPen(strokeColor, m_selectedPosition == tilePosition ? selectedPenWidth : UiScale::scaled(2)));
            painter.setBrush(fillColor);
            painter.drawRoundedRect(tileRect, tileRadius, tileRadius);

            if (illegalDropTarget) {
                const int illegalInset = UiScale::scaled(3);
                const int illegalCrossOffset = UiScale::scaled(16);
                painter.setPen(QPen(QColor(255, 164, 176, 230), UiScale::scaled(3)));
                painter.drawRoundedRect(tileRect.adjusted(illegalInset, illegalInset, -illegalInset, -illegalInset),
                                        UiScale::scaled(10), UiScale::scaled(10));
                painter.setPen(QPen(QColor(255, 196, 205, 210), UiScale::scaled(2)));
                painter.drawLine(tileRect.topLeft() + QPoint(illegalCrossOffset, illegalCrossOffset),
                                 tileRect.bottomRight() + QPoint(-illegalCrossOffset, -illegalCrossOffset));
                painter.drawLine(tileRect.topRight() + QPoint(-illegalCrossOffset, illegalCrossOffset),
                                 tileRect.bottomLeft() + QPoint(illegalCrossOffset, -illegalCrossOffset));
            }

            painter.setPen(QPen(QColor(255, 255, 255, 16), 1));
            painter.drawLine(tileRect.topLeft() + QPoint(UiScale::scaled(10), UiScale::scaled(8)),
                             tileRect.topRight() + QPoint(-UiScale::scaled(10), UiScale::scaled(8)));
            painter.drawLine(tileRect.bottomLeft() + QPoint(UiScale::scaled(10), -UiScale::scaled(8)),
                             tileRect.bottomRight() + QPoint(-UiScale::scaled(10), -UiScale::scaled(8)));

            if (const UnitPtr unit = m_board->unitAt(row, col)) {
                drawUnit(painter, tileRect, unit);
            }
        }
    }

    const QRect upper = cellRect(3, 0);
    const QRect lower = cellRect(4, 0);
    const int y = (upper.bottom() + lower.top()) / 2;
    painter.setPen(QPen(QColor("#7b7aff"), UiScale::scaled(2)));
    painter.drawLine(panelRect.left() + UiScale::scaled(34), y, panelRect.right() - UiScale::scaled(34), y);
}

void BoardWidget::mouseMoveEvent(QMouseEvent *event)
{
    const BoardPosition pos = boardPositionAt(event->pos());
    if (pos != m_hoveredPosition) {
        m_hoveredPosition = pos;
        update();
    }

    if ((event->buttons() & Qt::LeftButton) && m_board && m_pressedPosition.isValid()
        && (event->pos() - m_dragStartPos).manhattanLength() >= QApplication::startDragDistance()) {
        const UnitPtr unit = m_board->unitAt(m_pressedPosition.row, m_pressedPosition.col);
        if (unit && unit->owner() == ControllerSide::PlayerCtrl) {
            auto *mimeData = new QMimeData;
            mimeData->setData(kBoardDragMime,
                              QByteArray::number(m_pressedPosition.row) + "," + QByteArray::number(m_pressedPosition.col));

            QPixmap pixmap(UiScale::size(88, 88));
            pixmap.fill(Qt::transparent);
            {
                QPainter painter(&pixmap);
                painter.setRenderHint(QPainter::Antialiasing, true);
                painter.setPen(QPen(borderColorForSide(unit->owner()), UiScale::scaled(3)));
                painter.setBrush(fillColorForSide(unit->owner()));
                painter.drawRoundedRect(pixmap.rect().adjusted(UiScale::scaled(4), UiScale::scaled(4),
                                                               -UiScale::scaled(4), -UiScale::scaled(4)),
                                        UiScale::scaled(18), UiScale::scaled(18));
                painter.setPen(QColor("#f8f4eb"));
                painter.setFont(QFont(QStringLiteral("Helvetica"), UiScale::scaled(28), QFont::Bold));
                painter.drawText(pixmap.rect(), Qt::AlignCenter, avatarText(unit));
            }

            QDrag drag(this);
            drag.setMimeData(mimeData);
            drag.setPixmap(pixmap);
            drag.setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
            drag.exec(Qt::MoveAction);
        }
        m_pressedPosition = BoardPosition{};
    }

    QWidget::mouseMoveEvent(event);
}

void BoardWidget::leaveEvent(QEvent *event)
{
    m_hoveredPosition = BoardPosition{};
    update();
    QWidget::leaveEvent(event);
}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_board) {
        QWidget::mousePressEvent(event);
        return;
    }

    const BoardPosition pos = boardPositionAt(event->pos());
    if (event->button() == Qt::LeftButton) {
        m_dragStartPos = event->pos();
        m_pressedPosition = pos;
    }
    if (event->button() == Qt::LeftButton && pos.isValid() && m_board->unitAt(pos.row, pos.col) && !m_pendingPlacementUnit) {
        m_selectedPosition = pos;
    } else if (event->button() == Qt::LeftButton && !m_pendingPlacementUnit) {
        m_selectedPosition = BoardPosition{};
    }
    if (event->button() == Qt::LeftButton) {
        emit selectionChanged();
    }
    if (pos.isValid()) {
        emit tileActivated(pos, event->button());
    }
    update();
    QWidget::mousePressEvent(event);
}

void BoardWidget::dragEnterEvent(QDragEnterEvent *event)
{
    syncDragPreviewFromMime(event->mimeData());
    if (event->mimeData()->hasFormat(kBenchDragMime) || event->mimeData()->hasFormat(kBoardDragMime)) {
        event->acceptProposedAction();
        return;
    }
    event->ignore();
}

void BoardWidget::dragMoveEvent(QDragMoveEvent *event)
{
    m_hoveredPosition = boardPositionAt(event->pos());
    syncDragPreviewFromMime(event->mimeData());
    if (m_hoveredPosition.isValid()
        && (event->mimeData()->hasFormat(kBenchDragMime) || event->mimeData()->hasFormat(kBoardDragMime))) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
    update();
}

void BoardWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    m_hoveredPosition = BoardPosition{};
    clearPendingAction();
    update();
}

void BoardWidget::dropEvent(QDropEvent *event)
{
    const BoardPosition target = boardPositionAt(event->pos());
    if (!target.isValid()) {
        event->ignore();
        return;
    }

    if (event->mimeData()->hasFormat(kBenchDragMime)) {
        const int slot = event->mimeData()->data(kBenchDragMime).toInt();
        emit benchUnitDroppedOnTile(slot, target);
        event->acceptProposedAction();
    } else if (event->mimeData()->hasFormat(kBoardDragMime)) {
        const QList<QByteArray> parts = event->mimeData()->data(kBoardDragMime).split(',');
        if (parts.size() == 2) {
            emit boardUnitDroppedOnTile({parts.at(0).toInt(), parts.at(1).toInt()}, target);
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
    } else {
        event->ignore();
    }

    m_hoveredPosition = BoardPosition{};
    clearPendingAction();
}

QRect BoardWidget::cellRect(int row, int col) const
{
    if (!m_board) {
        return {};
    }

    const QRect contentRect = rect().adjusted(kBoardPadding, kBoardPadding, -kBoardPadding, -kBoardPadding);
    const int totalGapWidth = (m_board->columnCount() - 1) * kCellGap;
    const int totalGapHeight = (m_board->rowCount() - 1) * kCellGap + kMidGap;
    const int cellSize = std::min((contentRect.width() - totalGapWidth) / m_board->columnCount(),
                                  (contentRect.height() - totalGapHeight) / m_board->rowCount());

    const int boardWidth = cellSize * m_board->columnCount() + totalGapWidth;
    const int boardHeight = cellSize * m_board->rowCount() + (m_board->rowCount() - 1) * kCellGap + kMidGap;
    const int startX = contentRect.center().x() - boardWidth / 2;
    const int startY = contentRect.center().y() - boardHeight / 2;

    int y = startY + row * (cellSize + kCellGap);
    if (row >= m_board->rowCount() / 2) {
        y += kMidGap;
    }

    return QRect(startX + col * (cellSize + kCellGap), y, cellSize, cellSize);
}

BoardPosition BoardWidget::boardPositionAt(const QPoint &point) const
{
    if (!m_board) {
        return {};
    }

    for (int row = 0; row < m_board->rowCount(); ++row) {
        for (int col = 0; col < m_board->columnCount(); ++col) {
            if (cellRect(row, col).contains(point)) {
                return {row, col};
            }
        }
    }
    return {};
}

void BoardWidget::drawUnit(QPainter &painter, const QRect &rect, const UnitPtr &unit) const
{
    const QRect avatarRect = rect.adjusted(UiScale::scaled(10), UiScale::scaled(8), -UiScale::scaled(10), -UiScale::scaled(18));
    const QRect hpBarRect(rect.left() + UiScale::scaled(10), rect.bottom() - UiScale::scaled(18),
                          rect.width() - UiScale::scaled(20), UiScale::scaled(5));
    const QRect manaBarRect(rect.left() + UiScale::scaled(10), rect.bottom() - UiScale::scaled(10),
                            rect.width() - UiScale::scaled(20), UiScale::scaled(4));

    painter.setPen(QPen(borderColorForSide(unit->owner()), UiScale::scaled(2)));
    painter.setBrush(fillColorForSide(unit->owner()));
    painter.drawRoundedRect(avatarRect, UiScale::scaled(14), UiScale::scaled(14));

    painter.setPen(QColor("#f8f4eb"));
    painter.setFont(QFont(QStringLiteral("Helvetica"), std::max(UiScale::scaled(12), avatarRect.width() / 4), QFont::Bold));
    painter.drawText(avatarRect, Qt::AlignCenter, avatarText(unit));

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(18, 22, 30, 220));
    painter.drawRoundedRect(hpBarRect, UiScale::scaled(3), UiScale::scaled(3));
    painter.drawRoundedRect(manaBarRect, UiScale::scaled(3), UiScale::scaled(3));

    const qreal hpRatio = unit->maxHp() > 0 ? static_cast<qreal>(unit->hp()) / unit->maxHp() : 0.0;
    const qreal manaRatio = unit->maxMana() > 0 ? static_cast<qreal>(unit->mana()) / unit->maxMana() : 0.0;

    painter.setBrush(QColor("#65d48f"));
    painter.drawRoundedRect(QRect(hpBarRect.left(), hpBarRect.top(),
                                  static_cast<int>(hpBarRect.width() * hpRatio), hpBarRect.height()),
                            UiScale::scaled(3), UiScale::scaled(3));
    painter.setBrush(QColor("#63b1ff"));
    painter.drawRoundedRect(QRect(manaBarRect.left(), manaBarRect.top(),
                                  static_cast<int>(manaBarRect.width() * manaRatio), manaBarRect.height()),
                            UiScale::scaled(3), UiScale::scaled(3));
}

void BoardWidget::syncDragPreviewFromMime(const QMimeData *mimeData)
{
    if (!m_board || !mimeData) {
        clearPendingAction();
        return;
    }

    if (mimeData->hasFormat(kBenchDragMime)) {
        setPendingPlacementUnit(m_board->benchUnitAt(mimeData->data(kBenchDragMime).toInt()));
        return;
    }

    if (mimeData->hasFormat(kBoardDragMime)) {
        const QList<QByteArray> parts = mimeData->data(kBoardDragMime).split(',');
        if (parts.size() == 2) {
            const BoardPosition origin{parts.at(0).toInt(), parts.at(1).toInt()};
            setPendingMoveUnit(m_board->unitAt(origin.row, origin.col), origin);
            return;
        }
    }

    clearPendingAction();
}
