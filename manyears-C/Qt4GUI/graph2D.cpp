#include "graph2D.h"
#include <math.h>
#include "hardware.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

Graph2D::Graph2D(const char _typeView, const char _typeSource)
{

    // For now there is no receiver for the event
    this->resizeReceiver = NULL;

    // Initialize the graph
    initGraph(_typeView, _typeSource);

    // Generate new elements
    createLabels();
    createHorizontalAxes();
    createVerticalAxes();
    createEmptyGrid();
    createEmptyAll();

    // Draw all elements
    drawLabels();
    drawGrid();

}

/***********************************************************
* Destructor                                               *
***********************************************************/

Graph2D::~Graph2D()
{

}

/***********************************************************
* Events receivers                                         *
***********************************************************/

// +-------------------------------------------------------+
// | Resize                                                |
// +-------------------------------------------------------+

void Graph2D::setResizeReceiver(QObject* _receiver)
{
    this->resizeReceiver = _receiver;
}


/***********************************************************
* Parameters                                               *
***********************************************************/

// +-------------------------------------------------------+
// | Update parameters                                     |
// +-------------------------------------------------------+

void Graph2D::loadParameters(ConfigurationVector _configurationVector)
{

    if ((this->typeSource == Graph2D::POTENTIAL) && (this->typeView == Graph2D::LATITUDE))
    {

        this->backgroundColor = _configurationVector.getElementColor("POTENTIAL_LATITUDE_BACKGROUND_COLOR");
        this->gridHorizontalBarsColor = _configurationVector.getElementColor("POTENTIAL_LATITUDE_HORIZONTALAXES_COLOR");
        this->gridVerticalBarsColor = _configurationVector.getElementColor("POTENTIAL_LATITUDE_VERTICALAXES_COLOR");
        this->pointSize = _configurationVector.getElementInt("POTENTIAL_LATITUDE_TRACE_SIZE");       

    }
    if ((this->typeSource == Graph2D::POTENTIAL) && (this->typeView == Graph2D::LONGITUDE))
    {

        this->backgroundColor = _configurationVector.getElementColor("POTENTIAL_LONGITUDE_BACKGROUND_COLOR");
        this->gridHorizontalBarsColor = _configurationVector.getElementColor("POTENTIAL_LONGITUDE_HORIZONTALAXES_COLOR");
        this->gridVerticalBarsColor = _configurationVector.getElementColor("POTENTIAL_LONGITUDE_VERTICALAXES_COLOR");
        this->pointSize = _configurationVector.getElementInt("POTENTIAL_LONGITUDE_TRACE_SIZE");

    }
    if ((this->typeSource == Graph2D::TRACKED) && (this->typeView == Graph2D::LATITUDE))
    {

        this->backgroundColor = _configurationVector.getElementColor("TRACKED_LATITUDE_BACKGROUND_COLOR");
        this->gridHorizontalBarsColor = _configurationVector.getElementColor("TRACKED_LATITUDE_HORIZONTALAXES_COLOR");
        this->gridVerticalBarsColor = _configurationVector.getElementColor("TRACKED_LATITUDE_VERTICALAXES_COLOR");
        this->pointSize = _configurationVector.getElementInt("TRACKED_LATITUDE_TRACE_SIZE");

    }
    if ((this->typeSource == Graph2D::TRACKED) && (this->typeView == Graph2D::LONGITUDE))
    {

        this->backgroundColor = _configurationVector.getElementColor("TRACKED_LONGITUDE_BACKGROUND_COLOR");
        this->gridHorizontalBarsColor = _configurationVector.getElementColor("TRACKED_LONGITUDE_HORIZONTALAXES_COLOR");
        this->gridVerticalBarsColor = _configurationVector.getElementColor("TRACKED_LONGITUDE_VERTICALAXES_COLOR");
        this->pointSize = _configurationVector.getElementInt("TRACKED_LONGITUDE_TRACE_SIZE");

    }

    this->xView = _configurationVector.getElementFloat("TIME_BUFFER");
    this->xAfter = _configurationVector.getElementFloat("TIME_AFTER");
    this->xInterval = _configurationVector.getElementFloat("TIME_INTERVAL");

    this->timePerFrame = ((float) ParametersManager::HOPSIZE) / ((float) ParametersManager::SAMPLINGRATE);
    this->timePerPixel = (this->xView + this->xAfter) / ( this->gridWidth );
    this->pixelPerFrame = this->timePerFrame / this->timePerPixel;

}

/***********************************************************
* Size                                                     *
***********************************************************/

// +-------------------------------------------------------+
// | Resize                                                |
// +-------------------------------------------------------+

void Graph2D::resizeWindow(int _width, int _height)
{

    this->gridWidth = _width - this->labelWidth - this->hspacerleft - this->hspacercenter - this->hspacerright;

    // The grid must be at least 1 pixel wide
    if (this->gridWidth < 1)
    {
        this->gridWidth = 1;
    }

    this->gridHeight = _height - this->vspacertop - this->vspacerbottom;

    // The grid must be at least 1 pixel high
    if (this->gridHeight < 1)
    {
        this->gridHeight = 1;
    }

    this->timePerFrame = ((float) ParametersManager::HOPSIZE) / ((float) ParametersManager::SAMPLINGRATE);
    this->timePerPixel = (this->xView + this->xAfter) / ( this->gridWidth );
    this->pixelPerFrame = this->timePerFrame / this->timePerPixel;

}

/***********************************************************
* Events                                                   *
***********************************************************/

// +-------------------------------------------------------+
// | User                                                  |
// +-------------------------------------------------------+

bool Graph2D::event(QEvent* event)
{

    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | One frame of sources is added                 |
        // +-----------------------------------------------+

        UpdateGraphAddSampleEvent *updateGraphAddSampleEvent = dynamic_cast<UpdateGraphAddSampleEvent*>(event);

        if (updateGraphAddSampleEvent != NULL)
        {

            // Shift the grid
            shiftGridByOneFrame();

            // Draw new frame
            drawSources(updateGraphAddSampleEvent->getAllSources());

            // Redraw the modified grid on the complete pixmap
            drawGrid();

            // Display
            this->update();

        }

        // +-----------------------------------------------+
        // | The graph is resized                          |
        // +-----------------------------------------------+

        UpdateGraphResizeEvent *updateGraphResizeEvent = dynamic_cast<UpdateGraphResizeEvent*>(event);

        if (updateGraphResizeEvent != NULL)
        {

            this->gridWidth = updateGraphResizeEvent->getWidth() - this->labelWidth - this->hspacerleft - this->hspacercenter - this->hspacerright;

            // The grid must be at least 1 pixel wide
            if (this->gridWidth < 1)
            {
                this->gridWidth = 1;
            }

            this->gridHeight = updateGraphResizeEvent->getHeight() - this->vspacertop - this->vspacerbottom;

            // The grid must be at least 1 pixel high
            if (this->gridHeight < 1)
            {
                this->gridHeight = 1;
            }

            // Update scaling
            this->timePerFrame = ((float) ParametersManager::HOPSIZE) / ((float) ParametersManager::SAMPLINGRATE);
            this->timePerPixel = (this->xView + this->xAfter) / ( this->gridWidth );
            this->pixelPerFrame = this->timePerFrame / this->timePerPixel;

            // Generate new elements since dimensions have changed
            createLabels();
            createHorizontalAxes();
            createVerticalAxes();
            createEmptyGrid();
            createEmptyAll();

            // Draw all elements
            drawLabels();
            drawAllSources(updateGraphResizeEvent->getSourcesAllFrames());
            drawGrid();

            // Display
            this->update();

        }

        // +-----------------------------------------------+
        // | Update parameters                             |
        // +-----------------------------------------------+

        UpdateParametersEvent *updateParametersEvent = dynamic_cast<UpdateParametersEvent*>(event);

        if (updateParametersEvent != NULL)
        {

            loadParameters(updateParametersEvent->getConfigurationVector());

        }

        // +-----------------------------------------------+
        // | The graph needs to be refreshed               |
        // +-----------------------------------------------+

        UpdateGraphRefreshEvent *updateGraphRefreshEvent = dynamic_cast<UpdateGraphRefreshEvent*>(event);

        if (updateGraphRefreshEvent != NULL)
        {

            // Generate new elements since parameters have changed
            createLabels();
            createHorizontalAxes();
            createVerticalAxes();
            createEmptyGrid();
            createEmptyAll();

            // Draw all elements
            drawLabels();
            drawGrid();

            // Draw all elements
            drawLabels();
            drawAllSources(updateGraphRefreshEvent->getSourcesAllFrames());
            drawGrid();

            // Display
            this->update();

        }

        return true;

    }

    return QWidget::event(event);

}

// +-------------------------------------------------------+
// | Resize                                                |
// +-------------------------------------------------------+

void Graph2D::resizeEvent(QResizeEvent *resizeEvent)
{

    int width, height;

    width = resizeEvent->size().width();
    height = resizeEvent->size().height();

    // Create event
    GraphResizeEvent* tmp = new GraphResizeEvent(width, height, this, this->typeSource);

    // Post event
    if (this->resizeReceiver != NULL)
    {
        QCoreApplication::postEvent(this->resizeReceiver, tmp);
    }

}

// +-------------------------------------------------------+
// | Paint                                                 |
// +-------------------------------------------------------+

void Graph2D::paintEvent( QPaintEvent * )
{

    QPainter paint(this);

    paint.drawPixmap(0, 0, this->allPixelMap->width(), this->allPixelMap->height(), *(this->allPixelMap) );

    paint.end();

}


/***********************************************************
* Initialization                                           *
***********************************************************/

void Graph2D::initGraph(const char _typeView, const char _typeSource)
{

    // +-------------------------------------------------------+
    // | Type                                                  |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | View (latitude/longitude)                         |
        // +---------------------------------------------------+

        this->typeView = _typeView;

        // +---------------------------------------------------+
        // | Type of source (potential/tracked)                |
        // +---------------------------------------------------+

        this->typeSource = _typeSource;

    // +-------------------------------------------------------+
    // | Source buffer                                         |
    // +-------------------------------------------------------+

        //this->potentialSourceBuffer = _potentialSourceManager;

    // +-------------------------------------------------------+
    // | Graph shifter                                         |
    // +-------------------------------------------------------+

        // Pixel accumulator
        this->pixelAccumulator = 0.0;

        // Interval accumulator
        this->intervalAccumulator = 0.0;

    // +-------------------------------------------------------+
    // | Dimensions                                            |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Vertical spacers                                  |
        // +---------------------------------------------------+

        this->vspacertop = 20.0;
        this->vspacerbottom = 20.0;

        // +---------------------------------------------------+
        // | Horizontal spacer                                 |
        // +---------------------------------------------------+

        this->hspacerleft = 20.0;
        this->hspacercenter = 10.0;
        this->hspacerright = 20.0;

        // +---------------------------------------------------+
        // | Label                                             |
        // +---------------------------------------------------+

        this->labelWidth = 50.0;
        this->labelHeight = 20.0;

        // +---------------------------------------------------+
        // | Grid                                              |
        // +---------------------------------------------------+

        this->gridWidth = 0.0;
        this->gridHeight = 0.0;

        // +---------------------------------------------------+
        // | Points                                            |
        // +---------------------------------------------------+

        this->pointSize = 1.0;

    // +-------------------------------------------------------+
    // | Range                                                 |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Horizontal range                                  |
        // +---------------------------------------------------+

        this->xView = 10.0;
        this->xAfter = 1.0;
        this->xInterval = 1.0;

        // +---------------------------------------------------+
        // | Vertical range                                    |
        // +---------------------------------------------------+

        if (this->typeView == this->LATITUDE)
        {
            this->yMin = -90.0;
            this->yMax = 90.0;
            this->yInterval = 15.0;
        }
        if (this->typeView == this->LONGITUDE)
        {
            this->yMin = -180.0;
            this->yMax = 180.0;
            this->yInterval = 15.0;
        }


    // +-------------------------------------------------------+
    // | Colors                                                |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | All                                               |
        // +---------------------------------------------------+

        this->backgroundColor = QColor(0xFF,0xFF,0xFF);

        // +---------------------------------------------------+
        // | Grid                                              |
        // +---------------------------------------------------+

        this->gridHorizontalBarsColor = QColor(0xBB,0xBB,0xBB);
        this->gridVerticalBarsColor = QColor(0xBB,0xBB,0xBB);

        // +---------------------------------------------------+
        // | Color                                             |
        // +---------------------------------------------------+

        this->labelColor = QColor(0x00,0x00,0x00);


    // +-------------------------------------------------------+
    // | Text                                                  |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Label                                             |
        // +---------------------------------------------------+

        this->labelTextFont = QFont("Tahoma",10,QFont::Normal,false);

    // +-------------------------------------------------------+
    // | Graphics                                              |
    // +-------------------------------------------------------+

        this->labelPixelMap = NULL;
        this->hBarPixelMap = NULL;
        this->vBarPixelMap = NULL;
        this->gridPixelMap = NULL;
        this->allPixelMap = NULL;

        // Call a resize in order to create a default graph
        // in case the window is not currently shown and
        // not resize event is called
        resizeWindow(1,1);

}


/***********************************************************
* Drawing                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Create the labels                                     |
// +-------------------------------------------------------+

void Graph2D::createLabels()
{

    // Position of the labels
    int left;
    int top;
    int width;
    int height;

    // Pixmap dimensions
    int pixmapWidth = this->labelWidth;
    int pixmapHeight = (this->vspacertop + this->gridHeight + this->vspacerbottom);

    // Angle values
    float angleValue;
    QString angleString;

    // Get the number of labels
    int numberLabels = round((this->yMax - this->yMin) / (this->yInterval) + 1);

    // Delete existing pixmap
    if (this->labelPixelMap != NULL)
    {
        delete this->labelPixelMap;
    }

    // Create the pixmap
    this->labelPixelMap = new QPixmap(pixmapWidth, pixmapHeight);

    // Create painter
    QPainter paint(this->labelPixelMap);

    // Set fixed dimensions
    left = 0;
    width = this->labelWidth;
    height = this->labelHeight;

    // Erase
    paint.eraseRect(QRect(0,0,pixmapWidth,pixmapHeight));

    // Set font
    paint.setFont(this->labelTextFont);

    // Set color
    paint.setPen(this->labelColor);

    // Draw labels
    for (int indexLabel = 0; indexLabel < numberLabels; indexLabel++)
    {

        // Compute the angle
        angleValue = this->yMax - (((float) indexLabel) / ((float) (numberLabels-1))) * (this->yMax - this->yMin);
        angleString.setNum(angleValue, 'g', 3);

        // Compute top position
        top = (int) ( round( ( ((float) indexLabel) / ((float) (numberLabels - 1)) ) * ( (float) (this->gridHeight - 1) ) + (float) this->vspacertop - ( ( (float) this->labelHeight ) / 2.0 ) ) );

        // Draw
        paint.drawText(QRect(left,top,width,height), (Qt::AlignVCenter | Qt::AlignRight), angleString);

    }

    // Stop painting
    paint.end();

}

// +-------------------------------------------------------+
// | Create the horizontal axes                            |
// +-------------------------------------------------------+

void Graph2D::createHorizontalAxes()
{

    // Position of the points
    int left;
    int top;

    // Pixmap dimensions
    int pixmapWidth = 1;
    int pixmapHeight = this->gridHeight;

    // Get the number of lines
    int numberLines = round((this->yMax - this->yMin) / (this->yInterval) + 1);

    // Delete existing pixmap
    if (this->hBarPixelMap != NULL)
    {
        delete this->hBarPixelMap;
    }

    // Create the pixmap
    this->hBarPixelMap = new QPixmap(pixmapWidth, pixmapHeight);

    // Create painter
    QPainter paint(this->hBarPixelMap);

    // Set fixed dimensions
    left = 0;

    // Erase
    paint.setBackground(this->backgroundColor);
    paint.eraseRect(QRect(0,0,pixmapWidth,pixmapHeight));

    // Set color
    paint.setPen(this->gridHorizontalBarsColor);

    // Add axes
    for (int indexLine = 0; indexLine < numberLines; indexLine++)
    {

        // Left
        top = (int) ( round( ( ((float) indexLine) / ((float) (numberLines - 1) ) ) * ( (float) (this->gridHeight - 1) ) ) );

        // Draw
        paint.drawPoint(left,top);

    }

    // Stop painting
    paint.end();

}

// +-------------------------------------------------------+
// | Create the vertical axes                              |
// +-------------------------------------------------------+

void Graph2D::createVerticalAxes()
{

    // Pixmap dimensions
    int pixmapWidth = 1;
    int pixmapHeight = this->gridHeight;

    // Delete existing pixmap
    if (this->vBarPixelMap != NULL)
    {
        delete this->vBarPixelMap;
    }

    // Create the pixmap
    this->vBarPixelMap = new QPixmap(pixmapWidth, pixmapHeight);

    // Create painter
    QPainter paint(this->vBarPixelMap);

    // Erase
    paint.setBackground(this->backgroundColor);
    paint.eraseRect(QRect(0,0,pixmapWidth,pixmapHeight));

    // Set color
    paint.setPen(this->gridVerticalBarsColor);

    // Draw
    paint.drawRect(0,0,pixmapWidth,pixmapHeight);

    // Stop painting
    paint.end();

}

// +-------------------------------------------------------+
// | Create an empty grid                                  |
// +-------------------------------------------------------+

void Graph2D::createEmptyGrid()
{

    // Tolerance to compensate for the floating point error
    float tolerance = 1E-3;

    // Pixmap dimensions
    int pixmapWidth = this->gridWidth;
    int pixmapHeight = this->gridHeight;

    // Delete existing pixmap
    if (this->gridPixelMap != NULL)
    {
        delete this->gridPixelMap;
    }

    // Create the pixmap
    this->gridPixelMap = new QPixmap(pixmapWidth, pixmapHeight);

    int pixelShift;
    int left;

    float tmpIntervalAccumulator;

    QPainter paint(this->gridPixelMap);

    // +-------------------------------------------------------+
    // | Compute the shift                                     |
    // +-------------------------------------------------------+

    pixelShift = this->gridWidth;

    // +-------------------------------------------------------+
    // | Erase the pixmap                                      |
    // +-------------------------------------------------------+

    paint.setBackground(this->backgroundColor);
    paint.eraseRect(0, 0, this->gridWidth, this->gridHeight);

    // Stop painting
    paint.end();

    // +-------------------------------------------------------+
    // | Complete the grid                                     |
    // +-------------------------------------------------------+

    tmpIntervalAccumulator = this->intervalAccumulator;

    for (int indexPixel = 0; indexPixel < pixelShift; indexPixel++)
    {

        left = this->gridWidth - pixelShift + indexPixel;

        tmpIntervalAccumulator += this->timePerPixel;

        if ((tmpIntervalAccumulator + tolerance) >= this->xInterval)
        {
            insertVerticalAxes(left);
            tmpIntervalAccumulator -= this->xInterval;
        }
        else
        {
            insertHorizontalAxes(left);
        }

    }

}

// +-------------------------------------------------------+
// | Create an empty widget                                |
// +-------------------------------------------------------+

void Graph2D::createEmptyAll()
{

    // Pixmap dimensions
    int pixmapWidth = (this->hspacerleft + this->labelWidth + this->hspacercenter + this->gridWidth + this->hspacerright);
    int pixmapHeight = (this->vspacertop + this->gridHeight + this->vspacerbottom);

    // Delete existing pixmap
    if (this->allPixelMap != NULL)
    {
        delete this->allPixelMap;
    }

    // Create the pixmap
    this->allPixelMap = new QPixmap(pixmapWidth, pixmapHeight);

    // Erase
    QPainter paint(this->allPixelMap);
    paint.eraseRect(QRect(0,0,pixmapWidth,pixmapHeight));
    paint.end();

}


// +-------------------------------------------------------+
// | Draw the labels                                       |
// +-------------------------------------------------------+

void Graph2D::drawLabels()
{

    // Pixmap dimension and position
    int pixmapWidth = this->labelWidth;
    int pixmapHeight = (this->vspacertop + this->gridHeight + this->vspacerbottom);
    int pixmapLeft = this->hspacerleft;
    int pixmapTop = 0;

    // Create painter
    QPainter paint(this->allPixelMap);

    // Copy pixmap from label
    paint.drawPixmap(pixmapLeft, pixmapTop, pixmapWidth, pixmapHeight, *(this->labelPixelMap));

    // Stop painting
    paint.end();

}

// +-------------------------------------------------------+
// | Draw the horizontal axes                              |
// +-------------------------------------------------------+

void Graph2D::insertHorizontalAxes(const int _left)
{

    // Create painter
    QPainter paint(this->gridPixelMap);

    // Copy pixmap from label
    paint.drawPixmap(_left, 0, this->hBarPixelMap->width(), this->hBarPixelMap->height(), *(this->hBarPixelMap));

    // Stop painting
    paint.end();

}

// +-------------------------------------------------------+
// | Draw the vertical axes                                |
// +-------------------------------------------------------+

void Graph2D::insertVerticalAxes(const int _left)
{

    // Create painter
    QPainter paint(this->gridPixelMap);

    // Copy pixmap from label
    paint.drawPixmap(_left, 0, this->vBarPixelMap->width(), this->vBarPixelMap->height(), *(this->vBarPixelMap));

    // Stop painting
    paint.end();

}

// +-------------------------------------------------------+
// | Shift the grid                                        |
// +-------------------------------------------------------+

void Graph2D::shiftGridByOneFrame()
{

    int pixelShift;
    int left;

    QPainter paint(this->gridPixelMap);

    // +-------------------------------------------------------+
    // | Compute the shift                                     |
    // +-------------------------------------------------------+

    this->pixelAccumulator += this->pixelPerFrame;
    pixelShift = floor(this->pixelAccumulator);
    this->pixelAccumulator -= (float) pixelShift;

    // +-------------------------------------------------------+
    // | Shift the pixmap                                      |
    // +-------------------------------------------------------+

    paint.drawPixmap(0, 0, *(this->gridPixelMap), pixelShift, 0, (this->gridWidth - pixelShift), this->gridHeight);

    // Stop painting
    paint.end();

    // +-------------------------------------------------------+
    // | Complete the grid                                     |
    // +-------------------------------------------------------+

    for (int indexPixel = 0; indexPixel < pixelShift; indexPixel++)
    {

        left = this->gridWidth - pixelShift + indexPixel;

        this->intervalAccumulator += this->timePerPixel;

        if (this->intervalAccumulator >= this->xInterval)
        {
            insertVerticalAxes(left);
            this->intervalAccumulator -= this->xInterval;
        }
        else
        {
            insertHorizontalAxes(left);
        }

    }



}

// +-------------------------------------------------------+
// | Draw the full grid                                    |
// +-------------------------------------------------------+

void Graph2D::drawGrid()
{

    // Pixmap dimension and position
    int pixmapWidth = this->gridWidth;
    int pixmapHeight = this->gridHeight;
    int pixmapLeft = this->hspacerleft + this->labelWidth + this->hspacercenter;
    int pixmapTop = this->vspacertop;

    // Create painter
    QPainter paint(this->allPixelMap);

    // Copy pixmap from grid
    paint.drawPixmap(pixmapLeft, pixmapTop, pixmapWidth, pixmapHeight, *(this->gridPixelMap));;

    // Stop painting
    paint.end();

}

// +-------------------------------------------------------+
// | Draw the potential sources                            |
// +-------------------------------------------------------+

void Graph2D::drawSources(QList<SourceDisplayed> _sources)
{

    QPainter paint(this->gridPixelMap);

    int pixelView = round( this->xView / this->timePerPixel );

    float sourceX;
    float sourceY;
    float sourceZ;
    QColor sourceColor;
    float angle;
    int left;
    int top;

    // Loop for each potential source

    paint.setRenderHint(QPainter::Antialiasing,true);

    left = pixelView;

    for (int indexSource = 0; indexSource < _sources.size(); indexSource++)
    {

        // Get the color
        sourceColor = _sources[indexSource].getColor();

        // Compute the angle
        if (this->typeView == Graph2D::LONGITUDE)
        {
            sourceX = _sources[indexSource].getX();
            sourceY = _sources[indexSource].getY();
            angle = (atan2(sourceY, sourceX) / (2*M_PI)) * 360;
        }
        else
        {
            sourceZ = _sources[indexSource].getZ();
            angle = (asin(sourceZ) / (2*M_PI)) * 360;
        }

        // Compute top
        top = round( (1 - ( (angle - this->yMin) / (this->yMax - this->yMin) ) ) * ( this->gridHeight - 1 ) );

        // Draw
        paint.setPen(sourceColor);
        paint.setBrush(sourceColor);
        paint.drawEllipse(QPoint(left,top),this->pointSize,this->pointSize);

    }

    // Stop painting
    paint.end();

}

// +-------------------------------------------------------+
// | Draw all the potential sources                        |
// +-------------------------------------------------------+

void Graph2D::drawAllSources(QList< QList<SourceDisplayed> > _sources)
{

    QPainter paint(this->gridPixelMap);

    float sourceX;
    float sourceY;
    float sourceZ;
    QColor sourceColor;
    float angle;

    int left;
    int top;


    // Loop for each potential source

    paint.setRenderHint(QPainter::Antialiasing,true);

    QList< QList<SourceDisplayed> >::iterator frameIterator;

    frameIterator = _sources.begin();

    for (int indexFrame = 0; indexFrame < _sources.size(); indexFrame++)
    {

        for (int indexSource = 0; indexSource < frameIterator->size(); indexSource++)
        {

            // Get the color
            sourceColor = frameIterator->at(indexSource).getColor();

            // Compute the angle
            if (this->typeView == Graph2D::LONGITUDE)
            {
                sourceX = frameIterator->at(indexSource).getX();
                sourceY = frameIterator->at(indexSource).getY();
                angle = (atan2(sourceY, sourceX) / (2*M_PI)) * 360;
            }
            else
            {
                sourceZ = frameIterator->at(indexSource).getZ();
                angle = (asin(sourceZ) / (2*M_PI)) * 360;
            }

            // Compute top
            top = round( (1 - ( (angle - this->yMin) / (this->yMax - this->yMin) ) ) * ( this->gridHeight - 1 ) );

            // Compute left
            left = (int) (round(this->pixelPerFrame * indexFrame));

            // Draw
            paint.setPen(sourceColor);
            paint.setBrush(sourceColor);
            paint.drawEllipse(QPoint(left,top),this->pointSize,this->pointSize);

        }

        frameIterator++;

    }

    // Stop painting
    paint.end();

}
