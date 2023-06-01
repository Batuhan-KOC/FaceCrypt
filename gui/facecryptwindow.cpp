
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

    InitializeCameraReader();

    AddDropShadows();

    UpdateAvailableCameras();

    SetInvalidCameraSettingsWarning(true);

    SetDefaultCamera();

    connect(ui->availableCameras, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FaceCryptWindow::SourceChangeRequested);
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
    if(ui->availableCameras->count() > 0){
        emit SourceChanged(0);
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

void FaceCryptWindow::InitializeCameraReader()
{
    camReader = new CamReader();

    connect(camReader, &CamReader::FrameUpdated, this, &FaceCryptWindow::FrameUpdated, Qt::QueuedConnection);
    connect(camReader, &CamReader::SourceInvalid, this, &FaceCryptWindow::SourceInvalid, Qt::QueuedConnection);
    connect(camReader, &CamReader::SourceValid, this, &FaceCryptWindow::SourceValid, Qt::QueuedConnection);
    connect(this, &FaceCryptWindow::SourceChanged, camReader, &CamReader::SourceChanged, Qt::QueuedConnection);

    camReader->start();
}

void FaceCryptWindow::FrameUpdated()
{
    static QPixmap _pixmap;

    CamReader::frameLock.lock();
    _pixmap = *CamReader::capturedPixmap;
    CamReader::frameLock.unlock();

    pixmap->setPixmap(_pixmap);

    ui->camView->fitInView(pixmap, Qt::KeepAspectRatio);

    ui->camView->update();
}

void FaceCryptWindow::SourceInvalid()
{

}

void FaceCryptWindow::SourceValid()
{

}

void FaceCryptWindow::SourceChangeRequested(int index)
{
    emit SourceChanged(index);
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
