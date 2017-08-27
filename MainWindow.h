#pragma once

#include <QMainWindow>

#include <QAction>
#include <QDockWidget>
#include <QSettings>
#include <QSize>

#include "LifeWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
protected:
    QSize sizeHint() const override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void _setupActions();
    void _setupMenuBar();

    QAction *m_exitSimulation;
    QAction *m_restartSimulation;
    QAction *m_startStopSimulation;
    QAction *m_startStopCamera;

    QSettings m_settings;
    LifeWidget m_lifeWidget;
};
