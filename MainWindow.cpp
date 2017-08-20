#include "MainWindow.h"

#include <iostream>

#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include "Life.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setCentralWidget(&m_lifeWidget);
}

MainWindow::~MainWindow() {}
