#include "mainwindow.h"
#include "renderwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *w = new RenderWidget(this);
    setCentralWidget(w);
    resize(1000, 700);
}

