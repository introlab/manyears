#include "window.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

Window::Window()
{

    init();

}

/***********************************************************
* Initialization                                           *
***********************************************************/

void Window::init()
{

    const int scrollBarWidth = 15;

    // +-------------------------------------------------------+
    // | Create the menubar                                    |
    // +-------------------------------------------------------+

    this->menuBar = new QMenuBar(this);

    this->menuSystem = new QMenu(this->menuBar);
    this->menuSystem->setTitle("System");

    this->menuSystemStart = new QAction(this->menuSystem);
    this->menuSystemStart->setText("Start");
    this->menuSystemStop = new QAction(this->menuSystem);
    this->menuSystemStop->setText("Stop");
    this->menuSystemConfiguration = new QAction(this->menuSystem);
    this->menuSystemConfiguration->setText("System configuration panel");
    this->menuSystemConfiguration->setCheckable(true);
    this->menuSystemConfigurationOpen = new QAction(this->menuSystem);
    this->menuSystemConfigurationOpen->setText("Open system configuration vector...");
    this->menuSystemConfigurationSave = new QAction(this->menuSystem);
    this->menuSystemConfigurationSave->setText("Save system configuration vector...");

    this->menuSystemQuit = new QAction(this->menuSystem);
    this->menuSystemQuit->setText("Quit");

    this->menuView = new QMenu(this->menuBar);
    this->menuView->setTitle("View");

    this->menuViewLatitude = new QAction(this->menuView);
    this->menuViewLatitude->setText("Latitude");
    this->menuViewLatitude->setCheckable(true);
    this->menuViewLongitude = new QAction(this->menuView);
    this->menuViewLongitude->setText("Longitude");
    this->menuViewLongitude->setCheckable(true);
    this->menuViewSphere = new QAction(this->menuView);
    this->menuViewSphere->setText("Sphere");
    this->menuViewSphere->setCheckable(true);
    this->menuViewConfiguration = new QAction(this->menuView);
    this->menuViewConfiguration->setText("View configuration panel");
    this->menuViewConfiguration->setCheckable(true);
    this->menuViewConfigurationOpen = new QAction(this->menuView);
    this->menuViewConfigurationOpen->setText("Open view configuration vector...");
    this->menuViewConfigurationSave = new QAction(this->menuView);
    this->menuViewConfigurationSave->setText("Save view configuration vector...");

    this->menuSystem->addAction(this->menuSystemStart);
    this->menuSystem->addAction(this->menuSystemStop);
    this->menuSystem->addSeparator();
    this->menuSystem->addAction(this->menuSystemConfiguration);
    this->menuSystem->addSeparator();
    this->menuSystem->addAction(this->menuSystemConfigurationOpen);
    this->menuSystem->addAction(this->menuSystemConfigurationSave);
    this->menuSystem->addSeparator();
    this->menuSystem->addAction(this->menuSystemQuit);

    this->menuView->addAction(this->menuViewLatitude);
    this->menuView->addAction(this->menuViewLongitude);
    this->menuView->addAction(this->menuViewSphere);
    this->menuView->addSeparator();
    this->menuView->addAction(this->menuViewConfiguration);
    this->menuView->addSeparator();
    this->menuView->addAction(this->menuViewConfigurationOpen);
    this->menuView->addAction(this->menuViewConfigurationSave);

    this->menuBar->addAction(this->menuSystem->menuAction());
    this->menuBar->addAction(this->menuView->menuAction());

    this->setMenuBar(this->menuBar);

    // Connect each action with its corresponding function
    QObject::connect(this->menuSystemStart, SIGNAL(triggered()), this, SLOT(menuSystemStartClicked()));
    QObject::connect(this->menuSystemStop, SIGNAL(triggered()), this, SLOT(menuSystemStopClicked()));
    QObject::connect(this->menuSystemConfiguration, SIGNAL(triggered(bool)), this, SLOT(menuSystemConfigurationToggled()));
    QObject::connect(this->menuSystemConfigurationOpen, SIGNAL(triggered()), this, SLOT(menuSystemConfigurationOpenClicked()));
    QObject::connect(this->menuSystemConfigurationSave, SIGNAL(triggered()), this, SLOT(menuSystemConfigurationSaveClicked()));
    QObject::connect(this->menuSystemQuit, SIGNAL(triggered()), this, SLOT(menuSystemQuitClicked()));
    QObject::connect(this->menuViewLatitude, SIGNAL(triggered()), this, SLOT(menuViewLatitudeToggled()));
    QObject::connect(this->menuViewLongitude, SIGNAL(triggered()), this, SLOT(menuViewLongitudeToggled()));
    QObject::connect(this->menuViewSphere, SIGNAL(triggered()), this, SLOT(menuViewSphereToggled()));
    QObject::connect(this->menuViewConfiguration, SIGNAL(triggered()), this, SLOT(menuViewConfigurationToggled()));
    QObject::connect(this->menuViewConfigurationOpen, SIGNAL(triggered()), this, SLOT(menuViewConfigurationOpenClicked()));
    QObject::connect(this->menuViewConfigurationSave, SIGNAL(triggered()), this, SLOT(menuViewConfigurationSaveClicked()));

    // +-------------------------------------------------------+
    // | Create the docks                                      |
    // +-------------------------------------------------------+

    this->dockConfigSystem = new QDockWidget();
    this->dockConfigSystem->setWindowTitle("System configuration");
    this->dockConfigSystem->setMinimumWidth(250);
    this->dockConfigView = new QDockWidget();
    this->dockConfigView->setWindowTitle("View configuration");
    this->dockConfigView->setMinimumWidth(250);

    this->addDockWidget(Qt::LeftDockWidgetArea, this->dockConfigSystem);
    this->addDockWidget(Qt::LeftDockWidgetArea, this->dockConfigView);

    this->listPropertiesSystem = new ScrollListProperty(this->dockConfigSystem->width(),scrollBarWidth,this->dockConfigSystem->height());
    this->dockConfigSystem->setWidget(this->listPropertiesSystem);
    this->listPropertiesView = new ScrollListProperty(this->dockConfigView->width(),scrollBarWidth,this->dockConfigView->height());
    this->dockConfigView->setWidget(this->listPropertiesView);

    // +-------------------------------------------------------+
    // | Populate the docks                                    |
    // +-------------------------------------------------------+

    this->parametersManager = new ParametersManager();
    this->parametersManager->initView();
    this->parametersManager->initSystem();
    this->listPropertiesView->loadParameters(this->parametersManager->getRootPropertyView());
    this->listPropertiesSystem->loadParameters(this->parametersManager->getRootPropertySystem());
    this->parametersManager->loadViewDefault();
    this->parametersManager->loadSystemDefault();

    // +-------------------------------------------------------+
    // | Configure the view by default                         |
    // +-------------------------------------------------------+

    setViewLongitude(true);
    setViewLatitude(true);
    setViewSphere(true);

    // +-------------------------------------------------------+
    // | Resize the window                                     |
    // +-------------------------------------------------------+

    this->resize(this->windowWidth, this->windowHeight);

    // +-------------------------------------------------------+
    // | Create widgets                                        |
    // +-------------------------------------------------------+

    this->mdiArea = new QMdiArea(this);
    this->setCentralWidget(this->mdiArea);

    this->colorPalette = new ColorPalette();
    //this->potentialSourceManager = new PotentialSourceManager(10.0,48000.0/512.0);
    this->sourceManager = new SourceManager(10,this->colorPalette);

    // +-------------------------------------------------------+
    // | Create sub windows                                    |
    // +-------------------------------------------------------+

    this->mdiWinLongitude = new WinLongitude(600,300,48000,512,this->sourceManager);
    this->mdiWinLatitude = new WinLatitude(600,300,48000,512,this->sourceManager);
    //this->mdiWinBeamformerLongitude = new WinBeamformerLongitude(600,300,48000,512,this->potentialSourceManager);
    //this->mdiWinBeamformerLatitude = new WinBeamformerLatitude(600,300,48000,512,this->potentialSourceManager);
    this->mdiWinSphere = new WinSphere(800,300,48000,512,this->sourceManager,5);
    this->mdiWinLongitude->setWindowTitle("Longitude");
    this->mdiWinLatitude->setWindowTitle("Latitude");
    //this->mdiWinBeamformerLongitude->setWindowTitle("Potential sources longitude");
    //this->mdiWinBeamformerLatitude->setWindowTitle("Potential sources latitude");
    this->mdiWinSphere->setWindowTitle("3D View");
    this->mdiArea->addSubWindow(this->mdiWinLongitude);
    this->mdiArea->addSubWindow(this->mdiWinLatitude);
    //this->mdiArea->addSubWindow(this->mdiWinBeamformerLongitude);
    //this->mdiArea->addSubWindow(this->mdiWinBeamformerLatitude);
    this->mdiArea->addSubWindow(this->mdiWinSphere);

    // +-------------------------------------------------------+
    // | Connect signal to slot for refresh                    |
    // +-------------------------------------------------------+

    QObject::connect(this,SIGNAL(refreshNeeded()),this,SLOT(performRefresh()));

    // +-------------------------------------------------------+
    // | Connect signal to confirm system is stopped           |
    // +-------------------------------------------------------+

    QObject::connect(this,SIGNAL(confirmOperation()),this,SLOT(confirmReadyOperation()));

    // +-------------------------------------------------------+
    // | Create the sound card selection dialog                |
    // +-------------------------------------------------------+

    this->soundInputConfig = new SoundInputConfig(1024,512,48000,8);

    // +-------------------------------------------------------+
    // | Init status of the menu bar                           |
    // +-------------------------------------------------------+

    setSystemRunning(false);
    this->running = false;

    // +-------------------------------------------------------+
    // | Adjust appearance according to default parameters     |
    // +-------------------------------------------------------+

    refreshViewAppearance();

    // +-------------------------------------------------------+
    // | Connect signal to slot for updating appearance        |
    // +-------------------------------------------------------+

    QObject::connect(this->listPropertiesView->getListProperty(), SIGNAL(valueChanged()), this, SLOT(slotRefreshViewAppearance()));
    QObject::connect(this->listPropertiesSystem->getListProperty(), SIGNAL(valueChanged()), this, SLOT(slotRefreshSystemCore()));

    // +-------------------------------------------------------+
    // | Operation                                             |
    // +-------------------------------------------------------+

    // By default no operation triggered
    this->userOperation = NOP;
}

/***********************************************************
* Slots                                                    *
***********************************************************/

// +-------------------------------------------------------+
// | Process the graphs refresh query                      |
// +-------------------------------------------------------+

void Window::performRefresh()
{
    qDebug("performRefresh thread: %p",QThread::currentThread());
    this->updateGraphs();

}

// +-------------------------------------------------------+
// | Confirm the system has stopped                        |
// +-------------------------------------------------------+

void Window::confirmReadyOperation()
{

    switch (this->userOperation)
    {
        case NOP:
            break;

        case START:

            // Display system is started
            this->running = true;
            setSystemRunning(true);

            this->userOperation = NOP;

            break;

        case STOP:

            // Display system is stopped
            this->running = false;
            setSystemRunning(false);

            this->userOperation = NOP;

            break;

        case SYSTOPEN:

            // Open configuration
            openSystemConfig();

            // System will then restart if it was stopped
            if (this->running == true)
            {
                this->userOperation = NOP;
                emit(this->startCore());
            }

            this->userOperation = NOP;

            break;

        case SYSTSAVE:

            // Save configuration
            saveSystemConfig();

            // System will then restart if it was stopped
            if (this->running == true)
            {
                this->userOperation = NOP;
                emit(this->startCore());
            }

            this->userOperation = NOP;

            break;

        case SYSTREFRESH:

            // Refresh the appearance
            refreshSystemCore();

            // System will then restart if it was stopped
            if (this->running == true)
            {
                this->userOperation = NOP;
                emit(this->startCore());
            }

            this->userOperation = NOP;

            break;

        case VIEWOPEN:

            // Open configuration
            openViewConfig();

            // System will then restart if it was stopped
            if (this->running == true)
            {
                this->userOperation = NOP;
                emit(this->startCore());
            }

            this->userOperation = NOP;

            break;

        case VIEWSAVE:

            // Save configuration
            saveViewConfig();

            // System will then restart if it was stopped
            if (this->running == true)
            {
                this->userOperation = NOP;
                emit(this->startCore());
            }

            this->userOperation = NOP;

            break;

        case VIEWREFRESH:

            // Refresh the appearance
            refreshViewAppearance();

            // System will then restart if it was stopped
            if (this->running == true)
            {
                this->userOperation = NOP;
                emit(this->startCore());
            }

            this->userOperation = NOP;

            break;

    }

}

/***********************************************************
* Refresh                                                  *
***********************************************************/

void Window::loadWindow()
{
    this->promptSoundCard();
}

void Window::askForUpdateGraphs()
{
    qDebug("AskForUpdateGraph Thread: %p",QThread::currentThread());
    emit(this->refreshNeeded());
}

void Window::updateGraphs()
{
    this->mdiWinLatitude->updatePlot();
    this->mdiWinLongitude->updatePlot();
    //this->mdiWinBeamformerLatitude->updatePlot();
    //this->mdiWinBeamformerLongitude->updatePlot();
    this->mdiWinSphere->updatePlot();

}

PotentialSourceManager* Window::getPotentialSourceManager()
{
    return this->potentialSourceManager;
}

SourceManager* Window::getSourceManager()
{
    return this->sourceManager;
}

void Window::setViewLongitude(bool _enabled)
{

    this->menuViewLongitude->setChecked(_enabled);
    this->viewLongitude = _enabled;

}

void Window::setViewLatitude(bool _enabled)
{

    this->menuViewLatitude->setChecked(_enabled);
    this->viewLatitude = _enabled;

}

void Window::setViewSphere(bool _enabled)
{

    this->menuViewSphere->setChecked(_enabled);
    this->viewSphere = _enabled;

}

void Window::setSystemConfig(bool _shown)
{

    this->menuSystemConfiguration->setChecked(_shown);
    this->showSystemConfig = _shown;

}

void Window::setViewConfig(bool _shown)
{

    this->menuViewConfiguration->setChecked(_shown);
    this->showViewConfig = _shown;

}

void Window::setSystemRunning(bool _running)
{

    if (_running == true)
    {
        this->menuSystemStart->setEnabled(false);
        this->menuSystemStop->setEnabled(true);
    }
    else
    {
        this->menuSystemStart->setEnabled(true);
        this->menuSystemStop->setEnabled(false);
    }

}

void Window::promptSoundCard()
{


}

void Window::refreshSoundCard()
{

}

void Window::updateDocks()
{

    // Refresh the plots
    this->listPropertiesSystem->getListProperty()->drawAllElements("");
    this->listPropertiesSystem->getListProperty()->update();
    this->listPropertiesView->getListProperty()->drawAllElements("");
    this->listPropertiesView->getListProperty()->update();

}

void Window::refreshViewAppearance()
{

    // Update the plots
    this->mdiWinSphere->setBackgroundColor(this->parametersManager->getRootPropertyView()->getProperty("SPHERE_BACKGROUND_COLOR")->getColor());
    this->mdiWinSphere->setTopGridColor(this->parametersManager->getRootPropertyView()->getProperty("SPHERE_NORTH_COLOR")->getColor());
    this->mdiWinSphere->setBottomGridColor(this->parametersManager->getRootPropertyView()->getProperty("SPHERE_SOUTH_COLOR")->getColor());

    this->mdiWinLongitude->setColorBackground(this->parametersManager->getRootPropertyView()->getProperty("LONGITUDE_BACKGROUND_COLOR")->getColor());
    this->mdiWinLongitude->setColorHorizontalBar(this->parametersManager->getRootPropertyView()->getProperty("LONGITUDE_HORIZONTALAXES_COLOR")->getColor());
    this->mdiWinLongitude->setColorVerticalBar(this->parametersManager->getRootPropertyView()->getProperty("LONGITUDE_VERTICALAXES_COLOR")->getColor());
    this->mdiWinLongitude->setSizePoint(this->parametersManager->getRootPropertyView()->getProperty("LONGITUDE_TRACE_SIZE")->getInt());
    this->mdiWinLongitude->setXMin(this->parametersManager->getRootPropertyView()->getProperty("LONGITUDE_TRACE_TIME_BUFFER")->getFloat() * -1);
    this->mdiWinLongitude->setXMax(this->parametersManager->getRootPropertyView()->getProperty("LONGITUDE_TRACE_TIME_AFTER")->getFloat());
    this->mdiWinLongitude->setXInterval(this->parametersManager->getRootPropertyView()->getProperty("LONGITUDE_TRACE_TIME_INTERVAL")->getFloat());

    this->mdiWinLatitude->setColorBackground(this->parametersManager->getRootPropertyView()->getProperty("LATITUDE_BACKGROUND_COLOR")->getColor());
    this->mdiWinLatitude->setColorHorizontalBar(this->parametersManager->getRootPropertyView()->getProperty("LATITUDE_HORIZONTALAXES_COLOR")->getColor());
    this->mdiWinLatitude->setColorVerticalBar(this->parametersManager->getRootPropertyView()->getProperty("LATITUDE_VERTICALAXES_COLOR")->getColor());
    this->mdiWinLatitude->setSizePoint(this->parametersManager->getRootPropertyView()->getProperty("LATITUDE_TRACE_SIZE")->getInt());
    this->mdiWinLatitude->setXMin(this->parametersManager->getRootPropertyView()->getProperty("LATITUDE_TRACE_TIME_BUFFER")->getFloat() * -1);
    this->mdiWinLatitude->setXMax(this->parametersManager->getRootPropertyView()->getProperty("LATITUDE_TRACE_TIME_AFTER")->getFloat());
    this->mdiWinLatitude->setXInterval(this->parametersManager->getRootPropertyView()->getProperty("LATITUDE_TRACE_TIME_INTERVAL")->getFloat());

    // Update the colors
    this->colorPalette->stop();
    this->colorPalette->removeAllColor();
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE1_COLOR")->getColor());
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE2_COLOR")->getColor());
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE3_COLOR")->getColor());
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE4_COLOR")->getColor());
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE5_COLOR")->getColor());
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE6_COLOR")->getColor());
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE7_COLOR")->getColor());
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE8_COLOR")->getColor());
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE9_COLOR")->getColor());
    this->colorPalette->addColor(this->parametersManager->getRootPropertyView()->getProperty("SOURCE10_COLOR")->getColor());
    this->colorPalette->start();

    // Refresh plots
    this->mdiWinLatitude->initPlot();
    this->mdiWinLongitude->initPlot();
    this->mdiWinLatitude->updatePlot();
    this->mdiWinLongitude->updatePlot();;

}

void Window::refreshSystemCore()
{

    this->parametersManager->copyParametersSystemCore();

}

/***********************************************************
* Save / Open Files                                        *
***********************************************************/

void Window::saveSystemConfig()
{

    QString selectedFile;
    QString nameFilter;

    nameFilter = "ManyEars System Configuration files";
    nameFilter.append(" ");
    nameFilter.append("(*.");
    nameFilter.append(FILE_SYSTEM_TYPE);
    nameFilter.append(")");

    selectedFile = QFileDialog::getSaveFileName(this,"Save","",nameFilter);

    if (selectedFile != "")
    {

        this->parametersManager->saveParametersSystem(selectedFile);

    }

}

void Window::openSystemConfig()
{

    QString selectedFile;
    QString nameFilter;

    nameFilter = "ManyEars System Configuration files";
    nameFilter.append(" ");
    nameFilter.append("(*.");
    nameFilter.append(FILE_SYSTEM_TYPE);
    nameFilter.append(")");

    selectedFile = QFileDialog::getOpenFileName(this,"Open","",nameFilter);

    if (selectedFile != "")
    {

        this->parametersManager->openParametersSystem(selectedFile);
        this->updateDocks();

    }

}

void Window::saveViewConfig()
{

    QString selectedFile;
    QString nameFilter;

    nameFilter = "ManyEars View Configuration files";
    nameFilter.append(" ");
    nameFilter.append("(*.");
    nameFilter.append(FILE_VIEW_TYPE);
    nameFilter.append(")");

    selectedFile = QFileDialog::getSaveFileName(this,"Save","",nameFilter);

    if (selectedFile != "")
    {

        this->parametersManager->saveParametersView(selectedFile);

    }

}

void Window::openViewConfig()
{

    QString selectedFile;
    QString nameFilter;

    nameFilter = "ManyEars View Configuration files";
    nameFilter.append(" ");
    nameFilter.append("(*.");
    nameFilter.append(FILE_VIEW_TYPE);
    nameFilter.append(")");

    selectedFile = QFileDialog::getOpenFileName(this,"Open","",nameFilter);

    if (selectedFile != "")
    {

        this->parametersManager->openParametersView(selectedFile);
        this->updateDocks();

    }

}

/***********************************************************
* User interface                                           *
***********************************************************/

// +-------------------------------------------------------+
// | Triggers an operation                                 |
// +-------------------------------------------------------+

void Window::triggerOperation(USER_OPERATION _operation)
{

    this->userOperation = _operation;

    // Stop the system if needed

    if ((this->userOperation == STOP) || (this->userOperation == SYSTOPEN) || (this->userOperation == SYSTSAVE) || (this->userOperation == SYSTREFRESH) || (this->userOperation == VIEWOPEN) || (this->userOperation == VIEWSAVE) || ((this->userOperation == VIEWREFRESH)))
    {

        if (this->running == false)
        {
            emit(this->confirmOperation());
        }

        if (this->running == true)
        {
            emit(this->stopCore());
        }

    }

    if (this->userOperation == START)
    {

        if (this->running == true)
        {
            emit(this->confirmOperation());
        }

        if (this->running == false)
        {
            emit(this->startCore());
        }

    }

}

// +-------------------------------------------------------+
// | Properties updated                                    |
// +-------------------------------------------------------+

void Window::slotRefreshViewAppearance()
{

    // Trigger operation
    this->triggerOperation(VIEWREFRESH);

}

void Window::slotRefreshSystemCore()
{

    // Trigger operation
    this->triggerOperation(SYSTREFRESH);

}

// +-------------------------------------------------------+
// | Menubar                                               |
// +-------------------------------------------------------+

void Window::menuSystemStartClicked()
{

    // Trigger operation
    this->triggerOperation(START);

}

void Window::menuSystemStopClicked()
{

    // Trigger operation
    this->triggerOperation(STOP);

}

void Window::menuSystemConfigurationToggled()
{

}

void Window::menuSystemConfigurationOpenClicked()
{

    // Trigger operation
    this->triggerOperation(SYSTOPEN);

}

void Window::menuSystemConfigurationSaveClicked()
{

    // Trigger operation
    this->triggerOperation(SYSTSAVE);

}

void Window::menuSystemQuitClicked()
{

}

void Window::menuViewLongitudeToggled()
{

}

void Window::menuViewLatitudeToggled()
{

}

void Window::menuViewSphereToggled()
{

}

void Window::menuViewConfigurationToggled()
{

}

void Window::menuViewConfigurationOpenClicked()
{

    // Trigger operation
    this->triggerOperation(VIEWOPEN);

}

void Window::menuViewConfigurationSaveClicked()
{

    // Trigger operation
    this->triggerOperation(VIEWSAVE);

}
