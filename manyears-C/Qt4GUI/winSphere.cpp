#include "winSphere.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

WinSphere::WinSphere(int _width, int _height, int _samplingRate, int _hopSize, SourceManager *_sourceManager, int _updateFactor)
{

    // Set size of the window
    this->width = _width;
    this->height = _height;
    this->frameGeometry().setWidth(this->width);
    this->frameGeometry().setHeight(this->height);

    // Save information
    this->samplingRate = _samplingRate;
    this->hopSize = _hopSize;
    this->mainManager = _sourceManager;
    this->updateFactor = _updateFactor;

    // Initialize the update count
    this->updateCounter = 0;

    // Create plot
    QMutex temp;
    //this->mainPlot = new SpherePlot(3,2,1,0.05,this->mainManager,_width,_height,&this->mainManager->blockRendering);
    this->mainPlot = new SpherePlot(3,2,1,0.05,this->mainManager,_width,_height,&temp);

    this->setWidget(this->mainPlot);

    //---
    this->mainPlot->setBackgroundColor(QColor(0xFF,0xFF,0xFF));
    this->mainPlot->setTopGridColor(QColor(0xA0,0xA0,0xA0));
    this->mainPlot->setBottomGridColor(QColor(0x4C,0x4C,0x4C));
    this->mainPlot->setAxesColor(QColor(0x00,0x00,0x00));
    //---

}

/***********************************************************
* Destructor                                               *
***********************************************************/

WinSphere::~WinSphere()
{

    delete this->mainPlot;

}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Set the background color                              |
// +-------------------------------------------------------+

void WinSphere::setBackgroundColor(QColor _color)
{
    this->mainPlot->setBackgroundColor(_color);
}

// +-------------------------------------------------------+
// | Set the top grid color                                |
// +-------------------------------------------------------+

void WinSphere::setTopGridColor(QColor _color)
{
    this->mainPlot->setTopGridColor(_color);
}

// +-------------------------------------------------------+
// | Set the bottom grid color                             |
// +-------------------------------------------------------+

void WinSphere::setBottomGridColor(QColor _color)
{
    this->mainPlot->setBottomGridColor(_color);
}

/***********************************************************
* Refresh                                                  *
***********************************************************/

void WinSphere::updatePlot()
{

    this->updateCounter++;

    if (this->updateCounter == this->updateFactor)
    {
        this->mainPlot->updateGraph();
        this->updateCounter = 0;
    }   

}

