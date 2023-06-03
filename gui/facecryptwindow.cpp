
#include "facecryptwindow.h"
#include "ui_facecryptwindow.h"

#include <QRegion>

#include <QPainter>

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

    connect(ui->selectFileButton, &QPushButton::pressed, this, &FaceCryptWindow::SelectFileButtonPressed);

    connect(ui->selectedFile, &QLineEdit::textChanged, this, &FaceCryptWindow::SelectedFileTextChanged);

    connect(ui->enablePasswordButton, &QCheckBox::stateChanged, this, &FaceCryptWindow::EnablePasswordCheckboxStateChanged);

    connect(ui->generatePasswordButton, &QPushButton::pressed, this, &FaceCryptWindow::GeneratePasswordButtonPressed);
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
    for(int index = 0 ; index < 7 ; index++){
        effects.append(new QGraphicsDropShadowEffect);

        static_cast<QGraphicsDropShadowEffect*>(effects.at(index))->setBlurRadius(5);
        static_cast<QGraphicsDropShadowEffect*>(effects.at(index))->setXOffset(-4);
        static_cast<QGraphicsDropShadowEffect*>(effects.at(index))->setYOffset(4);
        static_cast<QGraphicsDropShadowEffect*>(effects.at(index))->setColor(Qt::darkGray);
    }

    ui->availableCameras->setGraphicsEffect(effects.at(0));
    ui->maskOutline->setGraphicsEffect(effects.at(1));
    ui->selectedFile->setGraphicsEffect(effects.at(2));
    ui->selectFileButton->setGraphicsEffect(effects.at(3));
    ui->password->setGraphicsEffect(effects.at(4));
    ui->generatePasswordButton->setGraphicsEffect(effects.at(5));
    ui->encryptButton->setGraphicsEffect(effects.at(6));
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

void FaceCryptWindow::SelectFileButtonPressed()
{
    FileDialog dialog;

    int success = dialog.exec();

    if(success){
        ui->selectedFile->setText(dialog.selectedFiles().front());
        ui->encryptButton->setEnabled(true);
    }
    else{
        ui->selectedFile->clear();
        ui->encryptButton->setEnabled(false);
    }
}

void FaceCryptWindow::SelectedFileTextChanged(QString file)
{
    if(file.isEmpty()){
        ui->enablePasswordButton->setChecked(false);
        ui->enablePasswordButton->setEnabled(false);
        ui->generatePasswordButton->setEnabled(false);
        ui->password->clear();
        ui->password->setEnabled(false);
        ui->encryptButton->setEnabled(false);
    }
    else{
        ui->enablePasswordButton->setEnabled(true);
    }
}

void FaceCryptWindow::EnablePasswordCheckboxStateChanged(int state)
{
    if(state == Qt::Unchecked){
        ui->password->clear();
        ui->password->setEnabled(false);
        ui->generatePasswordButton->setEnabled(false);
    }
    else if(state == Qt::Checked){
        ui->password->clear();
        ui->password->setEnabled(true);
        ui->generatePasswordButton->setEnabled(true);
    }
}

void FaceCryptWindow::GeneratePasswordButtonPressed()
{
     ui->password->setText(GenerateRandomPassword());
}

QString FaceCryptWindow::GenerateRandomPassword()
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!'^+%&/()=?_#${[]}\|<>/*-");
    const int randomStringLength = 100;

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
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
