
#ifndef FACECRYPTWINDOW_H
#define FACECRYPTWINDOW_H

#include <QMainWindow>

#include <QTimer>

#include <QStringList>

#include <QCameraInfo>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "opencv2/opencv.hpp"
#include "opencv2/opencv_modules.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class FaceCryptWindow; }
QT_END_NAMESPACE

#define ONE_SECOND_TO_MILLISECONDS 1000.0

class FaceCryptWindow : public QMainWindow
{
    Q_OBJECT

public:
    FaceCryptWindow(QWidget *parent = nullptr);
    ~FaceCryptWindow();

private:
    Ui::FaceCryptWindow *ui;

    // Camera info and description provided by Qt
    QCameraInfo currentCameraInfo;

    // Active camera video capture object provided by Open cv
    cv::VideoCapture* capture;

    // Current camera frame rate
    double frameRate;

    // Timer to read frames from the capture object provided by Open cv
    QTimer frameRateTimer;

    // Graphics Scene to display camera display
    QGraphicsScene* scene;

    // Cam pixmap image
    QGraphicsPixmapItem* pixmap;

    // Return the available and connectable cameras attached on the current pc.
    QStringList GetAvailableCameras();

    // Update available cameras combo box
    void UpdateAvailableCameras();

    // Set default camera during window construction
    void SetDefaultCamera();

    // Set invalid camera message visible
    void SetInvalidCameraSettingsWarning(bool val);

public slots:
    void frameRateTimeout();
};

#endif // FACECRYPTWINDOW_H