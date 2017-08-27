#include "MainWindow.h"

#include <iostream>

#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include "Life.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setCentralWidget(&m_lifeWidget);
    _setupActions();
    _setupMenuBar();
}

MainWindow::~MainWindow() {}

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
}

void MainWindow::_setupMenuBar() {
    QMenuBar *menu = menuBar();
    menu->addAction(m_exitSimulation);
    menu->addAction(m_startStopSimulation);
    menu->addAction(m_restartSimulation);
    menu->addAction(m_startStopCamera);
}
