#include <math.h>

#include "plot.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

Plot::Plot(int _gridWidth, int _labelWidth, int _height, int _topMargin, int _bottomMargin, int _leftMargin, int _rightMargin, int _spacerWidth)
{

    // Save the dimensions of the grid

    this->gridWidth = _gridWidth;
    this->gridHeight = _height;

    // Save the margins

    this->topMargin = _topMargin;
    this->bottomMargin = _bottomMargin;
    this->leftMargin = _leftMargin;
    this->rightMargin = _rightMargin;

    // Save the spacer

    this->spacerWidth = _spacerWidth;

    // Save the dimensions of the label

    this->labelWidth = _labelWidth;
    this->labelHeight = this->topMargin + _height + this->bottomMargin;

    // Save the dimension of the widget

    this->width = this->leftMargin + this->labelWidth + this->spacerWidth + this->gridWidth + this->rightMargin;
    this->height = this->topMargin + this->gridHeight + this->bottomMargin;
    this->setFixedSize(this->width, this->height);

    // Instantiate the pixmaps

    this->pixelMap = new QPixmap(this->width, this->height);
    this->gridPixelMap = new QPixmap(this->gridWidth, this->gridHeight);
    this->labelPixelMap = new QPixmap(this->labelWidth, this->labelHeight);
    this->hBarPixelMap = new QPixmap(1,this->gridHeight);
    this->vBarPixelMap = new QPixmap(1,this->gridHeight);

    // Save the mutex
    //this->blockRendering = _mutex;

}

Plot::~Plot()
{

    // Destroy the pixmaps
    delete this->gridPixelMap;
    delete this->hBarPixelMap;
    delete this->vBarPixelMap;

}

/***********************************************************
* Connections                                              *
***********************************************************/

// +-------------------------------------------------------+
// | Connect the plot with the data sources                |
// +-------------------------------------------------------+

void Plot::connectNumberValues(int *_numberValues)
{
    this->numberValues = _numberValues;
}

void Plot::connectValuesToDraw(QVector<float> *_valuesToDraw)
{
    this->valuesToDraw = _valuesToDraw;
}

void Plot::connectColorsToDraw(QVector<QColor> *_colorsToDraw)
{
    this->colorsToDraw = _colorsToDraw;
}

/***********************************************************
* Refreshing data                                          *
***********************************************************/

// +-------------------------------------------------------+
// | Initialize the graph                                  |
// +-------------------------------------------------------+

void Plot::initPlot()
{

    this->initDrawing();
    this->drawGrid();
    this->drawLabels();
    this->ratioPoint = (-1 * (float) this->xMin) / ((float) this->xMax - (float) this->xMin);

}

// +-------------------------------------------------------+
// | End of the current frame                              |
// +-------------------------------------------------------+

void Plot::addFrame()
{

    // Increment the pixel accumulator
    this->pixelAccumulator += this->pixelPerSample;

    // Refresh the plot
    this->refreshPlot();

}

// +-------------------------------------------------------+
// | Update the graph                                      |
// +-------------------------------------------------------+

void Plot::updateGraph()
{

    this->addFrame();
    this->refreshPlot();

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Range for the X and Y dimensions                      |
// +-------------------------------------------------------+

// X-Dimension

float Plot::getXMin()
{
    return (this->xMin);
}

float Plot::getXMax()
{
    return (this->xMax);
}

float Plot::getXInterval()
{
    return (this->xInterval);
}


// Y-Dimension

float Plot::getYMin()
{
    return (this->yMin);
}

float Plot::getYMax()
{
    return (this->yMax);
}

float Plot::getYInterval()
{
    return (this->yInterval);
}

// +-------------------------------------------------------+
// | Layout                                                |
// +-------------------------------------------------------+

// Margins

int Plot::getLeftMargin()
{
    return (this->leftMargin);
}

int Plot::getRightMargin()
{
    return (this->rightMargin);
}

int Plot::getTopMargin()
{
    return (this->topMargin);
}

int Plot::getBottomMargin()
{
    return (this->bottomMargin);
}

// Elements

int Plot::getLabelWidth()
{
    return (this->labelWidth);
}

int Plot::getSpacerWidth()
{
    return (this->spacerWidth);
}

int Plot::getGridWidth()
{
    return (this->gridWidth);
}

// Widget

int Plot::getWidth()
{
    return (this->width);
}

int Plot::getHeight()
{
    return (this->height);
}

// +-------------------------------------------------------+
// | Appearance                                            |
// +-------------------------------------------------------+

// Widget

QColor Plot::getColorBackground()
{
    return (this->colorBackground);
}

QColor Plot::getColorBorder()
{
    return (this->colorBorder);
}

// Points

int Plot::getSizePoint()
{
    return (this->sizePoint);
}

float Plot::getRatioPoint()
{
    return (this->ratioPoint);
}

// Grid

QColor Plot::getColorHorizontalBar()
{
    return (this->colorHorizontalBar);
}

QColor Plot::getColorVerticalBar()
{
    return (this->colorVerticalBar);
}

// Label

QColor Plot::getFontColorLabel()
{
    return (this->fontColorLabel);
}

int Plot::getFontSizeLabel()
{
    return (this->fontSizeLabel);
}

int Plot::getFontWeightLabel()
{
    return (this->fontWeightLabel);
}

QString Plot::getFontFamilyLabel()
{
    return (this->fontFamilyLabel);
}

bool Plot::getFontItalicLabel()
{
    return (this->fontItalicLabel);
}

bool Plot::getFontBoldLabel()
{
    return (this->fontBoldLabel);
}

// +-------------------------------------------------------+
// | Scaling                                               |
// +-------------------------------------------------------+

// Sampling rate

float Plot::getSamplesPerSecond()
{
    return (this->samplesPerSecond);
}


/***********************************************************
* Mutators                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Range for the X and Y dimensions                      |
// +-------------------------------------------------------+

// X-Dimension

void Plot::setXMin(float _xMin)
{
    this->xMin = _xMin;
}

void Plot::setXMax(float _xMax)
{
    this->xMax = _xMax;
}

void Plot::setXInterval(float _xInterval)
{
    this->xInterval = _xInterval;
}


// Y-Dimension

void Plot::setYMin(float _yMin)
{
    this->yMin = _yMin;
}

void Plot::setYMax(float _yMax)
{
    this->yMax = _yMax;
}

void Plot::setYInterval(float _yInterval)
{
    this->yInterval = _yInterval;
}

// +-------------------------------------------------------+
// | Appearance                                            |
// +-------------------------------------------------------+

// Widget

void Plot::setColorBackground(QColor _colorBackground)
{
    this->colorBackground = _colorBackground;
}

void Plot::setColorBorder(QColor _colorBorder)
{
    this->colorBorder = _colorBorder;
}

// Points

void Plot::setSizePoint(int _sizePoint)
{
    this->sizePoint = _sizePoint;
}

// Grid

void Plot::setColorHorizontalBar(QColor _colorHorizontalBar)
{
    this->colorHorizontalBar = _colorHorizontalBar;
}

void Plot::setColorVerticalBar(QColor _colorVerticalBar)
{
    this->colorVerticalBar = _colorVerticalBar;
}

// Label

void Plot::setFontColorLabel(QColor _fontColorLabel)
{
    this->fontColorLabel = _fontColorLabel;
}

void Plot::setFontSizeLabel(int _fontSizeLabel)
{
    this->fontSizeLabel = _fontSizeLabel;
}

void Plot::setFontWeightLabel(int _fontWeightLabel)
{
    this->fontWeightLabel = _fontWeightLabel;
}

void Plot::setFontFamilyLabel(QString _fontFamilyLabel)
{
    this->fontFamilyLabel = _fontFamilyLabel;
}

void Plot::setFontItalicLabel(bool _fontItalicLabel)
{
    this->fontItalicLabel = _fontItalicLabel;
}

void Plot::setFontBoldLabel(bool _fontBoldLabel)
{
    this->fontBoldLabel = _fontBoldLabel;
}

// +-------------------------------------------------------+
// | Scaling                                               |
// +-------------------------------------------------------+

// Sampling rate

void Plot::setSamplesPerSecond(float _samplesPerSecond)
{
    this->samplesPerSecond = _samplesPerSecond;  
}


/***********************************************************
* Protected events                                         *
***********************************************************/

// +-------------------------------------------------------+
// | Paint                                                 |
// +-------------------------------------------------------+

void Plot::paintEvent( QPaintEvent * )
{

    // This is a critical section
    // Protect with a mutex
    //this->blockRendering->lock();

    QPainter paint(this);

    paint.drawPixmap(0,0,*(this->pixelMap));

    paint.end();

    // Free
    //this->blockRendering->unlock();

}


/***********************************************************
* Drawing                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Preparing for drawing                                 |
// +-------------------------------------------------------+

void Plot::initDrawing()
{

    float totalTime;
    float totalSamples;

    int numberHBars;
    int yPixel;

    QPainter paintH(this->hBarPixelMap);
    QPainter paintV(this->vBarPixelMap);

    // Compute the time per sample
    this->timePerSample = 1.0 / ((float) this->samplesPerSecond);

    // Compute the number of pixels per sample
    totalTime = this->xMax - this->xMin;
    totalSamples = totalTime / (this->timePerSample);
    this->pixelPerSample = this->gridWidth / totalSamples;

    // Compute the time per pixel
    this->timePerPixel = totalTime / this->gridWidth;

    // Initialize pixel accumulator
    this->pixelAccumulator = 0;

    // Set the background
    paintH.setBackground(this->colorBackground);
    paintV.setBackground(this->colorBackground);

    // Draw the horizontal bars
    numberHBars = (int) ((this->yMax - this->yMin) / (this->yInterval));

    paintH.eraseRect(0,0,1,this->gridHeight);
    paintH.setPen(this->colorHorizontalBar);

    for (int indexBar = 0; indexBar <= numberHBars; indexBar++)
    {

        yPixel = (int) (round((((float) indexBar) / ((float) numberHBars)) * ((float) (this->gridHeight - 1))));

        paintH.drawPoint(0,yPixel);

    }

    paintH.end();

    // Draw the vertical bar
    paintV.eraseRect(0,0,1,this->gridHeight);
    paintV.setPen(this->colorVerticalBar);
    paintV.drawLine(0,0,0,this->gridHeight);
    paintV.end();



}

// +-------------------------------------------------------+
// | Draw the fixed full grid                              |
// +-------------------------------------------------------+

void Plot::drawGrid()
{

    QPainter paint(this->gridPixelMap);

    // Initialize time accumulator to the end of an interval
    this->timeAccumulator = this->xInterval;

    // Loop for each pixel
    for (int indexPixel = 0; indexPixel < this->gridWidth; indexPixel++)
    {

        // Check if the interval has been reached
        if (this->timeAccumulator >= this->xInterval)
        {

            // If so, draw a vertical line
            paint.drawPixmap(indexPixel,0,1,this->gridHeight,*(this->vBarPixelMap));

            // Remove the time interval from the accumulator
            this->timeAccumulator -= this->xInterval;

        }
        else
        {

            // If not, draw horizontal lines
            paint.drawPixmap(indexPixel,0,1,this->gridHeight,*(this->hBarPixelMap));

        }

        // Increment the time
        this->timeAccumulator += this->timePerPixel;

    }

    paint.end();


}

// +-------------------------------------------------------+
// | Draw the labels                                       |
// +-------------------------------------------------------+

void Plot::drawLabels()
{

    QPainter paint(this->labelPixelMap);

    int numberLabels;
    int labelRectWidth;
    int labelRectHeight;
    int yPixel;
    QString labelString;
    QFont labelFont;


    // Draw the labels

    labelRectWidth = this->labelWidth;
    labelRectHeight = (int) (round(((this->yInterval) / (this->yMax - this->yMin)) * (this->labelHeight - this->topMargin - this->bottomMargin)));

    numberLabels = (int) ((this->yMax - this->yMin) / (this->yInterval));

    paint.eraseRect(0,0,this->labelWidth,this->labelHeight);

    labelFont.setBold(this->fontBoldLabel);
    labelFont.setItalic(this->fontItalicLabel);
    labelFont.setFamily(this->fontFamilyLabel);
    labelFont.setWeight(this->fontWeightLabel);
    labelFont.setPointSize(this->fontSizeLabel);

    paint.setPen(this->fontColorLabel);
    paint.setFont(labelFont);

    for (int indexBar = 0; indexBar <= numberLabels; indexBar++)
    {

        yPixel = (int) (round(this->topMargin + (((float) indexBar) / ((float) numberLabels)) * ((float) (this->labelHeight - this->topMargin - this->bottomMargin - 1))));

        labelString.setNum((this->yMax - indexBar * this->yInterval),'g',5);

        paint.drawText(QRect(0,(yPixel - labelRectHeight / 2),labelRectWidth,labelRectHeight),(Qt::AlignVCenter | Qt::AlignRight),labelString);

    }

    paint.end();

}

/***********************************************************
* Refreshing graph                                         *
***********************************************************/

// +-------------------------------------------------------+
// | Refresh the plot                                      |
// +-------------------------------------------------------+

void Plot::refreshPlot()
{

    // This is a critical section
    // Protect with a mutex
    //this->blockRendering->lock();

    qDebug("Start refreshing a plot");

    QPainter paint(this->gridPixelMap);
    QPainter paintAll(this->pixelMap);

    int pixelShift;

    int xPixel;
    int yPixel;

    // Define the number of pixels for the shift
    pixelShift = floor(this->pixelAccumulator);

    // Remove the integer part
    this->pixelAccumulator -= floor(this->pixelAccumulator);

    // Move the graph
    paint.drawPixmap(0,0,*(this->gridPixelMap),pixelShift,0,(this->gridWidth-pixelShift),this->gridHeight);

    // Draw the missing grid
    for (int indexPixel = 0; indexPixel < pixelShift; indexPixel++)
    {
        // Compute the position of the new grid
        xPixel = this->gridWidth - pixelShift + indexPixel;

        // Check if the interval has been reached
        if (this->timeAccumulator >= this->xInterval)
        {

            // If so, draw a vertical line
            paint.drawPixmap(xPixel,0,1,this->gridHeight,*(this->vBarPixelMap));

            // Remove the time interval from the accumulator
            this->timeAccumulator -= this->xInterval;

        }

        else
        {

            // If not, draw horizontal lines
            paint.drawPixmap(xPixel,0,1,this->gridHeight,*(this->hBarPixelMap));

        }

        // Increment the time accumlator
        this->timeAccumulator += this->timePerPixel;

    }

    // Draw the new points
    xPixel = (int) (round(this->ratioPoint * ((float) this->gridWidth)));

    // Active anti-aliasing
    paint.setRenderHint(QPainter::Antialiasing,true);

    qDebug("Plot: start draw points");



    qDebug("Plot: start loop");

    for (int indexPoint = 0; indexPoint < *(this->numberValues); indexPoint++)
    {

        qDebug("Number value %d",*(this->numberValues));
        qDebug("Size colorsToDraw %d",(this->colorsToDraw->size()));
        qDebug("Size valuesToDraw %d",(this->valuesToDraw->size()));

        // Set the color
        paint.setPen(this->colorsToDraw->at(indexPoint));
        paint.setBrush(this->colorsToDraw->at(indexPoint));

        // Calculate the y position
        yPixel = round((1 - ((this->valuesToDraw->at(indexPoint) - this->yMin ) / (this->yMax - this->yMin))) * (this->gridHeight - 1));

        // Draw the point
        paint.drawEllipse(QPoint(xPixel,yPixel),this->sizePoint,this->sizePoint);

    }

    qDebug("Plot: end loop");

    qDebug("Plot: stop draw points");

    paint.end();

    qDebug("Plot: start painting");

    // Paint everything
    paintAll.eraseRect(0,0,this->width,this->height);
    paintAll.drawPixmap(this->leftMargin,0,*(this->labelPixelMap),0,0,this->labelWidth,this->labelHeight);
    paintAll.drawPixmap((this->leftMargin+this->labelWidth+this->spacerWidth),this->topMargin,*(this->gridPixelMap),0,0,this->gridWidth,this->gridHeight);
    paintAll.setPen(this->colorBorder);
    paintAll.drawRect(0,0,this->width-1,this->height-1);

    paintAll.end();

    qDebug("Plot: stop painting");

    // Update the widget appearance
    this->update();

    qDebug("Stop refreshing a plot");

    // Free
    //this->blockRendering->unlock();

}


