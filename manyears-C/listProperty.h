#ifndef LISTPROPERTY_H
#define LISTPROPERTY_H

#include <QWidget>
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QInputDialog>
#include <QColorDialog>
#include <QResizeEvent>

#include "property.h"
#include "configurationVector.h"

class ListProperty: public QWidget
{

    Q_OBJECT

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    ListProperty(int _width, int _height);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~ListProperty();

    /***********************************************************
    * Load properties                                          *
    ***********************************************************/

    void loadParameters(Property *_rootProperty);

    /***********************************************************
    * Modify properties                                        *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Read property                                         |
    // +-------------------------------------------------------+

        float getPropertyFloat(QString _key);
        QString getPropertyFloatString(QString _key);
        int getPropertyInt(QString _key);
        QString getPropertyIntString(QString _key);
        QColor getPropertyColor(QString _key);
        QString getPropertyColorString(QString _key);
        QString getPropertyValueString(QString _key);

    // +-------------------------------------------------------+
    // | Write property                                        |
    // +-------------------------------------------------------+

        void setPropertyFloat(QString _key, float _value);
        void setPropertyInt(QString _key, int _value);
        void setPropertyColor(QString _key, QColor _value);
        void setPropertyValueString(QString _key, QString _value);

    /***********************************************************
    * Generate a configuration vector                          *
    ***********************************************************/

    void generateVector(ConfigurationVector *_currentVector);
    void generateVectorRecursive(Property *_currentProperty, ConfigurationVector *_currentVector);

    /***********************************************************
    * Do not udpate the graph                                  *
    ***********************************************************/

    void freezeScrollBarUpdate();

    /***********************************************************
    * Allow udpating the graph                                 *
    ***********************************************************/

    void unfreezeScrollBarUpdate();

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Display Height                                        |
    // +-------------------------------------------------------+

    int getDisplayHeight();

    // +-------------------------------------------------------+
    // | Height of elements                                    |
    // +-------------------------------------------------------+

    int getHeightElement();

    /***********************************************************
    * Reset display top                                        *
    ***********************************************************/

    void resetDisplayTop();

signals:

    /***********************************************************
    * The widget is resized                                    *
    ***********************************************************/

    void isResized();

    /***********************************************************
    * A value is changed in the list                           *
    ***********************************************************/

    void valueChanged(int _newValue);

public slots:

    /***********************************************************
    * Change the position of the top of the list               *
    ***********************************************************/

    void scrollBarValueChanged(int _newValue);

protected:

    /***********************************************************
    * Protected events                                         *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Paint                                                 |
    // +-------------------------------------------------------+

    void paintEvent( QPaintEvent * );

    // +-------------------------------------------------------+
    // | Mouse is pressed                                      |
    // +-------------------------------------------------------+

    void mousePressEvent(QMouseEvent *event);

    // +-------------------------------------------------------+
    // | Mouse is double-clicked                               |
    // +-------------------------------------------------------+

    void mouseDoubleClickEvent(QMouseEvent *event);

    // +-------------------------------------------------------+
    // | Resize occurs                                         |
    // +-------------------------------------------------------+

    void resizeEvent(QResizeEvent *event);

private:

    /***********************************************************
    * Add a child                                              *
    ***********************************************************/

    void addChild(Property *_child);

    /***********************************************************
    * Draw                                                     *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Draw all elements                                     |
    // +-------------------------------------------------------+

    void drawAllElements(QString _keySelected);

    // +-------------------------------------------------------+
    // | + Sign                                                |
    // +-------------------------------------------------------+

    void drawPlus(QColor _color, int _left, int _top);

    // +-------------------------------------------------------+
    // | - Sign                                                |
    // +-------------------------------------------------------+

    void drawMinus(QColor _color, int _left, int _top);

    // +-------------------------------------------------------+
    // | Top most category                                     |
    // +-------------------------------------------------------+

    void drawTopCategory(int _top, Property *_property);

    // +-------------------------------------------------------+
    // | Category                                              |
    // +-------------------------------------------------------+

    void drawCategory(int _top, Property *_property, int _prevIndentation);

    // +-------------------------------------------------------+
    // | Separator light                                       |
    // +-------------------------------------------------------+

    void drawSeparatorLight(int _top);

    // +-------------------------------------------------------+
    // | Separator dark                                        |
    // +-------------------------------------------------------+

    void drawSeparatorDark(int _top);

    // +-------------------------------------------------------+
    // | Border                                                |
    // +-------------------------------------------------------+

    void drawBorder();

    // +-------------------------------------------------------+
    // | All children                                          |
    // +-------------------------------------------------------+

    int drawChildren(Property *_parent, int _indentation, int _top, QString _keySelected);

    // +-------------------------------------------------------+
    // | Get key                                               |
    // +-------------------------------------------------------+

    QString getKey(int _mousePosition);

    // +-------------------------------------------------------+
    // | Get property                                          |
    // +-------------------------------------------------------+

    Property *getProperty(QString _key);

    // +-------------------------------------------------------+
    // | Refresh selection                                     |
    // +-------------------------------------------------------+

    void refreshSelection(int _mousePosition);

    // +-------------------------------------------------------+
    // | Display text box                                      |
    // +-------------------------------------------------------+

    bool displayInputBox(int _mousePosition);

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Semaphore                                             |
    // +-------------------------------------------------------+

    QMutex isBeingUsed;
    QMutex mutexFreezeSignal;
    QMutex mutexDimension;

    // +-------------------------------------------------------+
    // | Dimensions                                            |
    // +-------------------------------------------------------+

    int width;
    int height;
    int displayHeight;
    int displayTop;

    // +-------------------------------------------------------+
    // | Elements                                              |
    // +-------------------------------------------------------+

    int squareSize;
    int indentationSize;
    int iconSize;
    int heightElement;
    int iconLeft;
    int iconRight;
    int widthLabel;

    // +-------------------------------------------------------+
    // | Links                                                 |
    // +-------------------------------------------------------+

    QVector<Property*> listDown;
    QVector<QString> keys;
    QVector<int> startMouse;
    QVector<int> endMouse;
    int numberChildren;
    int numberDisplay;

    // +-------------------------------------------------------+
    // | Graph                                                 |
    // +-------------------------------------------------------+

    QPixmap *pixelMap;
    bool freezeSignal;

};

#endif // LISTPROPERTY_H
