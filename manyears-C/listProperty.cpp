#include "listProperty.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

ListProperty::ListProperty(int _width, int _height)
{
    this->listDown.clear();
    this->numberChildren = 0;
    this->numberDisplay = 0;

    this->width = _width;
    this->height = _height;
    this->displayTop = 0;

    this->iconSize = 16;
    this->squareSize = 10;
    this->indentationSize = 20;
    this->heightElement = 25;
    this->iconLeft = 5;
    this->iconRight = 5;
    this->widthLabel = 0.7*_width;

    this->pixelMap = new QPixmap(this->width, this->height);

    this->drawAllElements("");
    this->freezeSignal = false;

}

/***********************************************************
* Destructor                                               *
***********************************************************/

ListProperty::~ListProperty()
{

    delete this->pixelMap;

}

/***********************************************************
* Add a child                                              *
***********************************************************/

void ListProperty::addChild(Property *_child)
{

    this->listDown.push_back(_child);
    this->numberChildren++;

}

/***********************************************************
* Load properties                                          *
***********************************************************/

void ListProperty::loadParameters(Property *_rootProperty)
{

    this->isBeingUsed.lock();

    this->listDown.clear();
    this->numberChildren = 0;

    for (int indexElement = 0; indexElement < _rootProperty->getNumberChildren(); indexElement++)
    {

        this->addChild(_rootProperty->getChild(indexElement));

    }

    this->isBeingUsed.unlock();

}

/***********************************************************
* Modify properties                                        *
***********************************************************/

// +-------------------------------------------------------+
// | Read property                                         |
// +-------------------------------------------------------+

    float ListProperty::getPropertyFloat(QString _key)
    {

        float tmp;

        this->isBeingUsed.lock();
        tmp = (getProperty(_key)->getFloat());
        this->isBeingUsed.unlock();

        return tmp;

    }


    QString ListProperty::getPropertyFloatString(QString _key)
    {

        QString tmp;

        this->isBeingUsed.lock();
        tmp = (getProperty(_key)->getFloatString());
        this->isBeingUsed.unlock();

        return tmp;

    }

    int ListProperty::getPropertyInt(QString _key)
    {

        int tmp;

        this->isBeingUsed.lock();
        tmp = (getProperty(_key)->getInt());
        this->isBeingUsed.unlock();

        return tmp;

    }

    QString ListProperty::getPropertyIntString(QString _key)
    {

        QString tmp;

        this->isBeingUsed.lock();
        tmp = (getProperty(_key)->getIntString());
        this->isBeingUsed.unlock();

        return tmp;

    }

    QColor ListProperty::getPropertyColor(QString _key)
    {

        QColor tmp;

        this->isBeingUsed.lock();
        tmp = (getProperty(_key)->getColor());
        this->isBeingUsed.unlock();

        return tmp;

    }

    QString ListProperty::getPropertyColorString(QString _key)
    {

        QString tmp;

        this->isBeingUsed.lock();
        tmp = (getProperty(_key)->getColorString());
        this->isBeingUsed.unlock();

        return tmp;

    }

    QString ListProperty::getPropertyValueString(QString _key)
    {

        QString tmp;

        this->isBeingUsed.lock();
        tmp = (getProperty(_key)->getValueString());
        this->isBeingUsed.unlock();

        return tmp;

    }

// +-------------------------------------------------------+
// | Write property                                        |
// +-------------------------------------------------------+

    void ListProperty::setPropertyFloat(QString _key, float _value)
    {

        this->isBeingUsed.lock();
        getProperty(_key)->setFloat(_value);
        this->isBeingUsed.unlock();

    }

    void ListProperty::setPropertyInt(QString _key, int _value)
    {

        this->isBeingUsed.lock();
        getProperty(_key)->setInt(_value);
        this->isBeingUsed.unlock();

    }

    void ListProperty::setPropertyColor(QString _key, QColor _value)
    {

        this->isBeingUsed.lock();
        getProperty(_key)->setColor(_value);
        this->isBeingUsed.unlock();

    }

    void ListProperty::setPropertyValueString(QString _key, QString _value)
    {

        this->isBeingUsed.lock();
        getProperty(_key)->setValue(_value);
        this->isBeingUsed.unlock();

    }

/***********************************************************
* Generate a configuration vector                          *
***********************************************************/

void ListProperty::generateVector(ConfigurationVector *_currentVector)
{

    this->isBeingUsed.lock();

    for (int indexElement = 0; indexElement < this->numberChildren; indexElement++)
    {
        this->generateVectorRecursive(this->listDown[indexElement],_currentVector);
    }

    this->isBeingUsed.unlock();

}

void ListProperty::generateVectorRecursive(Property *_currentProperty, ConfigurationVector *_currentVector)
{

    if (_currentProperty->getNumberChildren() == 0)
    {
        switch(_currentProperty->getType())
        {

        case Property::P_COLOR:

            _currentVector->addElement(_currentProperty->getKey(),_currentProperty->getColor());
            break;

        case Property::P_FLOAT:

            _currentVector->addElement(_currentProperty->getKey(),_currentProperty->getFloat());
            break;

        case Property::P_INT:

            _currentVector->addElement(_currentProperty->getKey(),_currentProperty->getInt());
            break;

        case Property::P_FLOAT_POS:

            _currentVector->addElement(_currentProperty->getKey(),_currentProperty->getFloat());
            break;

        case Property::P_INT_POS:

            _currentVector->addElement(_currentProperty->getKey(),_currentProperty->getInt());
            break;

        }
    }
    else
    {
        for (int indexChild = 0; indexChild < _currentProperty->getNumberChildren(); indexChild++)
        {
            this->generateVectorRecursive(_currentProperty->getChild(indexChild),_currentVector);
        }
    }

}

/***********************************************************
* Do not udpate the graph                                  *
***********************************************************/

void ListProperty::freezeScrollBarUpdate()
{
    this->mutexFreezeSignal.lock();

    this->freezeSignal = true;

    this->mutexFreezeSignal.unlock();
}

/***********************************************************
* Allow udpating the graph                                 *
***********************************************************/

void ListProperty::unfreezeScrollBarUpdate()
{
    this->mutexFreezeSignal.lock();

    this->freezeSignal = false;

    this->mutexFreezeSignal.unlock();
}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Display Height                                        |
// +-------------------------------------------------------+

int ListProperty::getDisplayHeight()
{
    int tmp;

    this->mutexDimension.lock();
    tmp = this->displayHeight;
    this->mutexDimension.unlock();

    return (tmp);
}

// +-------------------------------------------------------+
// | Height of elements                                    |
// +-------------------------------------------------------+

int ListProperty::getHeightElement()
{
    int tmp;

    tmp = this->heightElement;

    return (tmp);
}

/***********************************************************
* Reset display top                                        *
***********************************************************/

void ListProperty::resetDisplayTop()
{
    this->isBeingUsed.lock();
    this->displayTop = 0;
    this->isBeingUsed.unlock();
}


/***********************************************************
* Protected events                                         *
***********************************************************/

// +-------------------------------------------------------+
// | Paint                                                 |
// +-------------------------------------------------------+

void ListProperty::paintEvent( QPaintEvent * )
{

    QPainter paint(this);

    paint.drawPixmap(0,0,*(this->pixelMap));

    paint.end();

}

/***********************************************************
* Draw                                                     *
***********************************************************/

// +-------------------------------------------------------+
// | + Sign                                                |
// +-------------------------------------------------------+

void ListProperty::drawPlus(QColor _color, int _left, int _top)
{

    QPainter paint(this->pixelMap);

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

void ListProperty::drawMinus(QColor _color, int _left, int _top)
{

    QPainter paint(this->pixelMap);

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
// | Top most category                                     |
// +-------------------------------------------------------+

void ListProperty::drawTopCategory(int _top, Property *_property)
{

    QPainter paint(this->pixelMap);
    QFont font("Times",8,QFont::Bold,false);

    int leftSquare;
    int topSquare;

    // Draw background
    paint.fillRect(0,_top,this->width,this->heightElement,QColor(0x99,0x99,0x99));
    paint.fillRect(0,_top,this->width,(int) (this->heightElement*0.4),QColor(0xAA,0xAA,0xAA));

    // Write text
    paint.setPen(QColor(0xFF,0xFF,0xFF));
    paint.setFont(font);
    paint.drawText(this->indentationSize, _top, this->width - this->indentationSize, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getLabel());
    paint.end();

    // Draw plus/minus

    leftSquare = (this->indentationSize - this->squareSize) / 2;
    topSquare = (this->heightElement - this->squareSize) / 2;

    if (_property->getNumberChildren() > 0)
    {
        if (_property->getExpanded()==true)
        {
            this->drawMinus(QColor(0xFF,0xFF,0xFF),leftSquare,topSquare + _top);
        }
        else
        {
            this->drawPlus(QColor(0xFF,0xFF,0xFF),leftSquare,topSquare + _top);
        }
    }

}

// +-------------------------------------------------------+
// | Category                                              |
// +-------------------------------------------------------+

void ListProperty::drawCategory(int _top, Property *_property, int _prevIndentation)
{

    int leftSquare;
    int topSquare;
    int topIcon;

    QPainter paint(this->pixelMap);
    QFont fontWithChild("Times",8,QFont::Bold,false);
    QFont fontNoChild("Times",8,QFont::Normal,false);
    QFont fontIcon("Times",5,QFont::Normal,false);
    QFont fontValue("Times",8,QFont::Normal,false);

    // Check if there are children
    if (_property->getNumberChildren() > 0)
    {

        // Draw background
        paint.fillRect(0,_top,this->width,this->heightElement,QColor(0xFF,0xFF,0xFF));

        // Draw text
        paint.setPen(QColor(0x00,0x00,0x00));
        paint.setFont(fontWithChild);
        paint.drawText(_prevIndentation + this->indentationSize, _top, this->width - _prevIndentation - this->indentationSize, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getLabel());
        paint.end();

        leftSquare = (this->indentationSize - this->squareSize) / 2;
        topSquare = (this->heightElement - this->squareSize) / 2;

        // Draw plus/minus
        if (_property->getExpanded() == true)
        {
            this->drawMinus(QColor(0x00,0x00,0x00), _prevIndentation + leftSquare, _top + topSquare);
        }
        else
        {
            this->drawPlus(QColor(0x00,0x00,0x00), _prevIndentation + leftSquare, _top + topSquare);
        }

    }
    else
    {

        // Draw background
        if (_property->getExpanded() == true)
        {
            paint.fillRect(0,_top,this->width,this->heightElement,QColor(0xEE,0xEE,0xEE));
        }
        else
        {
            paint.fillRect(0,_top,this->width,this->heightElement,QColor(0xFF,0xFF,0xFF));
        }

        // Draw text
        paint.setPen(QColor(0x00,0x00,0x00));
        paint.setFont(fontNoChild);
        paint.drawText(_prevIndentation + this->indentationSize, _top, this->width - _prevIndentation - this->indentationSize, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getLabel());

        topIcon = (this->heightElement/2) - (this->iconSize/2);

        // Draw padding under the value
        if (_property->getExpanded() == true)
        {
            paint.fillRect(this->widthLabel, _top, this->width - this->widthLabel, this->heightElement, QColor(0xEE,0xEE,0xEE));
        }
        else
        {
            paint.fillRect(this->widthLabel, _top, this->width - this->widthLabel, this->heightElement, QColor(0xFF,0xFF,0xFF));
        }

        // Draw icon
        if (_property->getType() == Property::P_COLOR)
        {

            paint.fillRect(this->widthLabel + this->iconLeft, _top + topIcon, iconSize, iconSize, _property->getColor());

        }
        if (_property->getType() == Property::P_FLOAT)
        {

            paint.setPen(QColor(0x22,0x22,0x22));
            paint.setFont(fontIcon);
            paint.drawText(this->widthLabel + this->iconLeft, _top + topIcon, iconSize, iconSize, (Qt::AlignHCenter | Qt::AlignVCenter), "1.2");

        }
        if (_property->getType() == Property::P_INT)
        {

            paint.setPen(QColor(0x22,0x22,0x22));
            paint.setFont(fontIcon);
            paint.drawText(this->widthLabel + this->iconLeft, _top + topIcon, iconSize, iconSize, (Qt::AlignHCenter | Qt::AlignVCenter), "45");

        }

        if (_property->getType() == Property::P_FLOAT_POS)
        {

            paint.setPen(QColor(0x22,0x22,0x22));
            paint.setFont(fontIcon);
            paint.drawText(this->widthLabel + this->iconLeft, _top + topIcon, iconSize, iconSize, (Qt::AlignHCenter | Qt::AlignVCenter), "+1.2");

        }

        if (_property->getType() == Property::P_INT_POS)
        {

            paint.setPen(QColor(0x22,0x22,0x22));
            paint.setFont(fontIcon);
            paint.drawText(this->widthLabel + this->iconLeft, _top + topIcon, iconSize, iconSize, (Qt::AlignHCenter | Qt::AlignVCenter), "+45");

        }

        paint.setPen(QColor(0x88,0x88,0x88));
        paint.drawRect(this->widthLabel + this->iconLeft, _top + topIcon, iconSize, iconSize);

        // Draw value
        if (_property->getType() == Property::P_COLOR)
        {

            paint.setPen(QColor(0x00,0x00,0x00));
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, _top, this->width - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getColorString());

        }
        if (_property->getType() == Property::P_FLOAT)
        {

            paint.setPen(QColor(0x00,0x00,0x00));
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, _top, this->width - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getFloatString());

        }
        if (_property->getType() == Property::P_INT)
        {

            paint.setPen(QColor(0x00,0x00,0x00));
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, _top, this->width - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getIntString());

        }
        if (_property->getType() == Property::P_FLOAT_POS)
        {

            paint.setPen(QColor(0x00,0x00,0x00));
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, _top, this->width - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getFloatString());

        }
        if (_property->getType() == Property::P_INT_POS)
        {

            paint.setPen(QColor(0x00,0x00,0x00));
            paint.setFont(fontValue);
            paint.drawText(this->widthLabel + this->iconLeft + this->iconSize + this->iconRight, _top, this->width - this->widthLabel - this->iconSize - this->iconLeft - this->iconRight, this->heightElement, (Qt::AlignLeft | Qt::AlignVCenter), _property->getIntString());

        }

        // Draw vertical separator
        paint.setPen(QColor(0xAA,0xAA,0xAA));
        paint.drawLine(this->widthLabel,_top,this->widthLabel,_top + this->heightElement - 1);
        paint.end();

    }

}

// +-------------------------------------------------------+
// | Separator light                                       |
// +-------------------------------------------------------+

void ListProperty::drawSeparatorLight(int _top)
{
    QPainter paint(this->pixelMap);

    paint.setPen(QColor(0xAA,0xAA,0xAA));
    paint.drawLine(0,_top,this->width,_top);

    paint.end();

}

// +-------------------------------------------------------+
// | Separator dark                                        |
// +-------------------------------------------------------+

void ListProperty::drawSeparatorDark(int _top)
{
    QPainter paint(this->pixelMap);

    paint.setPen(QColor(0x44,0x44,0x44));
    paint.drawLine(0,_top,this->width,_top);

    paint.end();

}

// +-------------------------------------------------------+
// | Border                                                |
// +-------------------------------------------------------+

void ListProperty::drawBorder()
{
    QPainter paint(this->pixelMap);

    paint.setPen(QColor(0x44,0x44,0x44));
    paint.drawRect(0,0,this->width-1,this->height-1);

    paint.end();

}

// +-------------------------------------------------------+
// | All children                                          |
// +-------------------------------------------------------+

int ListProperty::drawChildren(Property *_parent, int _indentation, int _top, QString _keySelected)
{
    int currentTop;
    int currentIndentation;

    currentTop = _top;
    currentIndentation = _indentation;

    for (int indexChild = 0; indexChild < _parent->getNumberChildren(); indexChild++)
    {
        // Set position
        this->keys.push_back(_parent->getChild(indexChild)->getKey());
        this->startMouse.push_back(currentTop);
        this->endMouse.push_back(currentTop + this->heightElement);
        this->numberDisplay++;

        // Check if selection is changed
        if (_parent->getChild(indexChild)->getKey() == _keySelected)
        {
            if (_parent->getChild(indexChild)->getNumberChildren() > 0)
            {
                _parent->getChild(indexChild)->setExpanded(!_parent->getChild(indexChild)->getExpanded());
            }
            else
            {
                _parent->getChild(indexChild)->setExpanded(true);
            }
        }
        else
        {
            if (_parent->getChild(indexChild)->getNumberChildren() == 0)
            {
                _parent->getChild(indexChild)->setExpanded(false);
            }
        }


        // Draw the current child
        this->drawCategory(currentTop, _parent->getChild(indexChild), currentIndentation);

        currentTop += this->heightElement;

        this->drawSeparatorLight(currentTop);

        currentTop++;

        if ((_parent->getChild(indexChild)->getExpanded() == true) && (_parent->getChild(indexChild)->getNumberChildren()>0))
        {
            currentTop = this->drawChildren(_parent->getChild(indexChild),(currentIndentation + this->indentationSize),currentTop,_keySelected);
        }
    }

    return currentTop;
}

// +-------------------------------------------------------+
// | All elements                                          |
// +-------------------------------------------------------+

void ListProperty::drawAllElements(QString _keySelected)
{

    int currentTop;
    int currentIndentation;

    QPainter paint(this->pixelMap);

    paint.fillRect(0,0,this->width,this->height,QColor(0xFF,0xFF,0xFF));

    paint.end();

    currentTop = this->displayTop;
    currentIndentation = 0;

    this->numberDisplay = 0;
    this->keys.clear();
    this->startMouse.clear();
    this->endMouse.clear();

    for (int indexElement = 0; indexElement < this->numberChildren; indexElement++)
    {

        // Set position
        this->keys.push_back(this->listDown[indexElement]->getKey());
        this->startMouse.push_back(currentTop);
        this->endMouse.push_back(currentTop + this->heightElement);
        this->numberDisplay++;

        // Check if selection is changed
        if (this->listDown[indexElement]->getKey() == _keySelected)
        {
            this->listDown[indexElement]->setExpanded(!this->listDown[indexElement]->getExpanded());
        }

        // Draw top most category
        this->drawTopCategory(currentTop,this->listDown[indexElement]);

        currentTop += this->heightElement;

        // Draw separator
        this->drawSeparatorDark(currentTop);

        currentTop++;

        if ((this->listDown[indexElement]->getExpanded() == true) && (this->listDown[indexElement]->getNumberChildren()>0))
        {

            currentTop = this->drawChildren(this->listDown[indexElement],currentIndentation,currentTop,_keySelected);

        }

    }

    this->drawBorder();

    this->mutexDimension.lock();
    this->displayHeight = currentTop - this->displayTop;
    this->mutexDimension.unlock();

}

// +-------------------------------------------------------+
// | Get key                                               |
// +-------------------------------------------------------+

QString ListProperty::getKey(int _mousePosition)
{

    int indexDisplay;
    bool indexFound;

    indexFound = false;

    for (indexDisplay = 0; indexDisplay < this->numberDisplay; indexDisplay++)
    {
        if ((this->startMouse[indexDisplay] < _mousePosition) && (this->endMouse[indexDisplay] > _mousePosition))
        {
            indexFound = true;
            break;
        }
    }

    if (indexFound == false)
    {
        return("");
    }
    else
    {
        return(this->keys[indexDisplay]);
    }

}

// +-------------------------------------------------------+
// | Get property                                          |
// +-------------------------------------------------------+

Property *ListProperty::getProperty(QString _key)
{

    for (int indexElement = 0; indexElement < this->numberChildren; indexElement++)
    {

        if (this->listDown[indexElement]->getProperty(_key) != NULL)
        {
            return (this->listDown[indexElement]->getProperty(_key));
        }

    }

    return NULL;

}

// +-------------------------------------------------------+
// | Refresh selection                                     |
// +-------------------------------------------------------+

void ListProperty::refreshSelection(int _mousePosition)
{

    this->drawAllElements(this->getKey(_mousePosition));
    emit(this->isResized());
    this->update();

}

// +-------------------------------------------------------+
// | Display text box                                      |
// +-------------------------------------------------------+

bool ListProperty::displayInputBox(int _mousePosition)
{ 

    QString key;
    Property *currentProperty;
    bool ok;
    float floatValue;
    int intValue;
    QColor colorValue;

    key = this->getKey(_mousePosition);

    if (key != "")
    {

        currentProperty = this->getProperty(key);

        if ((currentProperty->getNumberChildren() == 0) && ((currentProperty->getHasParent()) == true))
        {
            if (currentProperty->getType() == Property::P_FLOAT)
            {
                floatValue = (float) QInputDialog::getDouble(this,"Enter a new value...",currentProperty->getDescription(),currentProperty->getFloat(),-3.4028E+38, +3.4028E+38, 4, &ok);

                if (ok == true)
                {
                    currentProperty->setFloat(floatValue);
                }

            }
            else if (currentProperty->getType() == Property::P_INT)
            {
                intValue = (int) QInputDialog::getInt(this,"Enter a new value...",currentProperty->getDescription(),currentProperty->getInt(),-2147483648,2147483647,1,&ok);

                if (ok == true)
                {
                    currentProperty->setInt(intValue);
                }
            }
            else if (currentProperty->getType() == Property::P_COLOR)
            {
                colorValue = QColorDialog::getColor(currentProperty->getColor());

                if (colorValue.isValid())
                {
                    currentProperty->setColor(colorValue);
                }
            }
            else if (currentProperty->getType() == Property::P_FLOAT_POS)
            {
                floatValue = (float) QInputDialog::getDouble(this,"Enter a new value...",currentProperty->getDescription(),currentProperty->getFloat(),0, +3.4028E+38, 4, &ok);

                if (ok == true)
                {
                    currentProperty->setFloat(floatValue);
                }

            }
            else if (currentProperty->getType() == Property::P_INT_POS)
            {
                intValue = (int) QInputDialog::getInt(this,"Enter a new value...",currentProperty->getDescription(),currentProperty->getInt(),0,2147483647,1,&ok);

                if (ok == true)
                {
                    currentProperty->setInt(intValue);
                }
            }

            // Refresh the display
            this->refreshSelection(_mousePosition);

        // If OK was pressed, then return true
        return ((ok == true) || (colorValue.isValid()));

        }

    }
}

/***********************************************************
* Change the position of the top of the list               *
***********************************************************/

void ListProperty::scrollBarValueChanged(int _newValue)
{

    this->isBeingUsed.lock();

    bool tmp;

    this->mutexFreezeSignal.lock();
    tmp = this->freezeSignal;
    this->mutexFreezeSignal.unlock();

    if (tmp == false)
    {

        this->displayTop = -1 * _newValue;

        this->drawAllElements("");
        this->update();
    }

    this->isBeingUsed.unlock();

}


/***********************************************************
* User interaction                                         *
***********************************************************/

// +-------------------------------------------------------+
// | Mouse is pressed                                      |
// +-------------------------------------------------------+

void ListProperty::mousePressEvent(QMouseEvent *event)
{

    this->isBeingUsed.lock();

    QPoint position;

    position = event->pos();

    this->refreshSelection(position.y());

    this->isBeingUsed.unlock();

}

// +-------------------------------------------------------+
// | Mouse is double-clicked                               |
// +-------------------------------------------------------+

void ListProperty::mouseDoubleClickEvent(QMouseEvent *event)
{

    this->isBeingUsed.lock();

    QPoint position;

    bool valueChanged;

    position = event->pos();

    valueChanged = displayInputBox(position.y());

    this->isBeingUsed.unlock();
    
    if (valueChanged == true)
    {
        emit(this->valueChanged(-1));
    }

}

// +-------------------------------------------------------+
// | Resize occurs                                         |
// +-------------------------------------------------------+

void ListProperty::resizeEvent(QResizeEvent *event)
{

    this->isBeingUsed.lock();

    QSize newSize;

    newSize = event->size();

    delete this->pixelMap;
    this->pixelMap = new QPixmap(newSize.width(),newSize.height());

    this->width = newSize.width();
    this->height = newSize.height();
    this->widthLabel = 0.7*this->width;

    this->drawAllElements("");
    this->update();

    this->isBeingUsed.unlock();

}
