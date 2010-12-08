#ifndef PARAMETERSMANAGER_H
#define PARAMETERSMANAGER_H

#include <QVector>
#include <QString>
#include <QMutex>
#include <QEvent>
#include <QCoreApplication>

#include "property.h"
#include "configurationVector.h"

#ifndef __GNUCC__

#include <math.h>

inline float round(float x)
{
   if (x >= 0.0)
      return floor(x + 0.5f);
     else
      return ceil(x - 0.5f);
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

#include "parameters.h"

#ifdef __cplusplus
}
#endif

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                           x
// x Manager                                                   x
// x                                                           x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX


class ParametersManager: public QObject
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    ParametersManager();

    ParametersManager(const ParametersManager& _parametersManager);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~ParametersManager();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    ParametersManager& operator=(const ParametersManager& _parametersManager);

    /***********************************************************
    * Fixed properties                                         *
    ***********************************************************/

    static const int SAMPLINGRATE = 48000;
    static const int NUMBERMICROPHONES = 8;
    static const int FRAMESIZE = 1024;
    static const int HOPSIZE = 512;
    static const int SIZECOLORPALETTE = 10;

    /***********************************************************
    * Initialize to default values                             *
    ***********************************************************/

    Property* initView();
    Property* initSystem();
    void loadViewDefault();
    void loadSystemDefault();

    /***********************************************************
    * Save parameters to a file                                *
    ***********************************************************/

    void saveParametersSystem(QString _filePath);
    void saveParametersView(QString _filePath);

    /***********************************************************
    * Open parameters from a file                              *
    ***********************************************************/

    void openParametersSystem(QString _filePath);
    void openParametersView(QString _filePath);

    /***********************************************************
    * Get property value                                       *
    ***********************************************************/

    float getPropertyViewFloat(QString _key);
    QString getPropertyViewFloatString(QString _key);
    int getPropertyViewInt(QString _key);
    QString getPropertyViewIntString(QString _key);
    QColor getPropertyViewColor(QString _key);
    QString getPropertyViewColorString(QString _key);
    QString getPropertyViewValueString(QString _key);
    float getPropertySystemFloat(QString _key);
    QString getPropertySystemFloatString(QString _key);
    int getPropertySystemInt(QString _key);
    QString getPropertySystemIntString(QString _key);
    QColor getPropertySystemColor(QString _key);
    QString getPropertySystemColorString(QString _key);
    QString getPropertySystemValueString(QString _key);

    /***********************************************************
    * Set property value                                       *
    ***********************************************************/

    void setPropertyViewFloat(QString _key, float _value);
    void setPropertyViewInt(QString _key, int _value);
    void setPropertyViewColor(QString _key, QColor _value);
    void setPropertyViewValueString(QString _key, QString _value);
    void setPropertySystemFloat(QString _key, float _value);
    void setPropertySystemInt(QString _key, int _value);
    void setPropertySystemColor(QString _key, QColor _value);
    void setPropertySystemValueString(QString _key, QString _value);

    /***********************************************************
    * Copy system parameters values to the core algorithm      *
    ***********************************************************/

    struct ParametersStruct getParametersSystem();

    /***********************************************************
    * Trigger parameters refresh event                         *
    ***********************************************************/

    void triggerParametersRefreshEventSystem();
    void triggerParametersRefreshEventView();

    /***********************************************************
    * Push event receivers                                     *
    ***********************************************************/

    void pushEventReceiverSystem(QObject* _receiver);
    void pushEventReceiverView(QObject* _receiver);

    /***********************************************************
    * Copy parameters values to a configuration vector         *
    ***********************************************************/

    ConfigurationVector copyParametersView();
    ConfigurationVector copyParametersSystem();

protected:

    /***********************************************************
    * Events                                                   *
    ***********************************************************/

    bool event(QEvent *event);

private:

    /***********************************************************
    * Copy parameters values to a configuration vector         *
    ***********************************************************/

    void copyParametersRecursive(Property *_property, ConfigurationVector* _configurationVector);

    /***********************************************************
    * Load parameters values from a configuration vector       *
    ***********************************************************/

    void loadParametersView(ConfigurationVector _configurationVector);
    void loadParametersSystem(ConfigurationVector _configurationVector);

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Properties root                                       |
    // +-------------------------------------------------------+

    ConfigurationVector systemParameters;
    ConfigurationVector viewParameters;

    // +-------------------------------------------------------+
    // | Events receivers                                      |
    // +-------------------------------------------------------+

    QList<QObject*> eventReceiversSystem;
    QList<QObject*> eventReceiversView;

};

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                           x
// x Events                                                    x
// x                                                           x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

class UpdateParametersEvent: public QEvent
{

public:

    /***********************************************************
    * Constants                                                *
    ***********************************************************/

    static const int VIEW = 1;
    static const int SYSTEM = 2;

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    UpdateParametersEvent();
    UpdateParametersEvent(const UpdateParametersEvent& _updateParametersEvent);
    UpdateParametersEvent(const ConfigurationVector _configurationVector, const int _typeVector);


    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~UpdateParametersEvent();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    UpdateParametersEvent& operator=(const UpdateParametersEvent& _updateParametersEvent);

    /***********************************************************
    * Accessor                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Configuration vector                                  |
    // +-------------------------------------------------------+

    ConfigurationVector getConfigurationVector() const;

    // +-------------------------------------------------------+
    // | Type of vector                                        |
    // +-------------------------------------------------------+

    int getTypeVector() const;

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    ConfigurationVector configurationVector;

    int typeVector;

};

#endif // PARAMETERSMANAGER_H
