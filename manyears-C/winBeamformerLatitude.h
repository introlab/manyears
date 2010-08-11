#ifndef WINBEAMFORMERLATITUDE_H
#define WINBEAMFORMERLATITUDE_H

#include <QMdiSubWindow>
#include <QGridLayout>

#include "PlotBeamformerLatitude.h"
#include "sourceManager.h"

//class WinBeamformerLatitude : public QMdiSubWindow
//{
//
//public:
//
//    /***********************************************************
//    * Constructor                                              *
//    ***********************************************************/
//
//    WinBeamformerLatitude(int _width, int _height, int _samplingRate, int _hopSize, PotentialSourceManager *_sourceManager);
//
//    /***********************************************************
//    * Destructor                                               *
//    ***********************************************************/
//
//    ~WinBeamformerLatitude();
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
//    PlotBeamformerLatitude *mainPlot;
//
//    int samplingRate;
//    int hopSize;
//
//    PotentialSourceManager *mainManager;
//
//};

#endif // WINLATITUDE_H
