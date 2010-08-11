#include "configurationVector.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

ConfigurationVector::ConfigurationVector()
{
    this->reset();
}

ConfigurationVector::ConfigurationVector(const ConfigurationVector& _configurationVector)
{
    *this = _configurationVector;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

ConfigurationVector::~ConfigurationVector()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

ConfigurationVector& ConfigurationVector::operator=(const ConfigurationVector& _configurationVector)
{

    reset();

    for (int indexElement = 0; indexElement < _configurationVector.getNumberElements(); indexElement++)
    {
        this->addElement(_configurationVector.getKey(indexElement), _configurationVector.getValue(_configurationVector.getKey(indexElement)));
    }

    return (*this);

}

// +-------------------------------------------------------+
// | Plus                                                  |
// +-------------------------------------------------------+

ConfigurationVector ConfigurationVector::operator+(const ConfigurationVector& _configurationVector)
{

    ConfigurationVector tmp;

    for (int indexElement = 0; indexElement < _configurationVector.getNumberElements(); indexElement++)
    {
        tmp.addElement(_configurationVector.getKey(indexElement), _configurationVector.getValue(_configurationVector.getKey(indexElement)));
    }

    return (tmp);

}

/***********************************************************
* Add element                                              *
***********************************************************/

void ConfigurationVector::addElement(QString _key, float _value)
{

    QString currentValue;

    currentValue.setNum(_value,'g',10);
    currentValue = "F" + currentValue;

    this->keys.push_back(_key);
    this->values.push_back(currentValue);

    this->numberElements++;

}

void ConfigurationVector::addElement(QString _key, int _value)
{

    QString currentValue;

    currentValue.setNum(_value,10);
    currentValue = "I" + currentValue;

    this->keys.push_back(_key);
    this->values.push_back(currentValue);

    this->numberElements++;

}

void ConfigurationVector::addElement(QString _key, QColor _value)
{

    QString currentValue;
    int red,green,blue;
    QString redHex, greenHex, blueHex;

    _value.getRgb(&red,&green,&blue);
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

    currentValue = "C" + redHex + greenHex + blueHex;

    this->keys.push_back(_key);
    this->values.push_back(currentValue);

    this->numberElements++;

}

void ConfigurationVector::addElement(Property* _property)
{

    switch (_property->getType())
    {

        case Property::P_COLOR:

            this->addElement(_property->getKey(),_property->getColor());

            break;

        case Property::P_FLOAT:

            this->addElement(_property->getKey(),_property->getFloat());

            break;

        case Property::P_FLOAT_POS:

            this->addElement(_property->getKey(),_property->getFloat());

            break;

        case Property::P_INT:

            this->addElement(_property->getKey(),_property->getInt());

            break;

        case Property::P_INT_POS:

            this->addElement(_property->getKey(),_property->getInt());

            break;

    }

}

void ConfigurationVector::addElement(QString _key, QString _value)
{

    this->keys.push_back(_key);
    this->values.push_back(_value);

    this->numberElements++;

}

void ConfigurationVector::setValue(QString _key, float _value)
{

    QString currentValue;

    currentValue.setNum(_value,'g',10);
    currentValue = "F" + currentValue;

    setValue(_key, currentValue);

}

void ConfigurationVector::setValue(QString _key, int _value)
{

    QString currentValue;

    currentValue.setNum(_value,10);
    currentValue = "I" + currentValue;

    setValue(_key, currentValue);

}

void ConfigurationVector::setValue(QString _key, QColor _value)
{

    QString currentValue;
    int red,green,blue;
    QString redHex, greenHex, blueHex;

    _value.getRgb(&red,&green,&blue);
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

    currentValue = "C" + redHex + greenHex + blueHex;

    setValue(_key, currentValue);

}

void ConfigurationVector::setValue(QString _key, QString _value)
{

    for (int indexElement = 0; indexElement < this->numberElements; indexElement++)
    {
        if (this->keys[indexElement] == _key)
        {
            this->values[indexElement] = _value;
        }
    }

}


/***********************************************************
* Reset the vector                                         *
***********************************************************/

void ConfigurationVector::reset()
{

    this->keys.clear();
    this->values.clear();
    this->numberElements = 0;

}


/***********************************************************
* Get element                                              *
***********************************************************/

float ConfigurationVector::getElementFloat(QString _key) const
{

    for (int indexElement = 0; indexElement < this->numberElements; indexElement++)
    {
        if (this->keys[indexElement] == _key)
        {

            if (this->values[indexElement].left(1) == "F")
            {
                return (this->values[indexElement].right(this->values[indexElement].length()-1).toFloat());
            }

        }
    }

    return 0.0;

}

int ConfigurationVector::getElementInt(QString _key) const
{
    for (int indexElement = 0; indexElement < this->numberElements; indexElement++)
    {      
        if (this->keys[indexElement] == _key)
        {

            if (this->values[indexElement].left(1) == "I")
            {
                return (this->values[indexElement].right(this->values[indexElement].length()-1).toInt());
            }

        }

    }

    return 0;
}

QColor ConfigurationVector::getElementColor(QString _key) const
{

    bool ok;

    for (int indexElement = 0; indexElement < this->numberElements; indexElement++)
    {
        if (this->keys[indexElement] == _key)
        {

            if (this->values[indexElement].left(1) == "C")
            {
                return (QColor(this->values[indexElement].mid(1,2).toInt(&ok, 16) , this->values[indexElement].mid(3,2).toInt(&ok, 16) , this->values[indexElement].mid(5,2).toInt(&ok, 16)));
            }

        }
    }

    return QColor(0xFF,0xFF,0xFF);

}

int ConfigurationVector::getNumberElements() const
{
    return (this->numberElements);
}

QString ConfigurationVector::getKey(int _index) const
{
    return (this->keys[_index]);
}

QString ConfigurationVector::getValue(QString _key) const
{

    QString value;

    value = "";

    for (int indexElement = 0; indexElement < this->numberElements; indexElement++)
    {
        if (this->keys[indexElement] == _key)
        {
            value = this->values[indexElement];
            break;
        }
    }

    return value;

}

QString ConfigurationVector::getValueNoType(QString _key) const
{

    QString value;

    value = "";

    for (int indexElement = 0; indexElement < this->numberElements; indexElement++)
    {
        if (this->keys[indexElement] == _key)
        {
            value = this->values[indexElement].right((this->values[indexElement].length() - 1));
            break;
        }
    }

    return value;

}

int ConfigurationVector::getType(QString _key) const
{

    QString type;
    int typeValue;

    type = "";

    for (int indexElement = 0; indexElement < this->numberElements; indexElement++)
    {
        if (this->keys[indexElement] == _key)
        {
            type = this->values[indexElement].left(1);
            break;
        }
    }

    typeValue = Property::P_NOTYPE;

    if (type == "I")
    {
        typeValue = Property::P_INT;
    }
    else if (type == "F")
    {
        typeValue = Property::P_FLOAT;
    }
    else if (type == "C")
    {
        typeValue = Property::P_COLOR;
    }

    return typeValue;

}

/***********************************************************
* Load from properties                                     *
***********************************************************/

ConfigurationVector ConfigurationVector::loadFromProperties(Property* _property)
{

    if (_property->isALeaf())
    {

        switch (_property->getType())
        {
        case Property::P_COLOR:
            this->addElement(_property->getKey(), _property->getColor());
            break;
        case Property::P_FLOAT:
        case Property::P_FLOAT_POS:
            this->addElement(_property->getKey(), _property->getFloat());
            break;
        case Property::P_INT:
        case Property::P_INT_POS:
            this->addElement(_property->getKey(), _property->getInt());
            break;
        }

    }
    else
    {

        for (int indexChild = 0; indexChild < _property->getNumberChildren(); indexChild++)
        {
            this->loadFromProperties(_property->getChild(indexChild));
        }

    }

    return (*this);

}


/***********************************************************
* Generate output                                          *
***********************************************************/

QString ConfigurationVector::generateOutput()
{

    QString rtnString;

    rtnString = "";

    for (int indexElement = 0; indexElement < this->numberElements; indexElement++)
    {
        rtnString += this->keys[indexElement];
        rtnString += "=";
        rtnString += this->values[indexElement];
        rtnString += ";\n";
    }

    return rtnString;

}

/***********************************************************
* Read input                                               *
***********************************************************/

void ConfigurationVector::readInput(QString _inputString)
{

    QString currentLine;
    QString key;
    QString value;

    int indexChar = 0;
    int newIndexChar = 0;

    while (_inputString.indexOf(";",indexChar) != -1)
    {

        newIndexChar = _inputString.indexOf(";",indexChar);
        currentLine = _inputString.mid(indexChar, newIndexChar - indexChar);

        key = currentLine.left(currentLine.indexOf("="));
        value = currentLine.right(currentLine.size() - currentLine.indexOf("=") - 1);

        this->addElement(key,value);

        indexChar = newIndexChar + 1;

    }

}

/***********************************************************
* Save to file                                             *
***********************************************************/

void ConfigurationVector::saveToFile(QString _filePath)
{

    QString outputStr;

    QFile outputFile(_filePath);

    outputStr = this->generateOutput();

    outputFile.open(QFile::WriteOnly | QFile::Text);

    QTextStream outputStream(&outputFile);

    outputStream << outputStr;

    outputFile.close();

}

/***********************************************************
* Open from file                                           *
***********************************************************/

void ConfigurationVector::openFromFile(QString _filePath)
{

    QString inputStr;
    QString currentLine;

    QFile inputFile(_filePath);

    inputFile.open(QFile::ReadOnly | QFile::Text);

    QTextStream inputStream(&inputFile);

    inputStr = "";   

    while(!inputStream.atEnd())
    {
        currentLine = inputStream.readLine();
        inputStr.append(currentLine);
    }

    this->reset();
    this->readInput(inputStr);

    inputFile.close();

}
