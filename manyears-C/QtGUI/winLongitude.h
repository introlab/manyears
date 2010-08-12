#ifndef WINLONGITUDE_H
#define WINLONGITUDE_H

#include <QMdiSubWindow>
#include <QGridLayout>

#include "plotLongitude.h"
#include "sourceManager.h"

class WinLongitude : public QMdiSubWindow
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    WinLongitude(int _width, int _height, int _samplingRate, int _hopSize, SourceManager *_sourceManager);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~WinLongitude();

    /***********************************************************
    * Refresh                                                  *
    ***********************************************************/

    void updatePlot();
    void initPlot();

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Time scale                                            |
    // +-------------------------------------------------------+

    void setXMin(float _xMin);
    void setXMax(float _xMax);
    void setXInterval(float _xInterval);

    // +-------------------------------------------------------+
    // | Background                                            |
    // +-------------------------------------------------------+

    void setColorBackground(QColor _colorBackground);

    // +-------------------------------------------------------+
    // | Trace                                                 |
    // +-------------------------------------------------------+

    void setSizePoint(int _sizePoint);

    // +-------------------------------------------------------+
    // | Axes                                                  |
    // +-------------------------------------------------------+

    void setColorHorizontalBar(QColor _colorHorizontalBar);
    void setColorVerticalBar(QColor _colorVerticalBar);

private:

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    QGridLayout *mainGrid;
    QWidget *tmpWidget;
    int width;
    int height;
    PlotLongitude *mainPlot;

    int samplingRate;
    int hopSize;

    SourceManager *mainManager;

};

#endif // WINLONGITUDE_H
