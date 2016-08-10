#ifndef WINSPHERE_H
#define WINSPHERE_H

#include <QMdiSubWindow>
#include <QGridLayout>

#include "spherePlot.h"
#include "sourceManager.h"

class WinSphere : public QMdiSubWindow
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    WinSphere(int _width, int _height, int _samplingRate, int _hopSize, SourceManager *_sourceManager, int _updateFactor);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~WinSphere();

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Set the background color                              |
    // +-------------------------------------------------------+

    void setBackgroundColor(QColor _color);

    // +-------------------------------------------------------+
    // | Set the top grid color                                |
    // +-------------------------------------------------------+

    void setTopGridColor(QColor _color);

    // +-------------------------------------------------------+
    // | Set the bottom grid color                             |
    // +-------------------------------------------------------+

    void setBottomGridColor(QColor _color);

    /***********************************************************
    * Refresh                                                  *
    ***********************************************************/

    void updatePlot();



private:

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    int width;
    int height;
    SpherePlot *mainPlot;

    int samplingRate;
    int hopSize;

    int updateFactor;
    int updateCounter;

    SourceManager *mainManager;

};

#endif // WINSPHERE_H
