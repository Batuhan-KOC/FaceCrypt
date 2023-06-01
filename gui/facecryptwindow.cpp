
#include "facecryptwindow.h"
#include "ui_facecryptwindow.h"

#include <QRegion>

#include <QPainter>

#include <QGraphicsDropShadowEffect>

// rgb(1,160,182)

FaceCryptWindow::FaceCryptWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FaceCryptWindow)
{
    ui->setupUi(this);

    InitializeMaskOutline();

    InitializeVideoView();

    AddDropShadows();

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

void FaceCryptWindow::AddDropShadows()
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    QGraphicsDropShadowEffect *effect2 = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setXOffset(-4);
    effect->setYOffset(4);
    effect->setColor(Qt::black);
    effect2->setBlurRadius(5);
    effect2->setXOffset(-4);
    effect2->setYOffset(4);
    effect2->setColor(Qt::black);
    ui->availableCameras->setGraphicsEffect(effect);
    ui->maskOutline->setGraphicsEffect(effect2);
}

void FaceCryptWindow::InitializeMaskOutline()
{
    ui->maskOutline = new MaskOverlay(ui->camView->height(), ui->centralwidget);
    ui->maskOutline->setObjectName(QString::fromUtf8("maskOutline"));
    ui->maskOutline->setGeometry(QRect(ui->camView->x(), ui->camView->y() - 5, ui->camView->width(), ui->camView->height() + 10));
}

void FaceCryptWindow::InitializeVideoView()
{
    scene = new QGraphicsScene();

    pixmap = new QGraphicsPixmapItem();

    scene->addItem(pixmap);

    ui->camView->setScene(scene);

    ui->camView->setFrameStyle(QFrame::NoFrame);

    QRegion viewMask((ui->camView->width() - ui->camView->height()) / 2, 0, ui->camView->height(), ui->camView->height(), QRegion::Ellipse);

    ui->camView->setMask(viewMask);
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

        pixmap->setPixmap(pixIn);

        ui->camView->fitInView(pixmap, Qt::KeepAspectRatio);

        ui->camView->update();
    }
}

MaskOverlay::MaskOverlay(int length, QWidget* parent) : length(length), QWidget(parent)
{
    overlayBorderPen.setColor(Qt::white);

    overlayBorderPen.setWidth(6);
}

void MaskOverlay::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    int w = this->width();
    int h = this->height();

    QRect overlayRect;
    overlayRect.setWidth(length);
    overlayRect.setHeight(length);
    overlayRect.moveCenter(QPoint(w/2, h/2));

    painter.setPen(overlayBorderPen);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawEllipse(overlayRect);
}
