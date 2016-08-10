#ifndef SPHEREPLOT_H
#define SPHEREPLOT_H

#include <QtOpenGL>
#include <QVector>
#include <QColor>
#include <QMutex>

#include "sphereObj.h"
#include "sourceManager.h"

class SpherePlot: public QGLWidget
{

    Q_OBJECT

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    SpherePlot(int _numberLevelsBig, int _numberLevelsSmall, float _radiusBig, float _radiusSmall, SourceManager *_sourceManager, int _width, int _height, QMutex *_mutex);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~SpherePlot();

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Set the background color                              |
    // +-------------------------------------------------------+

    void setBackgroundColor(QColor _color);

    // +-------------------------------------------------------+
    // | Set the axes color                                    |
    // +-------------------------------------------------------+

    void setAxesColor(QColor _color);

    // +-------------------------------------------------------+
    // | Set the top grid color                                |
    // +-------------------------------------------------------+

    void setTopGridColor(QColor _color);

    // +-------------------------------------------------------+
    // | Set the bottom grid color                             |
    // +-------------------------------------------------------+

    void setBottomGridColor(QColor _color);

    /***********************************************************
    * Refresh                                                  *
    ***********************************************************/

    void updateGraph();

protected:

    /***********************************************************
    * OpenGL                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Draw the spheres                                      |
    // +-------------------------------------------------------+

    void paintGL();

    // +-------------------------------------------------------+
    // | Resize the widget                                     |
    // +-------------------------------------------------------+

    void resizeGL(int _width, int _height);


    /***********************************************************
    * User interaction                                         *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Mouse is pressed                                      |
    // +-------------------------------------------------------+

    void mousePressEvent(QMouseEvent *event);

    // +-------------------------------------------------------+
    // | Mouse is released                                     |
    // +-------------------------------------------------------+

    void mouseReleaseEvent(QMouseEvent *event);

    // +-------------------------------------------------------+
    // | Mouse is moved                                        |
    // +-------------------------------------------------------+

    void mouseMoveEvent(QMouseEvent *event);

    // +-------------------------------------------------------+
    // | Wheel is turned                                       |
    // +-------------------------------------------------------+

    void wheelEvent(QWheelEvent *event);

private:

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Objects                                               |
    // +-------------------------------------------------------+

    SourceManager *currentSourceManager;

    // +-------------------------------------------------------+
    // | Appearance                                            |
    // +-------------------------------------------------------+

    // Background

    QColor backgroundColor;

    // Axes

    QColor axesColor;

    // Big sphere

    Sphere *bigSphere;
    QColor topGridColor;
    QColor bottomGridColor;

    // Small sphere

    Sphere *smallSphere;

    // Rotation parameters

    float rotationX;
    float rotationY;
    float rotationZ;
    QPoint cursorLastPosition;
    float zoom;

    // Pointer to the vectors of position of sources

    QVector<float> *xPosition;
    QVector<float> *yPosition;
    QVector<float> *zPosition;

    // Pointer to the vector of colors for the drawn values

    QVector<QColor> *colorsToDraw;

    // Flag to indicate resize has been performed
    bool resizeDone;

    // Dimensions
    int width;
    int height;

    // +-------------------------------------------------------+
    // | Mutex                                                 |
    // +-------------------------------------------------------+

    // Lock the plot for rendering
    QMutex *blockRendering;

};

#endif // SPHEREPLOT_H
