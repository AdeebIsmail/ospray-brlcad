#include <QApplication>
#include <QDebug>

#include <ospray/ospray.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  // OSPRay wants argc/argv pointers
  int ac = argc;
  const char **av = const_cast<const char **>(argv);

  OSPError err = ospInit(&ac, av);
  if (err != OSP_NO_ERROR) {
    qCritical() << "ospInit failed with error code" << int(err);
    return 1;
  }

  // Optional but safe on Windows
  ospLoadModule("cpu");

  int rc = 0;
  {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    rc = a.exec();
  } // MainWindow + RenderWidget destruct here (OSPRay objects gone)

  ospShutdown(); // now safe, device still valid until here
  return rc;
}