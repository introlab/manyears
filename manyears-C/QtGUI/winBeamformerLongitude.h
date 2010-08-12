#ifndef WINBEAMFORMERLONGITUDE_H
#define WINBEAMFORMERLONGITUDE_H

#include <QMdiSubWindow>
#include <QGridLayout>

#include "PlotBeamformerLongitude.h"
#include "sourceManager.h"

//class WinBeamformerLongitude : public QMdiSubWindow
//{
//
//public:
//
//    /***********************************************************
//    * Constructor                                              *
//    ***********************************************************/
//
//    WinBeamformerLongitude(int _width, int _height, int _samplingRate, int _hopSize, PotentialSourceManager *_sourceManager);
//
//    /***********************************************************
//    * Destructor                                               *
//    ***********************************************************/
//
//    ~WinBeamformerLongitude();
//
//    /***********************************************************
//    * Refresh                                                  *
//    ***********************************************************/
//
//    void updatePlot();
//    void initPlot();
//
//    /***********************************************************
//    * Mutator                                                  *
//    ***********************************************************/
//
//    // +-------------------------------------------------------+
//    // | Time scale                                            |
//    // +-------------------------------------------------------+
//
//    void setXMin(float _xMin);
//    void setXMax(float _xMax);
//    void setXInterval(float _xInterval);
//
//    // +-------------------------------------------------------+
//    // | Background                                            |
//    // +-------------------------------------------------------+
//
//    void setColorBackground(QColor _colorBackground);
//
//    // +-------------------------------------------------------+
//    // | Trace                                                 |
//    // +-------------------------------------------------------+
//
//    void setSizePoint(int _sizePoint);
//
//    // +-------------------------------------------------------+
//    // | Axes                                                  |
//    // +-------------------------------------------------------+
//
//    void setColorHorizontalBar(QColor _colorHorizontalBar);
//    void setColorVerticalBar(QColor _colorVerticalBar);
//
//private:
//
//    /***********************************************************
//    * Private fields                                           *
//    ***********************************************************/
//
//    QGridLayout *mainGrid;
//    QWidget *tmpWidget;
//    int width;
//    int height;
//    PlotBeamformerLongitude *mainPlot;
//
//    int samplingRate;
//    int hopSize;
//
//    PotentialSourceManager *mainManager;
//
//};

#endif // WINLATITUDE_H
