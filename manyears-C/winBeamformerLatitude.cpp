#include "winBeamformerLatitude.h"

///***********************************************************
//* Constructor                                              *
//***********************************************************/
//
//WinBeamformerLatitude::WinBeamformerLatitude(int _width, int _height, int _samplingRate, int _hopSize, PotentialSourceManager *_sourceManager)
//{
//
//    // Create temporary widget
//    this->tmpWidget = new QWidget(this);
//    this->setWidget(this->tmpWidget);
//
//    // Set size of the window
//    this->width = _width;
//    this->height = _height;
//    this->frameGeometry().setWidth(this->width);
//    this->frameGeometry().setHeight(this->height);
//
//    // Save information
//    this->samplingRate = _samplingRate;
//    this->hopSize = _hopSize;
//    this->mainManager = _sourceManager;
//
//    // Create plot
//    this->mainPlot = new PlotBeamformerLatitude(this->width,this->height,this->samplingRate,this->hopSize,this->mainManager);
//    this->mainPlot->updateGraph();
//
//    // Create grid
//    this->mainGrid = new QGridLayout(this->tmpWidget);
//    this->mainGrid->setMargin(0);
//
//    // Add plot to grid
//    this->mainGrid->addWidget(this->mainPlot);
//
//}
//
///***********************************************************
//* Destructor                                               *
//***********************************************************/
//
//WinBeamformerLatitude::~WinBeamformerLatitude()
//{
//
//    delete this->mainGrid;
//    delete this->mainPlot;
//    delete this->tmpWidget;
//
//}
//
///***********************************************************
//* Refresh                                                  *
//***********************************************************/
//
//void WinBeamformerLatitude::updatePlot()
//{
//
//    this->mainPlot->updateGraph();
//
//}
//
//void WinBeamformerLatitude::initPlot()
//{
//    this->mainPlot->initPlot();
//}
//
///***********************************************************
//* Mutator                                                  *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | Time scale                                            |
//// +-------------------------------------------------------+
//
//void WinBeamformerLatitude::setXMin(float _xMin)
//{
//    this->mainPlot->setXMin(_xMin);
//}
//
//void WinBeamformerLatitude::setXMax(float _xMax)
//{
//    this->mainPlot->setXMax(_xMax);
//}
//
//void WinBeamformerLatitude::setXInterval(float _xInterval)
//{
//    this->mainPlot->setXInterval(_xInterval);
//}
//
//// +-------------------------------------------------------+
//// | Background                                            |
//// +-------------------------------------------------------+
//
//void WinBeamformerLatitude::setColorBackground(QColor _colorBackground)
//{
//    this->mainPlot->setColorBackground(_colorBackground);
//}
//
//// +-------------------------------------------------------+
//// | Trace                                                 |
//// +-------------------------------------------------------+
//
//void WinBeamformerLatitude::setSizePoint(int _sizePoint)
//{
//    this->mainPlot->setSizePoint(_sizePoint);
//}
//
//// +-------------------------------------------------------+
//// | Axes                                                  |
//// +-------------------------------------------------------+
//
//void WinBeamformerLatitude::setColorHorizontalBar(QColor _colorHorizontalBar)
//{
//    this->mainPlot->setColorHorizontalBar(_colorHorizontalBar);
//}
//
//void WinBeamformerLatitude::setColorVerticalBar(QColor _colorVerticalBar)
//{
//    this->mainPlot->setColorVerticalBar(_colorVerticalBar);
//}
