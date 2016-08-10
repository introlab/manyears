#include <vector>

#include "colorID.h"
#include "colorPalette.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

ColorPalette::ColorPalette()
{

    this->numberColors = 0;
    this->listColors.clear();
    this->stackColors.clear();

    this->staticPalette = false;

}

ColorPalette::ColorPalette(QVector<QColor> _initialColors)
{

    this->numberColors = 0;

    ColorID tmp;

    this->listColors.clear();
    this->stackColors.clear();

    for (int indexColor = (_initialColors.size() - 1); indexColor >= 0; indexColor--)
    {

        tmp.setID(indexColor + 1);
        tmp.setColor(_initialColors[indexColor].red(), _initialColors[indexColor].green(), _initialColors[indexColor].blue());

        this->listColors.push_back(tmp);
        this->stackColors.push_back(tmp);
        this->numberColors++;

    }

    this->stackSize = this->numberColors;
    this->staticPalette = true;

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Accessor: Color                                       |
// +-------------------------------------------------------+

const QColor ColorPalette::getColor(const int _colorID)
{

    int indexColor;

    // If the ID is 0, then return default color
    if (_colorID == 0)
    {
        return QColor(0x00,0x00,0x00);
    }
    else
    {

        // Find this color
        for (indexColor = 0; indexColor < this->numberColors; indexColor++)
        {

            if (this->listColors[indexColor].getID() == _colorID)
            {

                break;

            }

        }

        // Return the color
        return listColors[indexColor].getColor();
    }

}

// +-------------------------------------------------------+
// | Accessor: Color object                                |
// +-------------------------------------------------------+

const ColorID ColorPalette::findColor(const int _colorID)
{

    int indexColor;

    // If the ID is 0, then return default color
    if (_colorID == 0)
    {
        return ColorID(0,this->defaultColor.red(),this->defaultColor.green(),this->defaultColor.blue());
    }
    else
    {

        // Find this color
        for (indexColor = 0; indexColor < this->numberColors; indexColor++)
        {

            if (this->listColors[indexColor].getID() == _colorID)
            {

                break;

            }

        }

        // Return the color
        return listColors[indexColor];
    }

}

/***********************************************************
* Other functions                                          *
***********************************************************/

// +-------------------------------------------------------+
// | Define the default color                              |
// +-------------------------------------------------------+

void ColorPalette::defineDefaultColor(const int _red, const int _green, const int _blue)
{

    // Only if the palette is not in use
    if (this->inUse == false)
    {

        this->defaultColor = QColor(_red,_green,_blue);

    }

}

void ColorPalette::defineDefaultColor(const QColor _color)
{

    // Only if the palette is not in use
    if (this->inUse == false)
    {

        this->defaultColor = _color;

    }

}

// +-------------------------------------------------------+
// | Add a color                                           |
// +-------------------------------------------------------+

void ColorPalette::addColor(const int _red, const int _green, const int _blue)
{

    ColorID tmp;

    // Only for a dynamic palette
    if (this->staticPalette == false)
    {

        // Only if the palette is not in use
        if (this->inUse == false)
        {

            tmp.setID(0);
            tmp.setColor(_red,_green,_blue);

            this->listColors.push_back(tmp);

            this->numberColors++;

        }

    }
}

void ColorPalette::addColor(const QColor _color)
{

    this->addColor(_color.red(),_color.green(),_color.blue());

}

// +-------------------------------------------------------+
// | Remove a color                                        |
// +-------------------------------------------------------+

const bool ColorPalette::removeColor(const int _colorIndex)
{

    // Only for a dynamic palette
    if (this->staticPalette == false)
    {

        // Only if the palette is not in use
        if (this->inUse == false)
        {

            if (_colorIndex < this->numberColors)
            {

                this->listColors.erase(listColors.begin() + _colorIndex);
                this->numberColors--;
                return true;

            }

        }

    }

    return false;

}

// +-------------------------------------------------------+
// | Remove all colors                                     |
// +-------------------------------------------------------+

const bool ColorPalette::removeAllColor()
{

    // Only for a dynamic palette
    if (this->staticPalette == false)
    {

        // Only if the palette is not in use
        if (this->inUse == false)
        {

            this->listColors.clear();
            this->numberColors = 0;
            return true;

        }

    }

    return false;

}

// +-------------------------------------------------------+
// | Replace a color by another                            |
// +-------------------------------------------------------+

const bool ColorPalette::replaceColor(const int _colorID, QColor _newColor)
{

    // Find this color
    for (int indexColor = 0; indexColor < this->numberColors; indexColor++)
    {

        if (this->listColors[indexColor].getID() == _colorID)
        {

            this->listColors[indexColor].setColor(_newColor.red(), _newColor.green(), _newColor.blue());
            return true;

        }

    }

    return false;

}

// +-------------------------------------------------------+
// | Start using the palette                               |
// +-------------------------------------------------------+

void ColorPalette::start()
{

    int newID;

    // The palette is now in use and cannot be modified
    // (except for replacing a color by another)
    this->inUse = true;

    // Clear the stack
    this->stackColors.clear();

    // Take required memory for the stack
    this->stackColors.reserve(this->numberColors);

    newID = 0;

    // Load all colors in the stack
    for (int indexColor = (this->numberColors - 1); indexColor >= 0; indexColor--)
    {

        newID++;
        this->listColors[indexColor].setID(newID);
        this->stackColors.push_back(this->listColors[indexColor]);

    }

    // Save the stack size
    this->stackSize = this->numberColors;

}

// +-------------------------------------------------------+
// | Stop using the palette                                |
// +-------------------------------------------------------+

void ColorPalette::stop()
{

    // The palette is no longer used and can therefore
    // be modified
    this->inUse = false;

    // Clear the stack
    this->stackColors.clear();

}

// +-------------------------------------------------------+
// | Use a new color                                       |
// +-------------------------------------------------------+

const int ColorPalette::useColor()
{

    ColorID tmp;

    // If there is at least one color left in the stack
    if (this->stackSize > 0)
    {

        // Take this color
        tmp = this->stackColors.back();

        // Remove it from the stack
        this->stackColors.pop_back();

        // Decrease the number of colors in the stack
        this->stackSize--;

        return tmp.getID();

    }

    return 0;


}

// +-------------------------------------------------------+
// | Stop using a color                                    |
// +-------------------------------------------------------+

void ColorPalette::freeColor(const int _colorID)
{

    bool alreadyFree;

    alreadyFree = false;

    // Check if this color is already in the stack
    for (int indexColor = 0; indexColor < this->stackSize; indexColor++)
    {

        if (this->stackColors[indexColor].getID() == _colorID)
        {
            alreadyFree = true;
            break;
        }

    }

    // If not, add it back
    if (alreadyFree == false)
    {
        // Inser the color at the end of the stack
        this->stackColors.insert(this->stackColors.begin(),this->findColor(_colorID));

        // Increment the number of colors
        this->stackSize++;
    }

}
