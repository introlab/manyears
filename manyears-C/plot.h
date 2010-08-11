#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QBrush>
#include <QColor>
#include <QString>
#include <QVector>
#include <QMutex>

class Plot : public QWidget
{

    Q_OBJECT

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    Plot(int _gridWidth, int _labelWidth, int _height, int _topMargin, int _bottomMargin, int _leftMargin, int _rightMargin, int _spacerWidth);//, QMutex *_mutex);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~Plot();

    /***********************************************************
    * Connections                                              *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Connect the plot with the data sources                |
    // +-------------------------------------------------------+

    void connectNumberValues(int *_numberValues);
    void connectValuesToDraw(QVector<float> *_valuesToDraw);
    void connectColorsToDraw(QVector<QColor> *_colorsToDraw);

    /***********************************************************
    * Refreshing data                                          *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Initialize the graph                                  |
    // +-------------------------------------------------------+

    void initPlot();

    // +-------------------------------------------------------+
    // | End of the current frame                              |
    // +-------------------------------------------------------+

    void addFrame();

    // +-------------------------------------------------------+
    // | Update the graph                                      |
    // +-------------------------------------------------------+

    void updateGraph();

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Range for the X and Y dimensions                      |
    // +-------------------------------------------------------+

    // X-Dimension

    float getXMin();
    float getXMax();
    float getXInterval();

    // Y-Dimension

    float getYMin();
    float getYMax();
    float getYInterval();

    // +-------------------------------------------------------+
    // | Layout                                                |
    // +-------------------------------------------------------+

    // Margins

    int getLeftMargin();
    int getRightMargin();
    int getTopMargin();
    int getBottomMargin();

    // Elements

    int getLabelWidth();
    int getSpacerWidth();
    int getGridWidth();

    // Widget

    int getWidth();
    int getHeight();

    // +-------------------------------------------------------+
    // | Appearance                                            |
    // +-------------------------------------------------------+

    // Widget

    QColor getColorBackground();
    QColor getColorBorder();

    // Points

    int getSizePoint();
    float getRatioPoint();

    // Grid

    QColor getColorHorizontalBar();
    QColor getColorVerticalBar();

    // Label

    QColor getFontColorLabel();
    int getFontSizeLabel();
    int getFontWeightLabel();
    QString getFontFamilyLabel();
    bool getFontItalicLabel();
    bool getFontBoldLabel();

    // +-------------------------------------------------------+
    // | Scaling                                               |
    // +-------------------------------------------------------+

    // Sampling rate

    float getSamplesPerSecond();


    /***********************************************************
    * Mutators                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Range for the X and Y dimensions                      |
    // +-------------------------------------------------------+

    // X-Dimension

    void setXMin(float _xMin);
    void setXMax(float _xMax);
    void setXInterval(float _xInterval);

    // Y-Dimension

    void setYMin(float _yMin);
    void setYMax(float _yMax);
    void setYInterval(float _yInterval);

    // +-------------------------------------------------------+
    // | Appearance                                            |
    // +-------------------------------------------------------+

    // Widget

    void setColorBackground(QColor _colorBackground);
    void setColorBorder(QColor _colorBorder);

    // Points

    void setSizePoint(int _sizePoint);

    // Grid

    void setColorHorizontalBar(QColor _colorHorizontalBar);
    void setColorVerticalBar(QColor _colorVerticalBar);

    // Label

    void setFontColorLabel(QColor _fontColorLabel);
    void setFontSizeLabel(int _fontSizeLabel);
    void setFontWeightLabel(int _fontWeightLabel);
    void setFontFamilyLabel(QString _fontFamilyLabel);
    void setFontItalicLabel(bool _fontItalicLabel);
    void setFontBoldLabel(bool _fontBoldLabel);

    // +-------------------------------------------------------+
    // | Scaling                                               |
    // +-------------------------------------------------------+

    // Sampling rate

    void setSamplesPerSecond(float _samplesPerSecond);


protected:

    /***********************************************************
    * Protected events                                         *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Paint                                                 |
    // +-------------------------------------------------------+

    void paintEvent( QPaintEvent * );


private:

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Pixel map                                             |
    // +-------------------------------------------------------+

    // Pixel map

    QPixmap *pixelMap;

    // Grid pixel map

    QPixmap *gridPixelMap;

    // Label pixel map
    QPixmap *labelPixelMap;

    // Horizontal bars pixel map

    QPixmap *hBarPixelMap;

    // Vertical bar pixel map

    QPixmap *vBarPixelMap;

    // +-------------------------------------------------------+
    // | Raw data to be drawn                                  |
    // +-------------------------------------------------------+

    // Number of values to draw on next rendering

    int *numberValues;

    // Pointer to the vector of values to be drawn

    QVector<float> *valuesToDraw;

    // Pointer to the vector of colors for the drawn values

    QVector<QColor> *colorsToDraw;

    // +-------------------------------------------------------+
    // | Range for the X and Y dimensions                      |
    // +-------------------------------------------------------+

    // X-Dimension

    float xMin;
    float xMax;
    float xInterval;

    // Y-Dimension

    float yMin;
    float yMax;
    float yInterval;

    // +-------------------------------------------------------+
    // | Layout                                                |
    // +-------------------------------------------------------+

    // Margins

    int leftMargin;
    int rightMargin;
    int topMargin;
    int bottomMargin;

    // Elements

    int labelWidth;
    int labelHeight;
    int spacerWidth;
    int gridWidth;
    int gridHeight;

    // Widget

    int width;
    int height;

    // +-------------------------------------------------------+
    // | Appearance                                            |
    // +-------------------------------------------------------+

    // Widget

    QColor colorBackground;
    QColor colorBorder;

    // Points

    int sizePoint;
    float ratioPoint;

    // Grid

    QColor colorHorizontalBar;
    QColor colorVerticalBar;

    // Label

    QColor fontColorLabel;
    int fontSizeLabel;
    int fontWeightLabel;
    QString fontFamilyLabel;
    bool fontItalicLabel;
    bool fontBoldLabel;

    // +-------------------------------------------------------+
    // | Mutex                                                 |
    // +-------------------------------------------------------+

    // Lock the plot for rendering
    //QMutex *blockRendering;

    // +-------------------------------------------------------+
    // | Scaling                                               |
    // +-------------------------------------------------------+

    // Sampling rate

    float samplesPerSecond;

    // Number of seconds per sample

    float timePerSample;

    // Number of pixels per sample

    float pixelPerSample;

    // Time per pixel

    float timePerPixel;

    // Time accumulator

    float timeAccumulator;

    // Pixel accumulator

    float pixelAccumulator;

    /***********************************************************
    * Drawing                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Preparing for drawing                                 |
    // +-------------------------------------------------------+

    void initDrawing();

    // +-------------------------------------------------------+
    // | Draw the fixed full grid                              |
    // +-------------------------------------------------------+

    void drawGrid();

    // +-------------------------------------------------------+
    // | Draw the labels                                       |
    // +-------------------------------------------------------+

    void drawLabels();

    /***********************************************************
    * Refreshing graph                                         *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Refresh the plot                                      |
    // +-------------------------------------------------------+

    void refreshPlot();

};

#endif // PLOT_H
