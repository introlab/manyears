#include "sourceGenerator.h"

SourceGenerator::SourceGenerator(SourceManager *_myManager, PlotLongitude *_myPlotLongitude, PlotLatitude *_myPlotLatitude, SpherePlot *_mySpherePlot)
{

    this->myManager = _myManager;
    this->myPlotLongitude = _myPlotLongitude;
    this->myPlotLatitude = _myPlotLatitude;
    this->mySpherePlot = _mySpherePlot;
    this->t = 0;
    this->refresh3d = 0;

}

void SourceGenerator::generateSources()
{

    this->t++;

    this->myManager->addSource(1,0.8*cos(t/100.0),0.8*sin(t/100.0),+0.6);
    this->myManager->addSource(2,0.8*sin(t/100.0),0.8*cos(t/100.0),-0.6);

    this->myManager->updateFrame();
    this->myPlotLongitude->updateGraph();
    this->myPlotLatitude->updateGraph();

    this->refresh3d++;

    if (this->refresh3d > 5)
    {
        this->refresh3d = 0;
        this->mySpherePlot->updateGL();
    }

}
