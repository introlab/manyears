#include "graph3D.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

Graph3D::Graph3D(int _numberLevelsBig, int _numberLevelsSmall, float _radiusBig, float _radiusSmall)
{

    // Create the spheres
    this->bigSphere = new Sphere(_numberLevelsBig, _radiusBig);
    this->smallSphere = new Sphere(_numberLevelsSmall, _radiusSmall);

    // Default rotation
    this->rotationX = -60.0;
    this->rotationY = 0.0;
    this->rotationZ = -45.0;

    // Default color
    this->backgroundColor = QColor(0x00,0x00,0x00);
    this->axesColor = QColor(0xFF,0xFF,0xFF);
    this->topGridColor = QColor(0xFF,0xFF,0xFF);
    this->bottomGridColor = QColor(0xFF,0xFF,0xFF);

    // Initialize
    this->setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    // Zoom
    this->zoom = -6.0;

    // Mouse cursor
    this->setCursor(Qt::OpenHandCursor);

    // Reset counter
    this->updateCounter = 0;

    // Set type
    this->typeInput = Graph3D::TYPE_TRACKED;

    // Resize
    this->resizeGL(this->geometry().width(), this->geometry().height());

}

/***********************************************************
* Destructor                                               *
***********************************************************/

Graph3D::~Graph3D()
{

    if (this->typeInput == Graph3D::TYPE_TRACKED)
    {

        delete this->bigSphere;
        delete this->smallSphere;
    }

}


/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Set the background color                              |
// +-------------------------------------------------------+

void Graph3D::setBackgroundColor(QColor _color)
{
    this->backgroundColor = _color;
}

// +-------------------------------------------------------+
// | Set the axes color                                    |
// +-------------------------------------------------------+

void Graph3D::setAxesColor(QColor _color)
{
    this->axesColor = _color;
}

// +-------------------------------------------------------+
// | Set the top grid color                                |
// +-------------------------------------------------------+

void Graph3D::setTopGridColor(QColor _color)
{
    this->topGridColor = _color;
}

// +-------------------------------------------------------+
// | Set the bottom grid color                             |
// +-------------------------------------------------------+

void Graph3D::setBottomGridColor(QColor _color)
{
    this->bottomGridColor = _color;
}

/***********************************************************
* Refresh                                                  *
***********************************************************/

void Graph3D::updateGraph()
{
    this->update();
}


/***********************************************************
* OpenGL                                                   *
***********************************************************/

// +-------------------------------------------------------+
// | Draw the spheres                                      |
// +-------------------------------------------------------+

void Graph3D::paintGL()
{

    GLfloat p1x, p1y, p1z;
    GLfloat p2x, p2y, p2z;
    GLfloat p3x, p3y, p3z;
    GLfloat p1e, p2e, p3e;

    GLfloat maxX, maxY, maxZ;

    QColor p1color, p2color, p3color;

    int p1index, p2index, p3index;

    GLfloat red, green, blue;
    QColor sphereColor;

    GLfloat centerX, centerY, centerZ;

    // Enable depth test, counter-clockwise face

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    // Create background
    glClearColor(this->backgroundColor.red()/255.0, this->backgroundColor.green()/255.0, this->backgroundColor.blue()/255.0, 1.0);

    // Clear the previous buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Resize if required
    if (this->resizeDone == true)
    {
        glViewport(0,0,this->geometry().width(),this->geometry().height());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLfloat x = GLfloat(this->geometry().width()) / GLfloat(this->geometry().height());
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

    // Check type of sphere
    if (this->typeInput == Graph3D::TYPE_TRACKED)
    {

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

    }

    // Check type of sphere
    if (this->typeInput == Graph3D::TYPE_TRACKED)
    {

        // Draw the axis
        glLoadIdentity();
        glTranslatef(0.0,0.0,this->zoom);
        glRotatef(this->rotationX,1.0,0.0,0.0);
        glRotatef(this->rotationY,0.0,1.0,0.0);
        glRotatef(this->rotationZ,0.0,0.0,1.0);
        glTranslatef(-1 * this->bigSphere->getRadius(), -1 * this->bigSphere->getRadius(), -1 * this->bigSphere->getRadius());
        glColor4f(this->axesColor.red()/255.0,this->axesColor.green()/255.0,this->axesColor.blue()/255.0,1.0);

    }


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

    if (this->typeInput == Graph3D::TYPE_TRACKED)
    {

        // Draw the small spheres
        for (int indexSphere = 0; indexSphere < this->trackedSources.size(); indexSphere++)
        {

            // Get position of the source
            centerX = this->trackedSources.at(indexSphere).getX();
            centerY = this->trackedSources.at(indexSphere).getY();
            centerZ = this->trackedSources.at(indexSphere).getZ();

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

                sphereColor = this->trackedSources.at(indexSphere).getColor();
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

    }

}

// +-------------------------------------------------------+
// | Resize the widget                                     |
// +-------------------------------------------------------+

void Graph3D::resizeGL(int w, int h)
{
    this->resizeDone = true;
    this->updateGL();
}

/***********************************************************
* User interaction                                         *
***********************************************************/

// +-------------------------------------------------------+
// | User events                                           |
// +-------------------------------------------------------+

bool Graph3D::event(QEvent *event)
{

    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | Add new tracked sources for one frame         |
        // +-----------------------------------------------+

        UpdateGraphAddSampleEvent *updateGraphAddSampleEvent = dynamic_cast<UpdateGraphAddSampleEvent*>(event);

        if (updateGraphAddSampleEvent != NULL)
        {

            this->trackedSources = updateGraphAddSampleEvent->getAllSources();

            // Do not update every frame because this is too much information
            // to process
            updateCounter++;

            if (updateCounter == 10)
            {

                updateCounter = 0;
                this->updateGL();

            }

        }

        // +-----------------------------------------------+
        // | Update parameters                             |
        // +-----------------------------------------------+

        UpdateParametersEvent *updateParametersEvent = dynamic_cast<UpdateParametersEvent*>(event);

        if (updateParametersEvent != NULL)
        {

            ConfigurationVector tmp = updateParametersEvent->getConfigurationVector();

            this->setBackgroundColor(tmp.getElementColor("SPHERE_BACKGROUND_COLOR"));
            this->setTopGridColor(tmp.getElementColor("SPHERE_NORTH_COLOR"));
            this->setBottomGridColor(tmp.getElementColor("SPHERE_SOUTH_COLOR"));
            this->setAxesColor(tmp.getElementColor("SPHERE_AXES_COLOR"));

            this->updateGL();

        }


        return true;

    }

    return QGLWidget::event(event);

}

// +-------------------------------------------------------+
// | Mouse is pressed                                      |
// +-------------------------------------------------------+

void Graph3D::mousePressEvent(QMouseEvent *event)
{
    this->cursorLastPosition = event->pos();
    this->setCursor(Qt::ClosedHandCursor);
}

// +-------------------------------------------------------+
// | Mouse is released                                     |
// +-------------------------------------------------------+

void Graph3D::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(Qt::OpenHandCursor);
}

// +-------------------------------------------------------+
// | Mouse is moved                                        |
// +-------------------------------------------------------+

void Graph3D::mouseMoveEvent(QMouseEvent *event)
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

        this->update();

    }

}

// +-------------------------------------------------------+
// | Wheel is turned                                       |
// +-------------------------------------------------------+

void Graph3D::wheelEvent(QWheelEvent *event)
{

    this->zoom += (event->delta() / 1000.0);
    this->updateGL();

}

QColor Graph3D::getColorEnergy(float _energy)
{

    float maxRange = 5;
    float minRange = 0;
    float maxRange2 = exp(1);
    float minRange2 = 1;

    float value1 = (_energy - minRange) / (maxRange - minRange);

    float value2 = (value1 * (maxRange2 - minRange2)) + minRange2;

    if (value2 > maxRange2)
    {
        value2 = maxRange2;
    }
    if (value2 < minRange2)
    {
        value2 = minRange2;
    }

    float value = log(value2);

    float red = (value / 0.5) * 255;

    if (red < 0)
    {
        red = 0;
    }

    if (red > 255)
    {
        red = 255;
    }

    float blue = 255 - red;

    float green = ((value - 0.5) / 0.5) * 255;

    if (green < 0)
    {
        green = 0;
    }

    if (green > 255)
    {
        green = 255;
    }

    return QColor(red, green, blue, 1);

}

