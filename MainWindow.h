#pragma once

#include <QMainWindow>

#include <QAction>
#include <QDockWidget>

#include "LifeWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    void _setupActions();
    void _setupMenuBar();

    QAction *m_exitSimulation;
    QAction *m_restartSimulation;
    QAction *m_startStopSimulation;
    QAction *m_startStopCamera;

    LifeWidget m_lifeWidget;
};
