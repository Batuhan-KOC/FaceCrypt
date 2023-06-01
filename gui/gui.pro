QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OPENCV_INSTALLATION_PATH=C:/libs/OPENCV/Installation

message($$OPENCV_INSTALLATION_PATH)

SOURCES += \
    camReader/camReader.cpp \
    main.cpp \
    facecryptwindow.cpp

HEADERS += \
    camReader/camReader.h \
    facecryptwindow.h

FORMS += \
    facecryptwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$OPENCV_INSTALLATION_PATH/include
DEPENDPATH += $$OPENCV_INSTALLATION_PATH/include

win32{
CONFIG(release, debug|release) {
    LIBS += -L OPENCV_INSTALLATION_PATH/x64/vc16/lib -lopencv_core470 -lopencv_imgproc470 -lopencv_highgui470 -lopencv_imgcodecs470 -lopencv_videoio470 -lopencv_video470 -lopencv_calib3d470 -lopencv_photo470 -lopencv_features2d470

    INCLUDEPATH += $$OPENCV_INSTALLATION_PATH/include
    DEPENDPATH += $$OPENCV_INSTALLATION_PATH/include

    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_core470.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_imgproc470.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_highgui470.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_imgcodecs470.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_videoio470.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_video470.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_calib3d470.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_photo470.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_features2d470.lib
} else {
    LIBS += -L$$OPENCV_INSTALLATION_PATH/x64/vc16/lib -lopencv_core470d -lopencv_imgproc470d -lopencv_highgui470d -lopencv_imgcodecs470d -lopencv_videoio470d -lopencv_video470d -lopencv_calib3d470d -lopencv_photo470d -lopencv_features2d470d

    INCLUDEPATH += $$OPENCV_INSTALLATION_PATH/include
    DEPENDPATH += $$OPENCV_INSTALLATION_PATH/include

    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_core470d.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_imgproc470d.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_highgui470d.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_imgcodecs470d.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_videoio470d.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_video470d.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_calib3d470d.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_photo470d.lib
    PRE_TARGETDEPS += $$OPENCV_INSTALLATION_PATH/x64/vc16/lib/opencv_features2d470d.lib
}
}

RESOURCES += \
    resource/resource.qrc
