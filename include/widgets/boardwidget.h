#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>

#include "core/board.h"

class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class QMimeData;

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWidget(QWidget *parent = nullptr);

    void setBoard(Board *board);
    void setEnemyUnitsVisible(bool visible);
    void setBattleVisualMode(bool enabled);
    void setPendingPlacementUnit(const UnitPtr &unit);
    void setPendingMoveUnit(const UnitPtr &unit, const BoardPosition &origin);
    void clearPendingAction();
    BoardPosition selectedPosition() const;
    void setSelectedPosition(const BoardPosition &position);
    UnitPtr selectedUnit() const;

signals:
    void selectionChanged();
    void unitPressed(const UnitPtr &unit);
    void tileActivated(const BoardPosition &position, Qt::MouseButton button);
    void benchUnitDroppedOnTile(int slot, const BoardPosition &position);
    void boardUnitDroppedOnTile(const BoardPosition &from, const BoardPosition &to);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QRect cellRect(int row, int col) const;
    BoardPosition boardPositionAt(const QPoint &point) const;
    void drawUnit(QPainter &painter, const QRect &rect, const UnitPtr &unit) const;
    void syncDragPreviewFromMime(const QMimeData *mimeData);

    Board *m_board;
    bool m_enemyUnitsVisible;
    bool m_battleVisualMode;
    UnitPtr m_pendingPlacementUnit;
    BoardPosition m_pendingMoveOrigin;
    BoardPosition m_hoveredPosition;
    BoardPosition m_selectedPosition;
    QPoint m_dragStartPos;
    BoardPosition m_pressedPosition;
};

#endif // BOARDWIDGET_H
