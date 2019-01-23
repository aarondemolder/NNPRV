TARGET=nodenoisepreview

OBJECTS_DIR=obj

QT+=gui opengl core

cache()

MOC_DIR=moc
CONFIG-=app_bundle

SOURCES+= $$PWD/src/NGLScene.cpp \
            $$PWD/src/MainWindow.cpp \
            $$PWD/src/main.cpp


HEADERS+= $$PWD/include/NGLScene.h \
          $$PWD/include/MainWindow.h


FORMS+= $$PWD/ui/MainWindow.ui

INCLUDEPATH +=./include

DESTDIR=./
# add the glsl shader files
OTHER_FILES+= README.md \
              shaders/*.glsl


CONFIG += console
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){
	message("including $HOME/NGL")
	include($(HOME)/NGL/UseNGL.pri)
}
else{
	message("Using custom NGL location")
	include($(NGLDIR)/UseNGL.pri)
}
