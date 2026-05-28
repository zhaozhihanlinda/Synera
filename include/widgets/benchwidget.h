#ifndef BENCHWIDGET_H
#define BENCHWIDGET_H

#include <QWidget>

#include "core/board.h"

class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;

class BenchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BenchWidget(QWidget *parent = nullptr);

    void setBoard(Board *board);
    void setSelectedSlot(int slot);

signals:
    void slotClicked(int slot);
    void unitPressed(const UnitPtr &unit);
    void boardUnitDroppedToBench(const BoardPosition &position, int slot);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QRect slotRect(int index) const;
    int slotAt(const QPoint &point) const;

    Board *m_board;
    int m_selectedSlot;
    QPoint m_dragStartPos;
    int m_pressedSlot;
};

#endif // BENCHWIDGET_H
