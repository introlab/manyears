#include "propertyEditor.h"
#include <limits.h>
#include <QDebug>
#include <float.h>

#define MAX_FLOAT FLT_MAX 
#define MIN_FLOAT FLT_MIN


/***********************************************************
* Constructor                                              *
***********************************************************/

PropertyEditor::PropertyEditor(const int _width, const int _height, const int _scrollBarWidth, const Property _property, const int _typeProperty)
{

    // Save parameters
    this->width = _width;
    this->listWidth = _width - _scrollBarWidth;
    this->listMaxHeight = 1;
    this->listCurrentHeight = 1;
    this->scrollBarWidth = _scrollBarWidth;
    this->height = _height;

    // Create widgets
    this->scrollBar = new QScrollBar(Qt::Vertical, this);
    this->pixelMap = new QPixmap(this->listWidth, this->listMaxHeight);

    // Widgets dimensions
    this->setGeometry(0, 0, this->width, this->height);
    this->scrollBar->setGeometry(this->listWidth, 0, this->scrollBarWidth, this->height);

    // Elements dimensions
    this->iconSize = 16;
    this->squareSize = 10;
    this->indentationSize = 20;
    this->heightElement = 25;
    this->iconLeft = 5;
    this->iconRight = 5;
    this->widthLabel = 0.7* this->listWidth;

    // Properties
    this->rootProperty = new Property(_property);
    this->selectedPropertyKey = "";
    this->scrollBarValue = 0;
    this->listMaxHeight = (this->rootProperty->getNumberChildrenRecursiveAll() - 1) * this->heightElement;
    this->typeProperty = _typeProperty;
    this->eventReceiver = NULL;

    // Connect signals to slot for the scrollbar
    QObject::connect(this->scrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarValueChanged(int)));




}

/***********************************************************
* Destructor                                               *
***********************************************************/

PropertyEditor::~PropertyEditor()
{

}

/***********************************************************
* Load values in properties                                *
***********************************************************/

void PropertyEditor::loadValues(const ConfigurationVector _configurationVector)
{

    QString key;

    for (int indexValue = 0; indexValue < _configurationVector.getNumberElements(); indexValue++)
    {
        key = _configurationVector.getKey(indexValue);
        this->rootProperty->getProperty(key)->setValue(_configurationVector.getValueNoType(key));
    }

    generatePixmaps();

    drawAll();

    update();

}

/***********************************************************
* Export values of properties                              *
***********************************************************/

ConfigurationVector PropertyEditor::exportValues()
{

    ConfigurationVector tmp;

    tmp.loadFromProperties(this->rootProperty);

    return tmp;

}

/***********************************************************
* Events receiver                                          *
***********************************************************/

void PropertyEditor::setEventReceiver(QObject* _eventReceiver)
{
    this->eventReceiver = _eventReceiver;
}


/***********************************************************
* Events                                                   *
***********************************************************/

// +-------------------------------------------------------+
// | Resize occurs                                         |
// +-------------------------------------------------------+

void PropertyEditor::resizeEvent(QResizeEvent *event)
{

    // Refresh dimensions
    this->width = event->size().width();
    this->height = event->size().height();
    this->listWidth = this->width - this->scrollBarWidth;
    this->widthLabel = 0.8* this->listWidth;

    // Make sure the width of list stays positive
    if (this->listWidth <= 0)
    {
        this->listWidth = 1;
    }

    // Check if the vertical bar needs to be displayed
    modifyScrollBarRangeAndSize();

    // Resize pixmaps
    generatePixmaps();

    // Draw
    drawAll();

    // Call paint event
    this->update();

}

// +-------------------------------------------------------+
// | Single click occurs                                   |
// +-------------------------------------------------------+

void PropertyEditor::mousePressEvent(QMouseEvent *event)
{

    Property* selectedProperty;

    // Make sure the mouse pointer is within the range
    selectedProperty = confirmValidClick(event->pos().x(), event->pos().y());

    // If the click is valid
    if (selectedProperty != NULL)
    {

        // If it has children, then expand/shrink
        if (selectedProperty->isALeaf() == false)
        {
            selectedProperty->setExpanded(!(selectedProperty->getExpanded()));
            this->selectedPropertyKey = "";
        }
        // If this is a leaf, then just select
        else
        {
            this->selectedPropertyKey = selectedProperty->getKey();
        }

    }

    // Refresh the size of the current list since some
    // items may have been expanded or shrinked
    // (Number of elements displayed times the height of
    // each element)
    // One is subtracted since the root property is not displayed
    // but still counted
    this->rootProperty->setExpanded(true);
    this->listCurrentHeight = (this->rootProperty->getNumberChildrenRecursiveExpanded() - 1) * this->heightElement;   

    // Check if the vertical bar needs to be displayed
    modifyScrollBarRangeAndSize();

    // Draw
    drawAll();

    // Call the paint event
    this->update();

}


// +-------------------------------------------------------+
// | General events                                        |
// +-------------------------------------------------------+
bool PropertyEditor::event ( QEvent * event )
{
    //Take care of the tooltips
    if (event->type() == QEvent::ToolTip)
    { 
        //Global coordinate of the cursor
        //Transform to widget coordinate
        QPoint pos = QWidget::mapFromGlobal(QCursor::pos());
        Property* selectedProperty = confirmValidClick(pos.x(), pos.y());

        if (selectedProperty)
        {
            // If it has children, no  tooltip
            if (selectedProperty->isALeaf() == false)
            {
                this->setToolTip("");
            }
            else
            {
                this->setToolTip(selectedProperty->getDescription());
            }
        }
    }


    return QWidget::event(event);
}


// +-------------------------------------------------------+
// | Double click occurs                                   |
// +-------------------------------------------------------+

void PropertyEditor::mouseDoubleClickEvent(QMouseEvent *event)
{

    Property* selectedProperty;
    QString valueChanged = "";

    // Make sure the mouse pointer is within the range
    selectedProperty = confirmValidClick(event->pos().x(), event->pos().y());

    // If the click is valid
    if (selectedProperty != NULL)
    {

        // If this is a leaf, then select and pop input dialog
        if (selectedProperty->isALeaf())
        {

            this->selectedPropertyKey = selectedProperty->getKey();
            valueChanged = displayInputBox(selectedProperty->getKey());

            if (valueChanged != "")
            {
                selectedProperty->setValue(valueChanged);
            }

        }

    }

    // Draw
    drawAll();

    // Call the paint event
    this->update();

    // Finally send an event to tell the application
    // a property has changed
    if (valueChanged != "")
    {
        ConfigurationVector tmp;

        tmp.loadFromProperties(this->rootProperty);

        if (this->eventReceiver != NULL)
        {

             if (this->typeProperty == PropertyEditor::VIEW)
            {
                QCoreApplication::postEvent(this->eventReceiver, new UpdateParametersEvent(tmp, UpdateParametersEvent::VIEW));
            }
            else
            {
                QCoreApplication::postEvent(this->eventReceiver, new UpdateParametersEvent(tmp, UpdateParametersEvent::SYSTEM));
            }

        }


    }

}

// +-------------------------------------------------------+
// | Paint                                                 |
// +-------------------------------------------------------+

void PropertyEditor::paintEvent( QPaintEvent * )
{

    // Paint the new portion of the pixmap
    QPainter paint(this);
    //paint.setRenderHint(QPainter::SmoothPixmapTransform);
    //paint.setRenderHint(QPainter::Antialiasing);
    //paint.setRenderHint(QPainter::TextAntialiasing);

    paint.drawPixmap(0,0, *(this->pixelMap), 0, this->scrollBarValue, this->listWidth, this->height);

    // Draw the border
    paint.setPen(QColor(0x44,0x44,0x44));
    paint.drawRect(0, 0, this->listWidth-1, this->height-1);

    paint.end();

}

/***********************************************************
* Slots                                                    *
***********************************************************/

void PropertyEditor::scrollBarValueChanged(int _newValue)
{

    this->scrollBarValue = _newValue;

    this->update();

}

/***********************************************************
* Dimensions adjustements                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Generate pixmaps with good dimensions                 |
// +-------------------------------------------------------+

void PropertyEditor::generatePixmaps()
{

    delete this->pixelMap;
    this->pixelMap = new QPixmap(this->listWidth, this->listMaxHeight);

}

// +-------------------------------------------------------+
// | Modify scrollbar range                                |
// +-------------------------------------------------------+

void PropertyEditor::modifyScrollBarRangeAndSize()
{

    int range = this->listCurrentHeight - this->height;

    this->scrollBar->setGeometry(this->listWidth, 0, this->scrollBarWidth, this->height);
    this->scrollBar->setMinimum(0);

    qDebug() << "Range: " << range;

    if (range < 0)
    {
        this->scrollBar->setMaximum(0);
    }
    else
    {
        this->scrollBar->setMaximum(range);
    }

}

// +-------------------------------------------------------+
// | Confirm valid click                                   |
// +-------------------------------------------------------+

Property* PropertyEditor::confirmValidClick(const int _x, const int _y)
{

    Property* tmp;

    // Make sure the mouse pointer is within the range
    if ((_x >= 0) && (_x < this->listWidth))
    {
        if ((_y >= 0) && (_y < this->height))
        {

            // Make sure there is a loaded set of properties
            if (this->rootProperty != NULL)
            {

                tmp = getProperty(this->rootProperty, _y + this->scrollBarValue);

                return tmp;

            }

        }

    }

    return NULL;

}


/***********************************************************
* Draw                                                     *
***********************************************************/

// +-------------------------------------------------------+
// | + Sign                                                |
// +-------------------------------------------------------+

void PropertyEditor::drawPlus(QColor _color, int _left, int _top)
{

    QPainter paint(this->pixelMap);
    //paint.setRenderHint(QPainter::SmoothPixmapTransform);
    //paint.setRenderHint(QPainter::Antialiasing);
    //paint.setRenderHint(QPainter::TextAntialiasing);

    int halfSquareSize;
    int firstBorder;
    int secondBorder;

    halfSquareSize = this->squareSize / 2;
    firstBorder = 2;
    secondBorder = this->squareSize - 2;

    paint.setPen(_color);
    paint.drawRect(_left, _top, this->squareSize, this->squareSize);
    paint.drawLine(_left + halfSquareSize, _top + firstBorder, _left + halfSquareSize, _top + secondBorder);
    paint.drawLine(_left + firstBorder, _top + halfSquareSize, _left + secondBorder, _top + halfSquareSize);

    paint.end();

}

// +-------------------------------------------------------+
// | - Sign                                                |
// +-------------------------------------------------------+

void PropertyEditor::drawMinus(QColor _color, int _left, int _top)
{

    QPainter paint(this->pixelMap);
    //paint.setRenderHint(QPainter::SmoothPixmapTransform);
    //paint.setRenderHint(QPainter::Antialiasing);
    //paint.setRenderHint(QPainter::TextAntialiasing);

    int halfSquareSize;
    int firstBorder;
    int secondBorder;

    halfSquareSize = this->squareSize / 2;
    firstBorder = 2;
    secondBorder = this->squareSize - 2;

    paint.setPen(_color);
    paint.drawRect(_left, _top, this->squareSize, this->squareSize);
    paint.drawLine(_left + firstBorder, _top + halfSquareSize, _left + secondBorder, _top + halfSquareSize);

    paint.end();

}

// +-------------------------------------------------------+
// | Separator light                                       |
// +-------------------------------------------------------+

void PropertyEditor::drawSeparatorLight(int _top)
{
    QPainter paint(this->pixelMap);
    //paint.setRenderHint(QPainter::SmoothPixmapTransform);
    //paint.setRenderHint(QPainter::Antialiasing);
    //paint.setRenderHint(QPainter::TextAntialiasing);

    paint.setPen(QColor(0xAA,0xAA,0xAA));
    paint.drawLine(0, _top, this->listWidth, _top);

    paint.end();

}

// +-------------------------------------------------------+
// | Separator dark                                        |
// +-------------------------------------------------------+

void PropertyEditor::drawSeparatorDark(int _top)
{
    QPainter paint(this->pixelMap);
    //paint.setRenderHint(QPainter::SmoothPixmapTransform);
    //paint.setRenderHint(QPainter::Antialiasing);
    //paint.setRenderHint(QPainter::TextAntialiasing);

    paint.setPen(QColor(0x44,0x44,0x44));
    paint.drawLine(0, _top, this->listWidth, _top);

    paint.end();

}

// +-------------------------------------------------------+
// | Property                                              |
// +-------------------------------------------------------+

void PropertyEditor::drawProperty(Property *_property, int _prevIndentation)
{

    int leftSquare;
    int topSquare;
    int topIcon;
    bool isSelected;

    QPainter paint(this->pixelMap);
    //paint.setRenderHint(QPainter::SmoothPixmapTransform);
    //paint.setRenderHint(QPainter::Antialiasing);
    //paint.setRenderHint(QPainter::TextAntialiasing);



    QFont fontWithChild("Tahoma",10,QFont::Bold,false);
    QFont fontNoChild("Tahoma",10,QFont::Normal,false);
    QFont fontIcon("Tahoma",10,QFont::Bold,false);
    QFont fontValue("Tahoma",10,QFont::Normal,false);

    // Check if there are children
    if (_property->isALeaf() == false)
    {

        // Draw background
        paint.fillRect(0, this->topElement, this->listWidth, this->heightElement, QColor(0xFF,0xFF,0xFF));

        // Draw text
        paint.setPen(Qt::black);
        paint.setFont(fontWithChild);
        paint.drawText(_prevIndentation + this->indentationSize, this->topElement, this->listWidth - _prevIndentation - this->indentationSize, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getLabel());
        paint.end();

        leftSquare = (this->indentationSize - this->squareSize) / 2;
        topSquare = (this->heightElement - this->squareSize) / 2;

        // Draw plus/minus
        if (_property->getExpanded() == true)
        {
            this->drawMinus(Qt::black, _prevIndentation + leftSquare, this->topElement + topSquare);
        }
        else
        {
            this->drawPlus(Qt::black, _prevIndentation + leftSquare, this->topElement + topSquare);
        }

        // Draw horizontal separator
        drawSeparatorLight(this->topElement + this->heightElement - 1);

        // If it is expanded, then draw children

        if (_property->getExpanded() == true)
        {

            for (int indexChild = 0; indexChild < _property->getNumberChildren(); indexChild++)
            {

                this->topElement += this->heightElement;
                drawProperty(_property->getChild(indexChild), _prevIndentation + this->indentationSize);

            }

        }

    }
    else
    {

        // Check if this leaf has been selected
        isSelected = (this->selectedPropertyKey == _property->getKey());

        // Draw background
        if (isSelected)
        {
            paint.fillRect(0, this->topElement, this->listWidth, this->heightElement, QColor(0xEE,0xEE,0xEE));
        }
        else
        {
            paint.fillRect(0, this->topElement, this->listWidth, this->heightElement, QColor(0xFF,0xFF,0xFF));
        }

        // Draw text
        paint.setPen(QColor(0x00,0x00,0x00));
        paint.setFont(fontNoChild);
        paint.drawText(QRect(_prevIndentation + this->indentationSize, this->topElement, this->listWidth - _prevIndentation - this->indentationSize, this->heightElement), (Qt::AlignLeft | Qt::AlignVCenter), _property->getLabel());

        topIcon = (this->heightElement/2) - (this->iconSize/2);

        // Draw padding under the value
        if (isSelected)
        {
            paint.fillRect(this->widthLabel, this->topElement, this->listWidth - this->widthLabel, this->heightElement, QColor(0xEE,0xEE,0xEE));
        }
        else
        {
            paint.fillRect(this->widthLabel, this->topElement, this->listWidth - this->widthLabel, this->heightElement, QColor(0xFF,0xFF,0xFF));
        }

        // Draw icon
        if (_property->getType() == Property::P_COLOR)
        {

            paint.fillRect(this->widthLabel + this->iconLeft, this->topElement + topIcon, iconSize, iconSize, _property->getColor());

            paint.setPen(QColor(0x88,0x88,0x88));
            paint.drawRect(this->widthLabel + this->iconLeft, this->topElement + topIcon, iconSize, iconSize);

        }


  /*
        if (_property->getType() == Property::P_FLOAT)
        {

            paint.setPen(QColor(0x22,0x22,0x22));
            paint.setFont(fontIcon);
            paint.drawText(this->widthLabel + this->iconLeft, this->topElement + topIcon, iconSize, iconSize, (Qt::AlignHCenter | Qt::AlignVCenter), "1.2");

        }
        if (_property->getType() == Property::P_INT)
        {

            paint.setPen(QColor(0x22,0x22,0x22));
            paint.setFont(fontIcon);
            paint.drawText(this->widthLabel + this->iconLeft, this->topElement + topIcon, iconSize, iconSize, (Qt::AlignHCenter | Qt::AlignVCenter), "45");

        }

        if (_property->getType() == Property::P_FLOAT_POS)
        {

            paint.setPen(QColor(0x22,0x22,0x22));
            paint.setFont(fontIcon);
            paint.drawText(this->widthLabel + this->iconLeft, this->topElement + topIcon, iconSize, iconSize, (Qt::AlignHCenter | Qt::AlignVCenter), "+1.2");

        }

        if (_property->getType() == Property::P_INT_POS)
        {

            paint.setPen(QColor(0x22,0x22,0x22));
            paint.setFont(fontIcon);
            paint.drawText(this->widthLabel + this->iconLeft, this->topElement + topIcon, iconSize, iconSize, (Qt::AlignHCenter | Qt::AlignVCenter), "+45");

        }

        paint.setPen(QColor(0x88,0x88,0x88));
        paint.drawRect(this->widthLabel + this->iconLeft, this->topElement + topIcon, iconSize, iconSize);
*/

        // Draw value
        if (_property->getType() == Property::P_COLOR)
        {

            paint.setPen(Qt::black);
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, this->topElement, this->listWidth - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getColorString());

        }
        if (_property->getType() == Property::P_FLOAT)
        {

            paint.setPen(Qt::black);
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, this->topElement, this->listWidth - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getFloatString());

        }
        if (_property->getType() == Property::P_INT)
        {

            paint.setPen(Qt::black);
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, this->topElement, this->listWidth - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getIntString());

        }
        if (_property->getType() == Property::P_FLOAT_POS)
        {

            paint.setPen(Qt::black);
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, this->topElement, this->listWidth - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getFloatString());

        }
        if (_property->getType() == Property::P_INT_POS)
        {

            paint.setPen(Qt::black);
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, this->topElement, this->listWidth - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getIntString());

        }

        // Draw vertical separator
        paint.setPen(QColor(0xAA,0xAA,0xAA));
        paint.drawLine(this->widthLabel, this->topElement, this->widthLabel, this->topElement + this->heightElement - 1);
        paint.end();

        // Draw horizontal separator
        drawSeparatorLight(this->topElement + this->heightElement - 1);

    }

}


// +-------------------------------------------------------+
// | All                                                   |
// +-------------------------------------------------------+

void PropertyEditor::drawAll()
{

    // Initialize the top index
    this->topElement = 0;

    // Clear pixmap
    QPainter paint(this->pixelMap);
    //paint.setRenderHint(QPainter::SmoothPixmapTransform);
    //paint.setRenderHint(QPainter::Antialiasing);
    //paint.setRenderHint(QPainter::TextAntialiasing);

    paint.setBackground(QBrush(Qt::white));
    paint.eraseRect(0, 0, this->pixelMap->width(), this->pixelMap->height());
    paint.end();

    // Draw properties
    for (int indexProperty = 0; indexProperty < this->rootProperty->getNumberChildren(); indexProperty++)
    {
        drawProperty(this->rootProperty->getChild(indexProperty), 0);
        this->topElement += this->heightElement;
    }

}

// +-------------------------------------------------------+
// | Get property                                          |
// +-------------------------------------------------------+

Property* PropertyEditor::getProperty(Property* _property, int _mousePosition)
{

    // Initialize the top element
    this->topElement = 0;

    Property* rtnProperty;

    // Get the property
    for (int indexChild = 0; indexChild < _property->getNumberChildren(); indexChild++)
    {
        rtnProperty = getPropertyRecursive(_property->getChild(indexChild), _mousePosition);

        if (rtnProperty != NULL)
        {
            return rtnProperty;
        }
    }

    return NULL;

}

Property* PropertyEditor::getPropertyRecursive(Property* _property, int _mousePosition)
{

    int lBoundPosition = this->topElement;
    int uBoundPosition = this->topElement + this->heightElement;

    Property* rtnProperty;

    // Increment the top value
    this->topElement += this->heightElement;

    // If the mouse position is within the interval, then this property has
    // been clicked
    if ((_mousePosition > lBoundPosition) && (_mousePosition <= uBoundPosition))
    {
        return _property;
    }
    // Otherwise keep searching
    else
    {

        if (_property->isALeaf() == false)
        {

            if (_property->getExpanded() == true)
            {

                for (int indexChild = 0; indexChild < _property->getNumberChildren(); indexChild++)
                {

                    rtnProperty = getPropertyRecursive(_property->getChild(indexChild), _mousePosition);

                    if (rtnProperty != NULL)
                    {
                        return rtnProperty;
                    }

                }

            }

        }

    }

    return NULL;

}

// +-------------------------------------------------------+
// | Display text box                                      |
// +-------------------------------------------------------+

QString PropertyEditor::displayInputBox(QString _key)
{

    Property* _selectedProperty;
    bool ok;
    bool valueChanged = false;
    QString rtnString = "";
    float floatValue;
    int intValue;
    QColor colorValue;

    _selectedProperty = this->rootProperty->getProperty(_key);

    switch(_selectedProperty->getType())
    {

    case Property::P_FLOAT:

        floatValue = (float) QInputDialog::getDouble(this, "Enter a new value...", _selectedProperty->getDescription(), _selectedProperty->getFloat(), MIN_FLOAT, MAX_FLOAT, 4, &ok);

        if (ok == true)
        {
            _selectedProperty->setFloat(floatValue);
            valueChanged = true;
        }

        break;

    case Property::P_FLOAT_POS:

        floatValue = (float) QInputDialog::getDouble(this, "Enter a new value...", _selectedProperty->getDescription(), _selectedProperty->getFloat(), 0, MAX_FLOAT, 4, &ok);

        if (ok == true)
        {
            _selectedProperty->setFloat(floatValue);
            valueChanged = true;
        }

        break;

    case Property::P_INT:

        intValue = (int) QInputDialog::getInt(this, "Enter a new value...", _selectedProperty->getDescription(), _selectedProperty->getInt(), INT_MIN, INT_MAX, 1, &ok);

        if (ok == true)
        {
            _selectedProperty->setInt(intValue);
            valueChanged = true;
        }

        break;

    case Property::P_INT_POS:

        intValue = (int) QInputDialog::getInt(this, "Enter a new value...", _selectedProperty->getDescription(), _selectedProperty->getInt(), 0, INT_MAX, 1, &ok);

        if (ok == true)
        {
            _selectedProperty->setInt(intValue);
            valueChanged = true;
        }

        break;

    case Property::P_COLOR:

        colorValue = QColorDialog::getColor(_selectedProperty->getColor());

        if (colorValue.isValid())
        {
            _selectedProperty->setColor(colorValue);
            valueChanged = true;
        }

        break;

    case Property::P_NOTYPE:

        break;
    }

    if (valueChanged == true)
    {
        rtnString = _selectedProperty->getValueString();
    }

    return (rtnString);

}
