#include <QApplication>
#include "MainWindow.h"

int main(int argc, char **argv)
{

  QSurfaceFormat format;

  format.setSamples(4);
  #if defined( __APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(2);
  #else
    format.setMajorVersion(4);
    format.setMinorVersion(3);
  #endif

  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);

  QSurfaceFormat::setDefaultFormat(format);
  QApplication a(argc, argv);
  MainWindow w;

  w.show();

  return a.exec();

}
