#ifndef CORETHREAD_H
#define CORETHREAD_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QMutex>
#include <QSemaphore>

#include <QDebug>

#include "inputOutputManager.h"
#include "sourceEvents.h"
#include "parametersManager.h"
#include "sourceBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "parameters.h"
#include "overallContext.h"

#ifdef __cplusplus
}
#endif

class CoreThread: public QThread
{

    Q_OBJECT

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    CoreThread();

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~CoreThread();

    /***********************************************************
    * System parameters                                        *
    ***********************************************************/    

    void loadParameters(struct ParametersStruct _parameters);

    /***********************************************************
    * Input/Output manager                                     *
    ***********************************************************/

    void setInputOuputManager(InputOutputManager* _manager);

    /***********************************************************
    * Thread control                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Start                                                 |
    // +-------------------------------------------------------+

    void startThread();

    // +-------------------------------------------------------+
    // | Stop                                                  |
    // +-------------------------------------------------------+

    void stopThread();

    // +-------------------------------------------------------+
    // | Run function                                          |
    // +-------------------------------------------------------+

    virtual void run();

    /***********************************************************
    * Events receivers                                         *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Potential sources                                     |
    // +-------------------------------------------------------+

    void pushPotentialSourceEventReceiver(QObject* _eventReceiver);

    // +-------------------------------------------------------+
    // | Tracked sources                                       |
    // +-------------------------------------------------------+

    void pushTrackedSourceEventReceiver(QObject* _eventReceiver);

protected:

    /***********************************************************
    * Events                                                   *
    ***********************************************************/

    bool event(QEvent *event);

private:

    /***********************************************************
    * Thread access to shared fields                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Flags that the thread needs to stop                   |
    // +-------------------------------------------------------+

    void askToStopAndWait();

    // +-------------------------------------------------------+
    // | Flag that stop has been performed                     |
    // +-------------------------------------------------------+

    void stopIsDone();

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Input/Output Manager                                  |
    // +-------------------------------------------------------+

    InputOutputManager* inputOutputManager;

    // +-------------------------------------------------------+
    // | Context for the library ManyEars                      |
    // +-------------------------------------------------------+

    struct objOverall libraryContext;
    struct ParametersStruct libraryParameters;

    // +-------------------------------------------------------+
    // | Receivers for results                                 |
    // +-------------------------------------------------------+

    QList<QObject*> receiverPotentialSources;
    QList<QObject*> receiverTrackedSources;

    // +-------------------------------------------------------+
    // | Mutex/Semaphores                                      |
    // +-------------------------------------------------------+

    QSemaphore semaStopDone;

};

#endif
