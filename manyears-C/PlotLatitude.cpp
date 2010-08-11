#include "plotLatitude.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

PlotLatitude::PlotLatitude(int _width, int _height, int _samplingRate, int _hopSize, SourceManager *_sourceManager)
{

    // Widget
    this->mainGrid = new QGridLayout(this);
    this->mainGrid->setMargin(0);

    // Save general information

    this->width = _width;
    this->height = _height;
    this->samplingRate = _samplingRate;
    this->hopSize = _hopSize;

    // Compute specific dimensions

    this->leftMargin = 20;
    this->rightMargin = 20;
    this->topMargin = 20;
    this->bottomMargin= 20;
    this->spacerWidth = 20;
    this->labelWidth = 40;

    this->labelHeight = this->height - this->topMargin - this->bottomMargin;
    this->gridWidth = this->width - this->leftMargin - this->labelWidth - this->spacerWidth - this->rightMargin;
    this->gridHeight = this->height - this->topMargin - this->bottomMargin;

    // Connect to source manager

    this->currentSourceManager = _sourceManager;

    // Generate a new plot with the given dimensions

    //this->activePlot = new Plot(this->gridWidth, this->labelWidth, this->gridHeight, this->topMargin, this->bottomMargin, this->leftMargin, this->rightMargin, this->spacerWidth, &this->currentSourceManager->blockRendering);
    this->activePlot = new Plot(this->gridWidth, this->labelWidth, this->gridHeight, this->topMargin, this->bottomMargin, this->leftMargin, this->rightMargin, this->spacerWidth);

    // Configure the plot

    this->activePlot->setYMin(-90);
    this->activePlot->setYMax(90);
    this->activePlot->setYInterval(15);

    this->activePlot->setColorBorder(QColor(0x00,0x00,0x00));

    this->activePlot->setSamplesPerSecond( ( (float) this->samplingRate ) / ( (float) this->hopSize ) );

    this->activePlot->setFontFamilyLabel("Tahoma");
    this->activePlot->setFontColorLabel(QColor(0x00,0x00,0x00));
    this->activePlot->setFontItalicLabel(false);
    this->activePlot->setFontBoldLabel(false);
    this->activePlot->setFontSizeLabel(10);
    this->activePlot->setFontWeightLabel(1);

    this->activePlot->connectNumberValues(&this->currentSourceManager->numberSources);
    this->activePlot->connectColorsToDraw(&this->currentSourceManager->connectColors);
    this->activePlot->connectValuesToDraw(&this->currentSourceManager->connectLatitudeAngles);

    // Initialize the graph
    this->activePlot->initPlot();

    // Add plot on the widget
    this->mainGrid->addWidget(this->activePlot,0,0,0,0,(Qt::AlignHCenter | Qt::AlignVCenter));

}

/***********************************************************
* Update graph                                             *
***********************************************************/

void PlotLatitude::updateGraph()
{

    qDebug("Start refreshing latitude plot");

    this->activePlot->updateGraph();

    qDebug("Stop refreshing latitude plot");

}

void PlotLatitude::initPlot()
{
    this->activePlot->initPlot();
}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Time scale                                            |
// +-------------------------------------------------------+

void PlotLatitude::setXMin(float _xMin)
{
    this->activePlot->setXMin(_xMin);
}

void PlotLatitude::setXMax(float _xMax)
{
    this->activePlot->setXMax(_xMax);
}

void PlotLatitude::setXInterval(float _xInterval)
{
    this->activePlot->setXInterval(_xInterval);
}

// +-------------------------------------------------------+
// | Background                                            |
// +-------------------------------------------------------+

void PlotLatitude::setColorBackground(QColor _colorBackground)
{
    this->activePlot->setColorBackground(_colorBackground);
}

// +-------------------------------------------------------+
// | Trace                                                 |
// +-------------------------------------------------------+

void PlotLatitude::setSizePoint(int _sizePoint)
{
    this->activePlot->setSizePoint(_sizePoint);
}

// +-------------------------------------------------------+
// | Axes                                                  |
// +-------------------------------------------------------+

void PlotLatitude::setColorHorizontalBar(QColor _colorHorizontalBar)
{
    this->activePlot->setColorHorizontalBar(_colorHorizontalBar);
}

void PlotLatitude::setColorVerticalBar(QColor _colorVerticalBar)
{
    this->activePlot->setColorVerticalBar(_colorVerticalBar);
}

/***********************************************************
* Update graph slot                                        *
***********************************************************/

void PlotLatitude::updateGraphSlot()
{
    this->updateGraph();
}

