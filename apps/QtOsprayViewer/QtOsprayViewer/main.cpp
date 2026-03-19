#include <windows.h>
#include <QApplication>

#include <ospray/ospray.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  int ac = argc;
  const char **av = const_cast<const char **>(argv);

  OSPError err = ospInit(&ac, av);
  if (err != OSP_NO_ERROR) {
    MessageBoxA(nullptr, "ospInit failed", "OSPRay", MB_OK | MB_ICONERROR);
    return 1;
  }

  ospLoadModule("cpu");

  int rc = 0;
  {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    rc = a.exec();
  }

  ospShutdown();
  return rc;
}