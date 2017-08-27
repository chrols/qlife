#pragma once

#include <QMainWindow>

#include <QAction>
#include <QDockWidget>
#include <QSettings>
#include <QSize>

#include "LifeWidget.h"
#include "LifeDockWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
protected:
    QSize sizeHint() const override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void _setupDock();
    void _setupActions();
    void _setupMenuBar();

    QAction *m_exitSimulation;
    QAction *m_restartSimulation;
    QAction *m_startStopSimulation;
    QAction *m_startStopCamera;
    QAction *m_showHideDock;

    QSettings m_settings;
    LifeWidget m_lifeWidget;
    QDockWidget m_dockWidget;
private slots:
    void _showHideDock();
};
