#include "spherePlot.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

SpherePlot::SpherePlot(int _numberLevelsBig, int _numberLevelsSmall, float _radiusBig, float _radiusSmall, SourceManager *_sourceManager, int _width, int _height, QMutex *_mutex)
{

    // Create the spheres
    this->bigSphere = new Sphere(_numberLevelsBig, _radiusBig);
    this->smallSphere = new Sphere(_numberLevelsSmall, _radiusSmall);

    // Save pointer to the source manager
    this->currentSourceManager = _sourceManager;

    // Connect
    this->colorsToDraw = &this->currentSourceManager->connectColors;
    this->xPosition = &this->currentSourceManager->connectXPosition;
    this->yPosition = &this->currentSourceManager->connectYPosition;
    this->zPosition = &this->currentSourceManager->connectZPosition;

    this->rotationX = -60.0;
    this->rotationY = 0.0;
    this->rotationZ = -45.0;

    // Initialize
    this->setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    // Zoom
    this->zoom = -6.0;

    // Mouse cursor
    this->setCursor(Qt::OpenHandCursor);

    // Resize
    this->resizeGL(_width, _height);

    // Save the mutex
    this->blockRendering = _mutex;

}

/***********************************************************
* Destructor                                               *
***********************************************************/

SpherePlot::~SpherePlot()
{

    delete this->bigSphere;
    delete this->smallSphere;

}


/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Set the background color                              |
// +-------------------------------------------------------+

void SpherePlot::setBackgroundColor(QColor _color)
{
    this->backgroundColor = _color;
}

// +-------------------------------------------------------+
// | Set the axes color                                    |
// +-------------------------------------------------------+

void SpherePlot::setAxesColor(QColor _color)
{
    this->axesColor = _color;
}

// +-------------------------------------------------------+
// | Set the top grid color                                |
// +-------------------------------------------------------+

void SpherePlot::setTopGridColor(QColor _color)
{
    this->topGridColor = _color;
}

// +-------------------------------------------------------+
// | Set the bottom grid color                             |
// +-------------------------------------------------------+

void SpherePlot::setBottomGridColor(QColor _color)
{
    this->bottomGridColor = _color;
}

/***********************************************************
* Refresh                                                  *
***********************************************************/

void SpherePlot::updateGraph()
{
    this->update();
}


/***********************************************************
* OpenGL                                                   *
***********************************************************/

// +-------------------------------------------------------+
// | Draw the spheres                                      |
// +-------------------------------------------------------+

void SpherePlot::paintGL()
{

    // This is a critical section
    // Protect with a mutex
    this->blockRendering->lock();

    GLfloat p1x, p1y, p1z;
    GLfloat p2x, p2y, p2z;
    GLfloat p3x, p3y, p3z;

    GLfloat red, green, blue;
    QColor sphereColor;

    GLfloat centerX, centerY, centerZ;

    // Enable depth test, counter-clockwise face
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Clear the previous buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Resize if required
    if (this->resizeDone == true)
    {
        glViewport(0,0,this->width,this->height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLfloat x = GLfloat(this->width) / GLfloat(this->height);
        glFrustum(-1.0*x, x, -1.0, 1.0, 4.0, 50.0);
        glMatrixMode(GL_MODELVIEW);

        // Remove flag
        this->resizeDone = false;
    }

    // Set up view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0,0.0,this->zoom);
    glRotatef(this->rotationX,1.0,0.0,0.0);
    glRotatef(this->rotationY,0.0,1.0,0.0);
    glRotatef(this->rotationZ,0.0,0.0,1.0);
    glLightModelf(GL_LIGHT_MODEL_AMBIENT, 1.5);

    // Create background
    glClearColor(this->backgroundColor.red()/255.0, this->backgroundColor.green()/255.0, this->backgroundColor.blue()/255.0, 1.0);

    // Draw the big sphere
    for (int indexTriangle = 0; indexTriangle < this->bigSphere->getNumberTriangles(); indexTriangle++)
    {

        glBegin(GL_LINE_LOOP);

        // Load vertices
        this->bigSphere->getTriangleVertices(indexTriangle, &p1x, &p1y, &p1z, &p2x, &p2y, &p2z, &p3x, &p3y, &p3z);

        // Setup appearance
        if (((p1z + p2z + p3z) / 3) > 0)
        {
            glColor4f(this->topGridColor.red()/255.0,this->topGridColor.green()/255.0,this->topGridColor.blue()/255.0,1.0);
        }
        else
        {
            glColor4f(this->bottomGridColor.red()/255.0,this->bottomGridColor.green()/255.0,this->bottomGridColor.blue()/255.0,1.0);
        }

        // Draw
        glVertex3f(p1x,p1y,p1z);
        glVertex3f(p2x,p2y,p2z);
        glVertex3f(p3x,p3y,p3z);

        glEnd();

    }



    // Draw the axis
    glLoadIdentity();
    glTranslatef(0.0,0.0,this->zoom);
    glRotatef(this->rotationX,1.0,0.0,0.0);
    glRotatef(this->rotationY,0.0,1.0,0.0);
    glRotatef(this->rotationZ,0.0,0.0,1.0);
    glTranslatef(-1 * this->bigSphere->getRadius(), -1 * this->bigSphere->getRadius(), -1 * this->bigSphere->getRadius());
    glColor4f(this->axesColor.red()/255.0,this->axesColor.green()/255.0,this->axesColor.blue()/255.0,1.0);

    glBegin(GL_LINES);

    // X-axis
    glVertex3f( 0.0 , 0.0 , 0.0 );
    glVertex3f( 1.0 , 0.0 , 0.0 );
    glVertex3f( 1.0 , 0.0 , 0.0 );
    glVertex3f( 0.9 , 0.05 , 0.0 );
    glVertex3f( 1.0 , 0.0 , 0.0 );
    glVertex3f( 0.9 , -0.05 , 0.0 );

    // Y-axis
    glVertex3f( 0.0 , 0.0 , 0.0 );
    glVertex3f( 0.0 , 1.0 , 0.0 );
    glVertex3f( 0.0 , 1.0 , 0.0 );
    glVertex3f( 0.05 , 0.9 , 0.0 );
    glVertex3f( 0.0 , 1.0 , 0.0 );
    glVertex3f( -0.05 , 0.9 , 0.0 );

    // Z-axis
    glVertex3f( 0.0 , 0.0 , 0.0 );
    glVertex3f( 0.0 , 0.0 , 1.0 );
    glVertex3f( 0.0 , 0.0 , 1.0 );
    glVertex3f( 0.05 , -0.05 , 0.9 );
    glVertex3f( 0.0 , 0.0 , 1.0 );
    glVertex3f( -0.05 , 0.05 , 0.9 );

    glEnd();


    // Draw the small spheres
    for (int indexSphere = 0; indexSphere < this->currentSourceManager->numberSources; indexSphere++)
    {

        // Get position of the source
        centerX = this->xPosition->at(indexSphere);
        centerY = this->yPosition->at(indexSphere);
        centerZ = this->zPosition->at(indexSphere);

        // Move to position
        glLoadIdentity();
        glTranslatef(0.0,0.0,this->zoom);
        glRotatef(this->rotationX,1.0,0.0,0.0);
        glRotatef(this->rotationY,0.0,1.0,0.0);
        glRotatef(this->rotationZ,0.0,0.0,1.0);
        glTranslatef(centerX, centerY, centerZ);

        // Draw each sphere
        for (int indexTriangle = 0; indexTriangle < this->smallSphere->getNumberTriangles(); indexTriangle++)
        {

            glBegin(GL_TRIANGLES);

            // Load vertices
            this->smallSphere->getTriangleVertices(indexTriangle, &p1x, &p1y, &p1z, &p2x, &p2y, &p2z, &p3x, &p3y, &p3z);

            // Get color of the source

            sphereColor = this->colorsToDraw->at(indexSphere);
            red = ((GLfloat) sphereColor.red()) / ((GLfloat) 255.0);
            green = ((GLfloat) sphereColor.green()) / ((GLfloat) 255.0);
            blue = ((GLfloat) sphereColor.blue()) / ((GLfloat) 255.0);

            // Setup appearance
            glColor4f(red,green,blue,0.1);

            // Draw
            glVertex3f(p1x,p1y,p1z);
            glVertex3f(p2x,p2y,p2z);
            glVertex3f(p3x,p3y,p3z);

            glEnd();

        }

    }

    // Free
    this->blockRendering->unlock();

}

// +-------------------------------------------------------+
// | Resize the widget                                     |
// +-------------------------------------------------------+

void SpherePlot::resizeGL(int _width, int _height)
{
    this->width = _width;
    this->height = _height;
    this->resizeDone = true;
}

/***********************************************************
* User interaction                                         *
***********************************************************/

// +-------------------------------------------------------+
// | Mouse is pressed                                      |
// +-------------------------------------------------------+

void SpherePlot::mousePressEvent(QMouseEvent *event)
{
    this->cursorLastPosition = event->pos();
    this->setCursor(Qt::ClosedHandCursor);
}

// +-------------------------------------------------------+
// | Mouse is released                                     |
// +-------------------------------------------------------+

void SpherePlot::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(Qt::OpenHandCursor);
}

// +-------------------------------------------------------+
// | Mouse is moved                                        |
// +-------------------------------------------------------+

void SpherePlot::mouseMoveEvent(QMouseEvent *event)
{
    float dx;
    float dy;

    if (event->buttons() & Qt::LeftButton)
    {

        // Compute the difference since last move
        dx = this->cursorLastPosition.x() - event->pos().x();
        dy = this->cursorLastPosition.y() - event->pos().y();

        // Increment the angle wrt the y-axis
        this->rotationZ -= dx;
        if (this->rotationZ > 360)
        {
            this->rotationZ -= 360;
        }

        // Increment the angle wrt the x-axis
        this->rotationX -= dy;
        if (this->rotationX > 360)
        {
            this->rotationX -= 360;
        }

        // Refresh the position
        this->cursorLastPosition = event->pos();

    }

}

// +-------------------------------------------------------+
// | Wheel is turned                                       |
// +-------------------------------------------------------+

void SpherePlot::wheelEvent(QWheelEvent *event)
{

    this->zoom += (event->delta() / 1000.0);

}
