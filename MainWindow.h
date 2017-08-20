#pragma once

#include <QMainWindow>

#include <QDockWidget>

#include "LifeWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    LifeWidget m_lifeWidget;
};
