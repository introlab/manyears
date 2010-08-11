#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <QWidget>
#include <QScrollBar>
#include <QPixmap>
#include <QString>
#include <QPainter>
#include <QInputDialog>
#include <QColorDialog>
#include <QMutex>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QCoreApplication>

#include "property.h"
#include "parametersManager.h"

class PropertyEditor: public QWidget
{

    Q_OBJECT

public:

    /***********************************************************
    * Constants                                                *
    ***********************************************************/

    static const int VIEW = 1;
    static const int SYSTEM = 2;

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    PropertyEditor(const int _width, const int _height, const int _scrollBarWidth, Property _property, const int _typeProperty);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~PropertyEditor();

    /***********************************************************
    * Load values in properties                                *
    ***********************************************************/

    void loadValues(const ConfigurationVector _configurationVector);

    /***********************************************************
    * Export values of properties                              *
    ***********************************************************/

    ConfigurationVector exportValues();

    /***********************************************************
    * Events receiver                                          *
    ***********************************************************/

    void setEventReceiver(QObject* _eventReceiver);

protected:

    /***********************************************************
    * Events                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Resize occurs                                         |
    // +-------------------------------------------------------+

    void resizeEvent(QResizeEvent *event);

    // +-------------------------------------------------------+
    // | Single click occurs                                   |
    // +-------------------------------------------------------+

    void mousePressEvent(QMouseEvent *event);

    // +-------------------------------------------------------+
    // | Double click occurs                                   |
    // +-------------------------------------------------------+

    void mouseDoubleClickEvent(QMouseEvent *event);

    // +-------------------------------------------------------+
    // | General events                                        |
    // +-------------------------------------------------------+
    bool event ( QEvent * event ) ;

    // +-------------------------------------------------------+
    // | Paint                                                 |
    // +-------------------------------------------------------+

    void paintEvent( QPaintEvent * );

private slots:

    /***********************************************************
    * Slots                                                    *
    ***********************************************************/

    void scrollBarValueChanged(int _newValue);

private:

    /***********************************************************
    * Dimensions adjustements                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Generate pixmaps with good dimensions                 |
    // +-------------------------------------------------------+

    void generatePixmaps();

    // +-------------------------------------------------------+
    // | Modify scrollbar range and size                       |
    // +-------------------------------------------------------+

    void modifyScrollBarRangeAndSize();

    // +-------------------------------------------------------+
    // | Confirm valid click                                   |
    // +-------------------------------------------------------+

    Property* confirmValidClick(const int _x, const int _y);

    /***********************************************************
    * Draw                                                     *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | + Sign                                                |
    // +-------------------------------------------------------+

    void drawPlus(QColor _color, int _left, int _top);

    // +-------------------------------------------------------+
    // | - Sign                                                |
    // +-------------------------------------------------------+

    void drawMinus(QColor _color, int _left, int _top);

    // +-------------------------------------------------------+
    // | Separator light                                       |
    // +-------------------------------------------------------+

    void drawSeparatorLight(int _top);

    // +-------------------------------------------------------+
    // | Separator dark                                        |
    // +-------------------------------------------------------+

    void drawSeparatorDark(int _top);

    // +-------------------------------------------------------+
    // | Property                                              |
    // +-------------------------------------------------------+

    void drawProperty(Property *_property, int _prevIndentation);

    // +-------------------------------------------------------+
    // | All                                                   |
    // +-------------------------------------------------------+

    void drawAll();

    // +-------------------------------------------------------+
    // | Get property                                          |
    // +-------------------------------------------------------+

    Property* getProperty(Property* _property, int _mousePosition);
    Property* getPropertyRecursive(Property* _property, int _mousePosition);

    // +-------------------------------------------------------+
    // | Display text box                                      |
    // +-------------------------------------------------------+

    QString displayInputBox(QString _key);

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Widgets                                               |
    // +-------------------------------------------------------+

    QScrollBar *scrollBar;
    QPixmap *pixelMap;

    // +-------------------------------------------------------+
    // | Properties                                            |
    // +-------------------------------------------------------+

    Property* rootProperty;
    QString selectedPropertyKey;
    int typeProperty;

    // +-------------------------------------------------------+
    // | Dimensions                                            |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Overall                                           |
        // +---------------------------------------------------+

        int width;
        int height;

        // +---------------------------------------------------+
        // | List                                              |
        // +---------------------------------------------------+

        int listWidth;
        int listMaxHeight;
        int listCurrentHeight;

        // +---------------------------------------------------+
        // | Scrollbar                                         |
        // +---------------------------------------------------+

        int scrollBarWidth;

        // +---------------------------------------------------+
        // | Elements to be drawn                              |
        // +---------------------------------------------------+

        int squareSize;
        int indentationSize;
        int iconSize;
        int heightElement;
        int iconLeft;
        int iconRight;
        int widthLabel;

    // +-------------------------------------------------------+
    // | Flags                                                 |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Keep in memory the top of the last elements drawn |
        // +---------------------------------------------------+

        int topElement;

        // +---------------------------------------------------+
        // | Last recorder scrollbar value                     |
        // +---------------------------------------------------+

        int scrollBarValue;

    // +-------------------------------------------------------+
    // | Events receiver                                       |
    // +-------------------------------------------------------+

        QObject* eventReceiver;

};

#endif
