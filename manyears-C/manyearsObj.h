#ifndef MANYEARSOBJ_H
#define MANYEARSOBJ_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QMutexLocker>

#include "audioManager.h"
#include "potentialSourceManager.h"
#include "sourceManager.h"
#include "window.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ManyEars/beamformer.h"
#include "ManyEars/mixture.h"

#ifdef __cplusplus
}
#endif

void updateCallBack(void *objPtr);

class ManyEarsObject : public QObject
{

    Q_OBJECT;

public:

    ManyEarsObject(PotentialSourceManager *_potentialSourceManager, SourceManager *_sourceManager, Window *_windowPlot);

    virtual ~ManyEarsObject();

    void setRecordDevice(int _deviceIndex);

    QString getRecordDeviceName(int _deviceIndex);

    int getRecordDeviceCount();

    void updateInputFrames();

    void findPotentialSources();

    void applyFilter();

    void loadSources();

    void start();

    void stop();

public slots:

    void slotStart();

    void slotStop();

signals:

    // +-------------------------------------------------------+
    // | Confirm operation                                     |
    // +-------------------------------------------------------+

    void confirmOperation();

private:

    AudioManager *recorder;

    int deviceIndex;

    int maximumSources;

    struct objMixture *currentMixture;

    struct objSources *currentSources;

    SourceManager *sourceManager;

    PotentialSourceManager *potentialSourceManager;

    Window *windowPlot;

    bool running;

    QMutex threadMutex;

};

#endif // MANYEARSOBJ_H
