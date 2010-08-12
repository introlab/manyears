#ifndef SOURCEGENERATOR_H
#define SOURCEGENERATOR_H

#include <QWidget>

#include "sourceManager.h"
#include "plotLongitude.h"
#include "plotLatitude.h"
#include "spherePlot.h"

class SourceGenerator: public QWidget
{

    Q_OBJECT

public:

    SourceGenerator(SourceManager *_myManager, PlotLongitude *_myPlotLongitude, PlotLatitude *_myPlotLatitude, SpherePlot *_mySpherePlot);

public slots:

    void generateSources();

private:

    SourceManager *myManager;
    PlotLongitude *myPlotLongitude;
    PlotLatitude *myPlotLatitude;
    SpherePlot *mySpherePlot;
    float t;
    int refresh3d;

};

#endif // SOURCEGENERATOR_H
