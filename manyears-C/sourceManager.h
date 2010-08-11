#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H

#include <QVector>
#include <QMutex>

#include <math.h>

#include "colorPalette.h"
#include "source.h"



class SourceManager
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    SourceManager(int _maxSources, ColorPalette *_colorPalette);

    /***********************************************************
    * Update                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Update the frame for plotting                         |
    // +-------------------------------------------------------+

    void updateFrame();

    // +-------------------------------------------------------+
    // | Add a source for the current frame                    |
    // +-------------------------------------------------------+

    void addSource(int _sourceID, float _x, float _y, float _z);

    // +-------------------------------------------------------+
    // | Reset (clear all sources)                             |
    // +-------------------------------------------------------+

    void reset();

    // +-------------------------------------------------------+
    // | Start                                                 |
    // +-------------------------------------------------------+

    void start();

    // +-------------------------------------------------------+
    // | Stop                                                  |
    // +-------------------------------------------------------+

    void stop();

    /***********************************************************
    * Public fields                                            *
    ***********************************************************/

    int numberSources;
    QVector<float> connectLongitudeAngles;
    QVector<float> connectLatitudeAngles;
    QVector<QColor> connectColors;
    QVector<float> connectXPosition;
    QVector<float> connectYPosition;
    QVector<float> connectZPosition;

    // +-------------------------------------------------------+
    // | Mutex                                                 |
    // +-------------------------------------------------------+

    // Lock the plot for rendering
    //QMutex blockRendering;

private:

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    ColorPalette *sourcePalette;

    QVector<Source> listSources;

    int maxSources;

    bool active;



};

#endif // SOURCEMANAGER_H
