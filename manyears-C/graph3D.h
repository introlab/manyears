#ifndef GRAPH3D_H
#define GRAPH3D_H

#include <QtOpenGL>
#include <QVector>
#include <QColor>

#include "sphereObj.h"
#include "sourceBuffer.h"

#include "sphere.h"

class Graph3D: public QGLWidget
{

    Q_OBJECT

public:

    static const char TYPE_TRACKED = 1;
    static const char TYPE_ENERGY = 2;

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    Graph3D(int _numberLevelsBig, int _numberLevelsSmall, float _radiusBig, float _radiusSmall);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~Graph3D();

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Load triangles                                        |
    // +-------------------------------------------------------+

    void loadTriangles(struct objSphereTriangle _sphereTriangles, struct objSphere _spherePoints);

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

    void pushFrameReceiver(QObject* _receiver);

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
    // | Resize the widget (OpenGL)                            |
    // +-------------------------------------------------------+

    void resizeGL(int w, int h);

    /***********************************************************
    * User interaction                                         *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | User events                                           |
    // +-------------------------------------------------------+

    bool event(QEvent *event);

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

    QColor getColorEnergy(float _energy);

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

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

    // Energy sphere
    float energySphereRadius;

    // Rotation parameters

    float rotationX;
    float rotationY;
    float rotationZ;
    QPoint cursorLastPosition;
    float zoom;

    // Dimensions
    int width;
    int height;

    bool resizeDone;

    QList<SourceDisplayed> trackedSources;

    int updateCounter;

    char typeInput;

};



#endif // GRAPH3D_H
