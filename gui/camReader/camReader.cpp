#include "camReader.h"

QPixmap* CamReader::capturedPixmap {nullptr};
QMutex CamReader::frameLock{};
QMutex CamReader::sourceLock{};

CamReader::CamReader(QObject* parent) : QThread(parent)
{

}

CamReader::~CamReader()
{
    capture->release();

    delete capture;

    delete capturedPixmap;

    frameLock.unlock();
    sourceLock.unlock();
}

void CamReader::run()
{
    capturedPixmap = new QPixmap();

    while(true){
        sourceLock.lock();

        if(readyToRead){
            cv::Mat frame;

            bool success = capture->read(frame);

            if(success){
                capturedFrame = QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);

                frameLock.lock();
                capturedPixmap->convertFromImage(capturedFrame);
                frameLock.unlock();

                emit FrameUpdated();
            }
            else{
                capture->release();

                delete capture;

                capture = nullptr;

                readyToRead = false;

                emit SourceInvalid();
            }
        }

        sourceLock.unlock();
    }
}

void CamReader::SourceChanged(int index)
{
    sourceLock.lock();

    readyToRead = false;

    if(capture != nullptr){
        if(capture->isOpened()){
            capture->release();
        }
    }

    capture = new cv::VideoCapture(index, cv::CAP_DSHOW);

    if(!capture->isOpened()){
        delete capture;

        capture = nullptr;

        emit SourceInvalid();
    }
    else{
        frameRate = capture->get(cv::CAP_PROP_FPS);

        readyToRead = true;

        emit SourceValid();
    }

    sourceLock.unlock();
}

