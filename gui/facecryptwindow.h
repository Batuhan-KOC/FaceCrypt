
#ifndef FACECRYPTWINDOW_H
#define FACECRYPTWINDOW_H

#include <QMainWindow>

#include <QTimer>

#include <QStringList>
#include <QList>

#include <QCameraInfo>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsDropShadowEffect>

#include <QPaintEvent>
#include <QPen>
#include <QBrush>

#include "camReader/camReader.h"
#include "fileDialog/fileDialog.h"

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

    // Graphic effects on ui elements
    QList<QGraphicsEffect*> effects;

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

    // Select file button press handler
    void SelectFileButtonPressed();

    // Selected file line edit text changes handler
    void SelectedFileTextChanged(QString file);

    // Enable password ui button check box state change handler
    void EnablePasswordCheckboxStateChanged(int state);

    // Generate password button press handler
    void GeneratePasswordButtonPressed();

    // Generate and return a random password to encrypt file/folder
    QString GenerateRandomPassword();
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
