#include "winLongitude.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

WinLongitude::WinLongitude(int _width, int _height, int _samplingRate, int _hopSize, SourceManager *_sourceManager)
{

    // Create temporary widget
    this->tmpWidget = new QWidget(this);
    this->setWidget(this->tmpWidget);

    // Set size of the window
    this->width = _width;
    this->height = _height;
    this->frameGeometry().setWidth(this->width);
    this->frameGeometry().setHeight(this->height);

    // Save information
    this->samplingRate = _samplingRate;
    this->hopSize = _hopSize;
    this->mainManager = _sourceManager;

    // Create plot   
    this->mainPlot = new PlotLongitude(this->width,this->height,this->samplingRate,this->hopSize,this->mainManager);
    this->mainPlot->updateGraph();

    // Create grid
    this->mainGrid = new QGridLayout(this->tmpWidget);
    this->mainGrid->setMargin(0);

    // Add plot to grid
    this->mainGrid->addWidget(this->mainPlot);

}

/***********************************************************
* Destructor                                               *
***********************************************************/

WinLongitude::~WinLongitude()
{

    delete this->mainGrid;
    delete this->mainPlot;
    delete this->tmpWidget;

}

/***********************************************************
* Refresh                                                  *
***********************************************************/

void WinLongitude::updatePlot()
{

    this->mainPlot->updateGraph();

}

void WinLongitude::initPlot()
{
    this->mainPlot->initPlot();
}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Time scale                                            |
// +-------------------------------------------------------+

void WinLongitude::setXMin(float _xMin)
{
    this->mainPlot->setXMin(_xMin);
}

void WinLongitude::setXMax(float _xMax)
{
    this->mainPlot->setXMax(_xMax);
}

void WinLongitude::setXInterval(float _xInterval)
{
    this->mainPlot->setXInterval(_xInterval);
}

// +-------------------------------------------------------+
// | Background                                            |
// +-------------------------------------------------------+

void WinLongitude::setColorBackground(QColor _colorBackground)
{
    this->mainPlot->setColorBackground(_colorBackground);
}

// +-------------------------------------------------------+
// | Trace                                                 |
// +-------------------------------------------------------+

void WinLongitude::setSizePoint(int _sizePoint)
{
    this->mainPlot->setSizePoint(_sizePoint);
}

// +-------------------------------------------------------+
// | Axes                                                  |
// +-------------------------------------------------------+

void WinLongitude::setColorHorizontalBar(QColor _colorHorizontalBar)
{
    this->mainPlot->setColorHorizontalBar(_colorHorizontalBar);
}

void WinLongitude::setColorVerticalBar(QColor _colorVerticalBar)
{
    this->mainPlot->setColorVerticalBar(_colorVerticalBar);
}
