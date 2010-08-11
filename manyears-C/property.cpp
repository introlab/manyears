#include "property.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

Property::Property()
{

    this->label = "";
    this->description = "";
    this->key = "";
    this->type = Property::P_NOTYPE;
    this->listDown.clear();
    this->numberChildren = 0;
    this->expanded = false;
    this->hasParent = false;
    this->setColor(QColor(0,0,0));
    this->setFloat(0.0);
    this->setInt(0);

}

Property::Property(const Property& _property)
{

    *this = _property;

}

Property::Property(const QString _label, const QString _description, const QString _key, const int _type)
{

    this->label = _label;
    this->description = _description;
    this->key = _key;
    this->type = _type;
    this->listDown.clear();
    this->numberChildren = 0;
    this->expanded = false;
    this->hasParent = false;
    this->setColor(QColor(0,0,0));
    this->setFloat(0.0);
    this->setInt(0);

}

/***********************************************************
* Destructor                                               *
***********************************************************/

Property::~Property()
{

    // Delete recursively all children
    for (int indexChild = 0; indexChild < this->numberChildren; indexChild++)
    {
        delete (this->listDown[indexChild]);
    }

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

Property& Property::operator=(const Property& _property)
{

    this->copy(&_property);

    return *this;

}

void Property::copy(const Property* _property)
{

    this->label = _property->getLabel();
    this->description = _property->getDescription();
    this->key = _property->getKey();
    this->type = _property->getType();
    this->numberChildren = _property->getNumberChildren();

    for (int indexChild = 0; indexChild < _property->getNumberChildren(); indexChild++)
    {

        Property* _child = new Property();
        _child->setHasParent(true);
        _child->copy(_property->getChild(indexChild));
        this->listDown.push_back(_child);

    }

}

/***********************************************************
* Add a child                                              *
***********************************************************/

void Property::addChild(Property* _child)
{

    _child->setHasParent(true);
    this->listDown.push_back(_child);
    this->numberChildren++;

}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Float value                                            |
// +-------------------------------------------------------+

void Property::setFloat(float _floatValue)
{

    this->floatValue = _floatValue;

}

// +-------------------------------------------------------+
// | Int value                                             |
// +-------------------------------------------------------+

void Property::setInt(int _intValue)
{

    this->intValue = _intValue;

}

// +-------------------------------------------------------+
// | Color value                                           |
// +-------------------------------------------------------+

void Property::setColor(QColor _colorValue)
{

    this->colorValue = _colorValue;

}

// +-------------------------------------------------------+
// | Value for unknown type                                |
// +-------------------------------------------------------+

void Property::setValue(QString _value)
{

    int red, green, blue;
    QString redHex, greenHex, blueHex;
    bool ok;

    switch(this->type)
    {

        case Property::P_COLOR:

        // Remove 0x if it is there
        if ((_value.length() == 8) && (_value.left(2).toLower() == "0x"))
        {
            _value = _value.right(_value.length() - 2);
        }

        redHex = _value.mid(0,2);
        greenHex = _value.mid(2,2);
        blueHex = _value.mid(4,2);

        red = redHex.toInt(&ok,16);
        green = greenHex.toInt(&ok,16);
        blue = blueHex.toInt(&ok,16);

        this->setColor(QColor(red,green,blue));

        break;

        case Property::P_FLOAT:

        this->setFloat(_value.toFloat());

        break;

        case Property::P_FLOAT_POS:

        this->setFloat(_value.toFloat());

        break;

        case Property::P_INT:

        this->setInt(_value.toInt());

        break;

        case Property::P_INT_POS:

        this->setInt(_value.toInt());

        break;

    }

}


// +-------------------------------------------------------+
// | Expanded                                              |
// +-------------------------------------------------------+

void Property::setExpanded(bool _isExpanded)
{

    this->expanded = _isExpanded;

}

// +-------------------------------------------------------+
// | Has parent                                            |
// +-------------------------------------------------------+

void Property::setHasParent(bool _hasParent)
{

    this->hasParent = _hasParent;

}

/***********************************************************
* Accessor                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Float value                                           |
// +-------------------------------------------------------+

float Property::getFloat() const
{

    return (this->floatValue);

}

// +-------------------------------------------------------+
// | Float string                                          |
// +-------------------------------------------------------+

QString Property::getFloatString() const
{

    QString floatString;

    floatString.setNum(this->floatValue,'g',6);

    return (floatString);
}

// +-------------------------------------------------------+
// | Int value                                             |
// +-------------------------------------------------------+

int Property::getInt() const
{

    return (this->intValue);

}

// +-------------------------------------------------------+
// | Int string                                            |
// +-------------------------------------------------------+

QString Property::getIntString() const
{
    QString intString;

    intString.setNum(this->intValue,10);

    return (intString);
}

// +-------------------------------------------------------+
// | Color value                                           |
// +-------------------------------------------------------+

QColor Property::getColor() const
{

    return (this->colorValue);

}

// +-------------------------------------------------------+
// | Color RGB string                                      |
// +-------------------------------------------------------+

QString Property::getColorString() const
{

    int red, green, blue;
    QString redHex, greenHex, blueHex;

    this->colorValue.getRgb(&red,&green,&blue);

    redHex.setNum(red,16);

    if (redHex.length() == 1)
    {
        redHex = "0" + redHex;
    }

    greenHex.setNum(green,16);

    if (greenHex.length() == 1)
    {
        greenHex = "0" + greenHex;
    }

    blueHex.setNum(blue,16);

    if (blueHex.length() == 1)
    {
        blueHex = "0" + blueHex;
    }

    return ("0x" + redHex + greenHex + blueHex);

}

// +-------------------------------------------------------+
// | Get the value as a string no matter what is the type  |
// +-------------------------------------------------------+

QString Property::getValueString() const
{

    QString tmp;

    switch(this->getType())
    {

    case Property::P_COLOR:
            tmp = this->getColorString();
            break;
    case Property::P_FLOAT:
            tmp = this->getFloatString();
            break;
    case Property::P_FLOAT_POS:
            tmp = this->getFloatString();
            break;
    case Property::P_INT:
            tmp = this->getIntString();
            break;
    case Property::P_INT_POS:
            tmp = this->getIntString();
            break;
    case Property::P_NOTYPE:
            tmp = "";
            break;

    }

    return tmp;

}


// +-------------------------------------------------------+
// | Expanded                                          |
// +-------------------------------------------------------+

bool Property::getExpanded() const
{

    return (this->expanded);

}

// +-------------------------------------------------------+
// | Label                                                 |
// +-------------------------------------------------------+

QString Property::getLabel() const
{

    return (this->label);

}

// +-------------------------------------------------------+
// | Description                                           |
// +-------------------------------------------------------+

QString Property::getDescription() const
{

    return (this->description);

}

// +-------------------------------------------------------+
// | Key                                                   |
// +-------------------------------------------------------+

QString Property::getKey() const
{

    return (this->key);

}

// +-------------------------------------------------------+
// | Type                                                  |
// +-------------------------------------------------------+

int Property::getType() const
{

    return (this->type);

}

// +-------------------------------------------------------+
// | Number of children                                    |
// +-------------------------------------------------------+

int Property::getNumberChildren() const
{

    return (this->numberChildren);

}

// +-------------------------------------------------------+
// | Number of children (counting expanded children)       |
// +-------------------------------------------------------+

int Property::getNumberChildrenRecursiveExpanded() const
{

    int totalProperty = 1;

    // Browse only if it is expanded
    if (this->getExpanded() == true)
    {

        for (int indexChild = 0; indexChild < this->getNumberChildren(); indexChild++)
        {

            totalProperty += this->getChild(indexChild)->getNumberChildrenRecursiveExpanded();

        }

    }

    return totalProperty;

}

// +-------------------------------------------------------+
// | Number of children (counting all recursive children)  |
// +-------------------------------------------------------+

int Property::getNumberChildrenRecursiveAll() const
{

    int totalProperty = 1;

    for (int indexChild = 0; indexChild < this->getNumberChildren(); indexChild++)
    {
        totalProperty += this->getChild(indexChild)->getNumberChildrenRecursiveAll();
    }

    return totalProperty;

}

// +-------------------------------------------------------+
// | Children                                              |
// +-------------------------------------------------------+

Property *Property::getChild(int _childIndex) const
{

    return (this->listDown[_childIndex]);

}

// +-------------------------------------------------------+
// | Property with a key                                   |
// +-------------------------------------------------------+

Property *Property::getProperty(QString _key)
{

    if (this->getKey() == _key)
    {
        return (this);
    }

    for (int indexChild = 0; indexChild < this->getNumberChildren(); indexChild++)
    {

        Property* tmp = this->getChild(indexChild)->getProperty(_key);

        if (tmp != NULL)
        {
            return (tmp);
        }
    }

    return (NULL);

}

// +-------------------------------------------------------+
// | Has parent                                            |
// +-------------------------------------------------------+

bool Property::getHasParent() const
{

    return (this->hasParent);

}

// +-------------------------------------------------------+
// | Is a leaf                                             |
// +-------------------------------------------------------+

bool Property::isALeaf() const
{
    return (this->getNumberChildren() == 0);
}
