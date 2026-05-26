#include "mainwindow.h"
#include "profilepage.h"
#include "startpage.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QStackedWidget>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , stackedWidget(nullptr)
    , startPage(nullptr)
    , profilePage(nullptr)
{
    ui->setupUi(this);

    stackedWidget = new QStackedWidget(this);
    startPage = new StartPage(stackedWidget);
    profilePage = new ProfilePage(stackedWidget);

    stackedWidget->addWidget(startPage);
    stackedWidget->addWidget(profilePage);
    stackedWidget->setCurrentWidget(startPage);

    connect(startPage, &StartPage::startGameClicked, this, [this]() {
        qDebug() << "Start game clicked";
        stackedWidget->setCurrentWidget(profilePage);
    });

    connect(profilePage, &ProfilePage::profileConfirmed, this, []() {
    });

    setCentralWidget(stackedWidget);
    setWindowTitle(QStringLiteral("Synera"));
    resize(1520, 980);
    setMinimumSize(1280, 820);
    statusBar()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
