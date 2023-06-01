
#ifndef CAMREADER_H
#define CAMREADER_H

#include <QThread>

#include <QObject>

class CamReader : public QThread
{
    Q_OBJECT
public:
    CamReader();
};

#endif // CAMREADER_H
