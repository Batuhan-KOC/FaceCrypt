
#ifndef FACECRYPTWINDOW_H
#define FACECRYPTWINDOW_H

#include <QMainWindow>

#include <QTimer>

#include <QStringList>

#include <QCameraInfo>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QPaintEvent>
#include <QPen>
#include <QBrush>

#include "camReader/camReader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FaceCryptWindow; }
QT_END_NAMESPACE

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

    // Graphics Scene to display camera display
    QGraphicsScene* scene;

    // Cam pixmap image
    QGraphicsPixmapItem* pixmap;

    // Camera reader thread
    CamReader* camReader;

    // Return the available and connectable cameras attached on the current pc.
    QStringList GetAvailableCameras();

    // Update available cameras combo box
    void UpdateAvailableCameras();

    // Set default camera during window construction
    void SetDefaultCamera();

    // Set invalid camera message visible
    void SetInvalidCameraSettingsWarning(bool val);

    // Add drop shadows to objects at construction
    void AddDropShadows();

    // Initialize video view mask outline
    void InitializeMaskOutline();

    // Initialize graphics scene and view
    void InitializeVideoView();

    // Initialize camera reader thread
    void InitializeCameraReader();

signals:
    void SourceChanged(int index);

private slots:
    // Cam reader sended a new frame
    void FrameUpdated();

    // New source is not valid
    void SourceInvalid();

    // New source is valid
    void SourceValid();

    // User chosen a new video source
    void SourceChangeRequested(int index);
};

class MaskOverlay : public QWidget
{
    Q_OBJECT
public:
    MaskOverlay(int length, QWidget* parent = nullptr);
    ~MaskOverlay() = default;

protected:
    void paintEvent(QPaintEvent* event) override;

    QPen overlayBorderPen;
    int length;
};

#endif // FACECRYPTWINDOW_H
