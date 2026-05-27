#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>
#include <QHash>

#include "core/pageid.h"

class QStackedWidget;
class QWidget;

class PageManager : public QObject
{
    Q_OBJECT

public:
    explicit PageManager(QStackedWidget *stackedWidget, QObject *parent = nullptr);

    void registerPage(PageId pageId, QWidget *page);
    void switchTo(PageId pageId);
    QWidget *page(PageId pageId) const;

private:
    QStackedWidget *m_stackedWidget;
    QHash<int, QWidget *> m_pages;
};

#endif // PAGEMANAGER_H
