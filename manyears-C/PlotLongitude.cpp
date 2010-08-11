#include "plotLongitude.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

PlotLongitude::PlotLongitude(int _width, int _height, int _samplingRate, int _hopSize, SourceManager *_sourceManager)
{

    // Widget
    this->mainGrid = new QGridLayout(this);
    this->mainGrid->setMargin(0);

    // Save general information

    this->width = _width;
    this->height = _height;
    this->samplingRate = _samplingRate;
    this->hopSize = _hopSize;

    this->setGeometry(0,0,this->width,this->height);

    // Compute specific dimensions

    this->leftMargin = 20;
    this->rightMargin = 20;
    this->topMargin = 20;
    this->bottomMargin = 20;
    this->spacerWidth = 20;
    this->labelWidth = 40;

    this->labelHeight = this->height - this->topMargin - this->bottomMargin;
    this->gridWidth = this->width - this->leftMargin - this->labelWidth - this->spacerWidth - this->rightMargin;
    this->gridHeight = this->height - this->topMargin - this->bottomMargin;

    // Connect to source manager

    this->currentSourceManager = _sourceManager;

    // Generate a new plot with the given dimensions

    //TODO REMOVE MUTEX FROM PLOT
    //this->activePlot = new Plot(this->gridWidth, this->labelWidth, this->gridHeight, this->topMargin, this->bottomMargin, this->leftMargin, this->rightMargin, this->spacerWidth, &this->currentSourceManager->blockRendering);
    this->activePlot = new Plot(this->gridWidth, this->labelWidth, this->gridHeight, this->topMargin, this->bottomMargin, this->leftMargin, this->rightMargin, this->spacerWidth);

    // Configure the plot

    this->activePlot->setYMin(-180);
    this->activePlot->setYMax(180);
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
    this->activePlot->connectValuesToDraw(&this->currentSourceManager->connectLongitudeAngles);

    // Initialize the graph
    this->activePlot->initPlot();

    // Add plot on the widget
    this->mainGrid->addWidget(this->activePlot,0,0,0,0,(Qt::AlignHCenter | Qt::AlignVCenter));

}

/***********************************************************
* Update graph                                             *
***********************************************************/

void PlotLongitude::updateGraph()
{

    qDebug("Start refreshing longitude plot");

    this->activePlot->updateGraph();

    qDebug("Stop refreshing longitude plot");

}

void PlotLongitude::initPlot()
{
    this->activePlot->initPlot();
}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Time scale                                            |
// +-------------------------------------------------------+

void PlotLongitude::setXMin(float _xMin)
{
    this->activePlot->setXMin(_xMin);
}

void PlotLongitude::setXMax(float _xMax)
{
    this->activePlot->setXMax(_xMax);
}

void PlotLongitude::setXInterval(float _xInterval)
{
    this->activePlot->setXInterval(_xInterval);
}

// +-------------------------------------------------------+
// | Background                                            |
// +-------------------------------------------------------+

void PlotLongitude::setColorBackground(QColor _colorBackground)
{
    this->activePlot->setColorBackground(_colorBackground);
}

// +-------------------------------------------------------+
// | Trace                                                 |
// +-------------------------------------------------------+

void PlotLongitude::setSizePoint(int _sizePoint)
{
    this->activePlot->setSizePoint(_sizePoint);
}

// +-------------------------------------------------------+
// | Axes                                                  |
// +-------------------------------------------------------+

void PlotLongitude::setColorHorizontalBar(QColor _colorHorizontalBar)
{
    this->activePlot->setColorHorizontalBar(_colorHorizontalBar);
}

void PlotLongitude::setColorVerticalBar(QColor _colorVerticalBar)
{
    this->activePlot->setColorVerticalBar(_colorVerticalBar);
}

/***********************************************************
* Update graph slot                                        *
***********************************************************/

void PlotLongitude::updateGraphSlot()
{
    this->updateGraph();
}
