#ifndef PLOTLATITUDE_H
#define PLOTLATITUDE_H

#include <QWidget>
#include <QGridLayout>

#include "plot.h"
#include "sourceManager.h"

class PlotLatitude : public QWidget
{

    Q_OBJECT

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    PlotLatitude(int _width, int _height, int _samplingRate, int _hopSize, SourceManager *_sourceManager);

    /***********************************************************
    * Update graph                                             *
    ***********************************************************/

    void initPlot();
    void updateGraph();

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

public slots:

    /***********************************************************
    * Update graph slot                                        *
    ***********************************************************/

    void updateGraphSlot();

private:

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Objects                                               |
    // +-------------------------------------------------------+

    SourceManager *currentSourceManager;

    Plot *activePlot;

    QGridLayout *mainGrid;

    // +-------------------------------------------------------+
    // | General                                               |
    // +-------------------------------------------------------+

    int width;
    int height;
    int samplingRate;
    int hopSize;

    // +-------------------------------------------------------+
    // | Margins                                               |
    // +-------------------------------------------------------+

    int leftMargin;
    int rightMargin;
    int topMargin;
    int bottomMargin;

    // +-------------------------------------------------------+
    // | Dimensions                                            |
    // +-------------------------------------------------------+

    int spacerWidth;
    int gridWidth;
    int gridHeight;
    int labelWidth;
    int labelHeight;

};

#endif // PLOTLATITUDE_H
