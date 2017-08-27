#include "MainWindow.h"

#include <iostream>

#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QSettings>

#include "Life.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setCentralWidget(&m_lifeWidget);    
    _setupDock();
    _setupActions();
    _setupMenuBar();
}

MainWindow::~MainWindow() {}

QSize MainWindow::sizeHint() const
{
    if (m_settings.contains("window/size")) {
        return m_settings.value("window/size").toSize();
    } else {
        QSize size;
        QDesktopWidget dw;
        size.setWidth(dw.width() * 0.7);
        size.setHeight(dw.height() * 0.7);
        return size;
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    const QSize size = event->size();
    m_settings.setValue("window/size", size);
}

void MainWindow::_setupDock()
{
    LifeDockWidget *ldw = new LifeDockWidget(this);
    m_dockWidget.setWidget(ldw);
    m_dockWidget.setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

    // FIXME Not the best way to do things
    Life *life = m_lifeWidget.life();

    connect(ldw, &LifeDockWidget::newWidth,
            life, &Life::onNewWidth);
    connect(ldw, &LifeDockWidget::newHeight,
            life, &Life::onNewHeight);
    connect(ldw, &LifeDockWidget::newDepth,
            life, &Life::onNewDepth);

    connect(ldw, &LifeDockWidget::newMinAlive,
            life, &Life::onNewMinAlive);
    connect(ldw, &LifeDockWidget::newMaxAlive,
            life, &Life::onNewMaxAlive);
    connect(ldw, &LifeDockWidget::newMinBirth,
            life, &Life::onNewMinBirth);
    connect(ldw, &LifeDockWidget::newMaxBirth,
            life, &Life::onNewMaxBirth);

    addDockWidget(Qt::RightDockWidgetArea, &m_dockWidget);
}

void MainWindow::_setupActions() {
    m_exitSimulation = new QAction(this);
    m_exitSimulation->setText("E&xit");
    m_exitSimulation->setShortcuts(QKeySequence::Quit);
    m_exitSimulation->setToolTip("Exit QLife");
    connect(m_exitSimulation, &QAction::triggered, this, &QMainWindow::close);

    m_startStopSimulation = new QAction(this);
    m_startStopSimulation->setText("&Start/Stop Simulation");
    m_startStopSimulation->setToolTip("Start/stop simulation");
    connect(m_startStopSimulation, &QAction::triggered, &m_lifeWidget,
            &LifeWidget::startStopSimulation);

    m_restartSimulation = new QAction(this);
    m_restartSimulation->setText("&Restart");
    m_restartSimulation->setToolTip("Restart simulation");
    connect(m_restartSimulation, &QAction::triggered, &m_lifeWidget,
            &LifeWidget::restartSimulation);

    m_startStopCamera = new QAction(this);
    m_startStopCamera->setText("Start/Stop &Camera");
    m_startStopCamera->setToolTip("Start/stop camera");
    connect(m_startStopCamera, &QAction::triggered, &m_lifeWidget,
            &LifeWidget::startStopCamera);

    m_showHideDock = new QAction(this);
    m_showHideDock->setText("Show/hide &Dock");
    m_showHideDock->setToolTip("Show/hide the dock menu");
    connect(m_showHideDock, &QAction::triggered, this, &MainWindow::_showHideDock);
}

void MainWindow::_setupMenuBar() {
    QMenuBar *menu = menuBar();
    menu->addAction(m_exitSimulation);
    menu->addAction(m_startStopSimulation);
    menu->addAction(m_restartSimulation);
    menu->addAction(m_startStopCamera);
    menu->addAction(m_showHideDock);
}

void MainWindow::_showHideDock()
{
    m_dockWidget.isHidden() ? m_dockWidget.show() : m_dockWidget.hide();
}
