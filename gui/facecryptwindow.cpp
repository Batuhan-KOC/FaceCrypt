
#include "facecryptwindow.h"
#include "ui_facecryptwindow.h"

FaceCryptWindow::FaceCryptWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FaceCryptWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();

    pixmap = new QGraphicsPixmapItem();

    scene->addItem(pixmap);

    ui->camView->setScene(scene);

    ui->camView->setFrameStyle(QFrame::NoFrame);

    connect(&frameRateTimer, &QTimer::timeout, this, &FaceCryptWindow::frameRateTimeout);

    UpdateAvailableCameras();

    SetDefaultCamera();
}

FaceCryptWindow::~FaceCryptWindow()
{
    delete ui;
}

QStringList FaceCryptWindow::GetAvailableCameras()
{
    QStringList availableCameras;

    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : cameras){
        availableCameras.append(cameraInfo.description());
    }

    return availableCameras;
}

void FaceCryptWindow::UpdateAvailableCameras()
{
    QStringList availableCameras = GetAvailableCameras();

    ui->availableCameras->clear();

    ui->availableCameras->addItems(availableCameras);
}

void FaceCryptWindow::SetDefaultCamera()
{
    bool cameraSet{false};
    int cameraIndex{0};

    if(ui->availableCameras->count() > 0){
        for(int index = 0; index < ui->availableCameras->count() ; index++){
            capture = new cv::VideoCapture(cameraIndex);

            if(!capture->isOpened()){
                continue;
            }
            else{
                cameraSet = true;

                frameRate = capture->get(cv::CAP_PROP_FPS);

                frameRateTimer.setInterval(static_cast<int>(static_cast<double>(ONE_SECOND_TO_MILLISECONDS) / frameRate));

                frameRateTimer.start();

                break;
            }
        }
    }

    if(!cameraSet){
        SetInvalidCameraSettingsWarning(true);
    }
    else{

    }
}

void FaceCryptWindow::SetInvalidCameraSettingsWarning(bool val)
{

}

void FaceCryptWindow::frameRateTimeout()
{
    cv::Mat frame;

    bool success = capture->read(frame);

    if(success){
        static QImage imgIn;
        imgIn = QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);

        static QPixmap pixIn;
        pixIn.convertFromImage(imgIn);

        bool ress = pixIn.save("ananke.jpg", "JPG");

        qDebug() << ress << "   " << pixIn.width() << " " << pixIn.height();

        pixmap->setPixmap(pixIn);

        ui->camView->fitInView(pixmap, Qt::KeepAspectRatio);

        ui->camView->update();
    }
}


