# Add more folders to ship with the application, here
folder_01.source = qml/Hypex_Volume
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
           hypex.cpp
win32:      SOURCES += _QT_Libs/hidapi-0.7.0/windows/hid.c
unix:       SOURCES += _QT_Libs/hidapi-0.7.0/linux/hid.c

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
        hypex.h \
        _QT_Libs/hidapi-0.7.0/hidapi/hidapi.h


INCLUDEPATH += _QT_Libs/hidapi-0.7.0/hidapi

win32: LIBS += _QT_Libs/MinGW/libsetupapi.a
unix: LIBS += -ludev

OTHER_FILES += \
