#-------------------------------------------------
#
# Project created by QtCreator 2010-11-05T14:00:55
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = final
TEMPLATE = app
CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    drawengine.cpp \
    glm.cpp \
    math/testMatrix.cpp \
    math/CS123Vector.inl \
    math/CS123Matrix.inl \
    math/CS123Matrix.cpp \
    shot/shot.cpp \
    targa.cpp \
    shot/testShot.cpp \
    shot/orbshot.cpp \
    Structs.cpp \
    shot/pshapes.cpp \
    shot/polarshapes.cpp \
    nmsphere.cpp \
    nmplane.cpp \
    shot/spiralorbshot.cpp \
    shot/intronailshot.cpp \
    shot/tempshot.cpp \
    shot/upropeshot.cpp \
    shot/nailsinfield.cpp \
    shot/polaranimated.cpp \
    shot/polaranimated2.cpp \
    shot/polarclusters.cpp \
    shot/twonailballs.cpp \
    shot/threenailballs.cpp \
    shot/particleorbs.cpp \
    shot/manynailballs.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    drawengine.h \
    glm.h \
    common.h \
    math/testMatrix.h \
    math/CS123Vector.h \
    math/CS123Matrix.h \
    math/CS123Algebra.h \
    lib/CS123Common.h \
    shot/shot.h \
    targa.h \
    shot/testShot.h \
    shot/orbshot.h \
    shot/pshapes.h \
    shot/polarshapes.h \
    nmsphere.h \
    nmplane.h \
    shot/spiralorbshot.h \
    shot/intronailshot.h \
    shot/tempshot.h \
    shot/upropeshot.h \
    shot/nailsinfield.h \
    shot/polaranimated.h \
    shot/polaranimated2.h \
    shot/polarclusters.h \
    shot/twonailballs.h \
    shot/threenailballs.h \
    shot/particleorbs.h \
    shot/manynailballs.h

FORMS    += mainwindow.ui
INCLUDEPATH += lib math shot
DEPENDPATH += lib math shot
RESOURCES +=
