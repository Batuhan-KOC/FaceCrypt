
#ifndef CAMREADER_H
#define CAMREADER_H

#include <QThread>
#include <QMutex>

#include <QImage>
#include <QPixmap>

#include <chrono>

#include "opencv2/opencv.hpp"
#include "opencv2/opencv_modules.hpp"

#define ONE_SECOND_TO_MILLISECONDS 1000.0

class CamReader : public QThread
{
    Q_OBJECT
public:
    CamReader(QObject* parent = nullptr);
    virtual ~CamReader() override;

    // Readed frame from the cv::VideoCapture* capture object
    QImage capturedFrame{};

    // capturedFrame converted to pixmap
    static QPixmap* capturedPixmap;

    // Keep QImage frame object safe
    static QMutex frameLock;

    // Keep cv::VideoCapture* capture object safe
    static QMutex sourceLock;

protected:
    void run() override;

private:
    // Active camera video capture object provided by Open cv
    cv::VideoCapture* capture{nullptr};

    // Current camera frame rate
    double frameRate;

    // Thread is ready to read capture device or not
    bool readyToRead{false};

    // Time points to calculate passed time
    std::chrono::time_point<std::chrono::steady_clock> tp1, tp2;

signals:
    // Notify main window that a new frame is available
    void FrameUpdated();

    // Notify main window that the new source is not valid
    void SourceInvalid();

    // Notify main window that the new source is valid
    void SourceValid();

public slots:
    void SourceChanged(int index);
};

#endif // CAMREADER_H
