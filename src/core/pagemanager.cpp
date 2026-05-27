#include "core/pagemanager.h"

#include <QStackedWidget>
#include <QWidget>

PageManager::PageManager(QStackedWidget *stackedWidget, QObject *parent)
    : QObject(parent)
    , m_stackedWidget(stackedWidget)
{
}

void PageManager::registerPage(PageId pageId, QWidget *page)
{
    if (!m_stackedWidget || !page) {
        return;
    }

    if (m_stackedWidget->indexOf(page) == -1) {
        m_stackedWidget->addWidget(page);
    }

    m_pages.insert(static_cast<int>(pageId), page);
}

void PageManager::switchTo(PageId pageId)
{
    if (!m_stackedWidget) {
        return;
    }

    QWidget *targetPage = page(pageId);
    if (targetPage) {
        m_stackedWidget->setCurrentWidget(targetPage);
    }
}

QWidget *PageManager::page(PageId pageId) const
{
    return m_pages.value(static_cast<int>(pageId), nullptr);
}
