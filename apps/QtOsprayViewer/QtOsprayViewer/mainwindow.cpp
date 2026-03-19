#include "mainwindow.h"
#include "renderwidget.h"

#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  renderWidget_ = new RenderWidget(this);
  setCentralWidget(renderWidget_);
  resize(1100, 750);

  setupMenus();
  setWindowTitle("QtOsprayViewer - MainWindow");
}

void MainWindow::setupMenus()
{
  QMenu *fileMenu = menuBar()->addMenu("&File");
  menuBar()->setVisible(true);

  QAction *openAction = new QAction("&Open Model...", this);
  fileMenu->addAction(openAction);

  QAction *resetViewAction = new QAction("&Reset View", this);
  fileMenu->addAction(resetViewAction);

  fileMenu->addSeparator();

  QAction *exitAction = new QAction("E&xit", this);
  fileMenu->addAction(exitAction);

  connect(openAction, &QAction::triggered, this, [this]() {
    QString path = QFileDialog::getOpenFileName(this,
        "Open Model",
        QString(),
        "Model Files (*.obj *.g *.stl *.ply);;OBJ Files (*.obj);;BRL-CAD Files (*.g)");

    if (path.isEmpty())
      return;

    QFileInfo info(path);
    const QString ext = info.suffix().toLower();

    if (ext == "obj") {
      if (!renderWidget_->loadModel(path)) {
        QMessageBox::warning(this, "Load Failed", "Could not load OBJ file.");
      }
      return;
    }

    if (ext == "g") {
      QMessageBox::information(this,
          "Not Yet Implemented",
          "BRL-CAD .g loading is planned next. OBJ loading works now.");
      return;
    }

    QMessageBox::information(this,
        "Not Yet Implemented",
        "That file type is not implemented yet. Start with OBJ for testing.");
  });

  connect(resetViewAction, &QAction::triggered, this, [this]() {
    renderWidget_->resetView();
  });

  connect(exitAction, &QAction::triggered, this, [this]() { close(); });
}