#include "mdiWindow.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

MDIWindow::MDIWindow()
{

    // +-------------------------------------------------------+
    // | General                                               |
    // +-------------------------------------------------------+

    // Create core thread
    this->runningThread = new CoreThread();

    // Create input/output manager
    this->inputOutputManager = new InputOutputManager();
    this->runningThread->setInputOuputManager(this->inputOutputManager);

    // Define size of this window
    int windowWidth = 1500;
    int windowHeight = 1000;

    this->setGeometry(50,50,windowWidth,windowHeight);

    // Create main zone
    this->mdiArea = new QMdiArea(this);
    this->setCentralWidget(this->mdiArea);

    // +---------------------------------------------------+
    // | Parameters managers                               |
    // +---------------------------------------------------+

    this->parametersManager = new ParametersManager();
    this->rootSystem = this->parametersManager->initSystem();
    this->rootView = this->parametersManager->initView();
    this->parametersManager->loadSystemDefault();
    this->parametersManager->openParametersSystem(":/Configuration_Vectors/default.mes");
    this->parametersManager->loadViewDefault();  

    this->runningThread->loadParameters(this->parametersManager->getParametersSystem());

    // +---------------------------------------------------+
    // | Sources managers                                  |
    // +---------------------------------------------------+

    this->potentialSourcesManager = new PotentialSourceBufferManager();
    this->trackedSourcesManager = new TrackedSourceBufferManager();

    // +-------------------------------------------------------+
    // | User input                                            |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Properties                                        |
        // +---------------------------------------------------+

            // +-----------------------------------------------+
            // | Create docks                                  |
            // +-----------------------------------------------+

                // Create docks

                int leftDockWidth = 300;
                int rightDockWidth = 300;
                int emptyWidth = 0;

                this->dockConfigSystem = new QDockWidget();
                this->dockConfigSystem->setWindowTitle("System configuration");
                this->dockConfigSystem->setMinimumWidth(leftDockWidth);
                this->addDockWidget(Qt::LeftDockWidgetArea, this->dockConfigSystem);
                this->dockConfigView = new QDockWidget();
                this->dockConfigView->setWindowTitle("View configuration");
                this->dockConfigView->setMinimumWidth(rightDockWidth);
                this->addDockWidget(Qt::RightDockWidgetArea, this->dockConfigView);

            // +-----------------------------------------------+
            // | Create property editors                       |
            // +-----------------------------------------------+

                // Create property editors

                int scrollBarWidth = 20;

                this->propertyEditorSystem = new PropertyEditor(this->dockConfigSystem->width(), this->dockConfigSystem->height(), scrollBarWidth, *this->rootSystem, PropertyEditor::SYSTEM);
                this->dockConfigSystem->setWidget(this->propertyEditorSystem);
                this->propertyEditorView = new PropertyEditor(this->dockConfigView->width(), this->dockConfigView->height(), scrollBarWidth, *this->rootView, PropertyEditor::VIEW);
                this->dockConfigView->setWidget(this->propertyEditorView);

                // Load default values
                this->propertyEditorSystem->loadValues(this->parametersManager->copyParametersSystem());
                this->propertyEditorView->loadValues(this->parametersManager->copyParametersView());

            // +-----------------------------------------------+
            // | Create actions                                |
            // +-----------------------------------------------+

                this->action_action_start = new QAction("Start", this);
                this->action_action_stop = new QAction("Stop", this);
                this->action_action_select = new QAction("Select devices...", this);
                this->action_action_quit = new QAction("Quit", this);
                this->action_parameters_open_view = new QAction("Load parameters for the current view...", this);
                this->action_parameters_save_view = new QAction("Save the parameter of the current view...", this);
                this->action_parameters_open_system = new QAction("Load parameters for the system...", this);
                this->action_parameters_save_system = new QAction("Save the parameters of the system...", this);
                this->action_parameters_show_view = new QAction("Show the view parameters", this);
                this->action_parameters_show_system = new QAction("Show the system parameters", this);
                this->action_windows_show_potential_latitude = new QAction("Latitude of potential sources", this);
                this->action_windows_show_potential_longitude = new QAction("Longitude of potential sources", this);
                this->action_windows_show_tracked_latitude = new QAction("Latitude of tracked sources", this);
                this->action_windows_show_tracked_longitude = new QAction("Longitude of tracked sources", this);
                this->action_windows_show_tracked_sphere = new QAction("3D Sphere of tracked sources", this);
                this->action_help_about = new QAction("&About", this);

                QObject::connect(this->action_action_start, SIGNAL(triggered()), this, SLOT(action_start_slot()));
                QObject::connect(this->action_action_stop, SIGNAL(triggered()), this, SLOT(action_stop_slot()));
                QObject::connect(this->action_action_select, SIGNAL(triggered()), this, SLOT(action_select_slot()));
                QObject::connect(this->action_action_quit, SIGNAL(triggered()), this, SLOT(action_quit_slot()));
                QObject::connect(this->action_parameters_open_view, SIGNAL(triggered()), this, SLOT(action_parameters_open_view_slot()));
                QObject::connect(this->action_parameters_save_view, SIGNAL(triggered()), this, SLOT(action_parameters_save_view_slot()));
                QObject::connect(this->action_parameters_show_view, SIGNAL(triggered()), this, SLOT(action_parameters_show_view_slot()));
                QObject::connect(this->action_parameters_open_system, SIGNAL(triggered()), this, SLOT(action_parameters_open_system_slot()));
                QObject::connect(this->action_parameters_save_system, SIGNAL(triggered()), this, SLOT(action_parameters_save_system_slot()));
                QObject::connect(this->action_parameters_show_system, SIGNAL(triggered()), this, SLOT(action_parameters_show_system_slot()));
                QObject::connect(this->action_windows_show_potential_latitude, SIGNAL(triggered()), this, SLOT(action_windows_show_potential_latitude_slot()));
                QObject::connect(this->action_windows_show_potential_longitude, SIGNAL(triggered()), this, SLOT(action_windows_show_potential_longitude_slot()));
                QObject::connect(this->action_windows_show_tracked_latitude, SIGNAL(triggered()), this, SLOT(action_windows_show_tracked_latitude_slot()));
                QObject::connect(this->action_windows_show_tracked_longitude, SIGNAL(triggered()), this, SLOT(action_windows_show_tracked_longitude_slot()));
                QObject::connect(this->action_windows_show_tracked_sphere, SIGNAL(triggered()), this, SLOT(action_windows_show_tracked_sphere_slot()));
                QObject::connect(this->action_help_about, SIGNAL(triggered()), this, SLOT(action_help_about_slot()));

            // +-----------------------------------------------+
            // | Create menus                                  |
            // +-----------------------------------------------+

                this->menu_action = this->menuBar()->addMenu("&Action");
                this->menu_action->addAction(this->action_action_start);
                this->menu_action->addAction(this->action_action_stop);
                this->menu_action->addSeparator();
                this->menu_action->addAction(this->action_action_select);
                this->menu_action->addSeparator();
                this->menu_action->addAction(this->action_action_quit);
                this->menu_parameters = this->menuBar()->addMenu("&Parameters");
                this->menu_parameters->addAction(this->action_parameters_open_view);
                this->menu_parameters->addAction(this->action_parameters_save_view);
                this->menu_parameters->addAction(this->action_parameters_show_view);
                this->menu_parameters->addSeparator();
                this->menu_parameters->addAction(this->action_parameters_open_system);
                this->menu_parameters->addAction(this->action_parameters_save_system);
                this->menu_parameters->addAction(this->action_parameters_show_system);
                this->menu_windows = this->menuBar()->addMenu("&Windows");
                this->menu_windows->addAction(this->action_windows_show_potential_latitude);
                this->menu_windows->addAction(this->action_windows_show_potential_longitude);
                this->menu_windows->addAction(this->action_windows_show_tracked_latitude);
                this->menu_windows->addAction(this->action_windows_show_tracked_longitude);
                this->menu_windows->addAction(this->action_windows_show_tracked_sphere);
                this->menu_help = this->menuBar()->addMenu("&Help");
                this->menu_help->addAction(this->action_help_about);


    // +-------------------------------------------------------+
    // | Core                                                  |
    // +-------------------------------------------------------+

    // +-------------------------------------------------------+
    // | User output                                           |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Graphs                                            |
        // +---------------------------------------------------+

        // Create graphs

        this->graphPotentialLatitude = new Graph2D(Graph2D::LATITUDE, Graph2D::POTENTIAL);
        this->graphPotentialLongitude = new Graph2D(Graph2D::LONGITUDE, Graph2D::POTENTIAL);
        this->graphTrackedLatitude = new Graph2D(Graph2D::LATITUDE, Graph2D::TRACKED);
        this->graphTrackedLongitude = new Graph2D(Graph2D::LONGITUDE, Graph2D::TRACKED);
        this->graphTrackedSphere = new Graph3D(3,2,1,0.1);

        // Create subwindows

        int subWindowWidth = (windowWidth - leftDockWidth - rightDockWidth - emptyWidth) / 2;
        int subWindowHeight = (windowHeight) / 3;

        this->windowPotentialLatitude = new MdiSubWindow(this);
        this->windowPotentialLatitude->setWidget(this->graphPotentialLatitude);
        this->windowPotentialLatitude->setWindowTitle("Potential sources: Latitude");
        this->windowPotentialLatitude->setGeometry(0,0,subWindowWidth,subWindowHeight);
        this->mdiArea->addSubWindow(this->windowPotentialLatitude);
        this->windowPotentialLatitude->hide();

        this->windowPotentialLongitude = new MdiSubWindow(this);
        this->windowPotentialLongitude->setWidget(this->graphPotentialLongitude);
        this->windowPotentialLongitude->setWindowTitle("Potential sources: Longitude");
        this->windowPotentialLongitude->setGeometry(0,subWindowHeight,subWindowWidth,subWindowHeight);
        this->mdiArea->addSubWindow(this->windowPotentialLongitude);
        this->windowPotentialLongitude->hide();

        this->windowTrackedLatitude = new MdiSubWindow(this);
        this->windowTrackedLatitude->setWidget(this->graphTrackedLatitude);
        this->windowTrackedLatitude->setWindowTitle("Tracked sources: Latitude");
        this->windowTrackedLatitude->setGeometry(subWindowWidth,0,subWindowWidth,subWindowHeight);
        this->mdiArea->addSubWindow(this->windowTrackedLatitude);
        this->windowTrackedLatitude->hide();

        this->windowTrackedLongitude = new MdiSubWindow(this);
        this->windowTrackedLongitude->setWidget(this->graphTrackedLongitude);
        this->windowTrackedLongitude->setWindowTitle("Tracked sources: Longitude");
        this->windowTrackedLongitude->setGeometry(subWindowWidth,subWindowHeight,subWindowWidth,subWindowHeight);
        this->mdiArea->addSubWindow(this->windowTrackedLongitude);
        this->windowTrackedLongitude->hide();

        this->windowTrackedSphere = new MdiSubWindow(this);
        this->windowTrackedSphere->setWidget(this->graphTrackedSphere);
        this->windowTrackedSphere->setWindowTitle("Tracked sources: 3D Sphere");
        this->windowTrackedSphere->setGeometry(subWindowWidth,subWindowHeight*2,subWindowWidth,subWindowHeight);
        this->mdiArea->addSubWindow(this->windowTrackedSphere);
        this->windowTrackedSphere->hide();

        ///Stream Output Window Begin
        windowStreamOutput = new MdiSubWindow(this);
        windowStreamOutput->setWidget(new StreamOutputWidget(windowStreamOutput));
        windowStreamOutput->setWindowTitle("Stream Output Control");
        windowStreamOutput->setGeometry(subWindowWidth,subWindowHeight*2,subWindowWidth,subWindowHeight);
        mdiArea->addSubWindow(windowStreamOutput);
        windowStreamOutput->show();
        //Stream Output Window End


        QObject::connect(this->dockConfigSystem, SIGNAL(visibilityChanged(bool)), this, SLOT(dock_system_changes()));
        QObject::connect(this->dockConfigView, SIGNAL(visibilityChanged(bool)), this, SLOT(dock_view_changes()));
        QObject::connect(this->windowPotentialLatitude, SIGNAL(windowIsHidden()), this, SLOT(window_potential_latitude_changes()));
        QObject::connect(this->windowPotentialLongitude, SIGNAL(windowIsHidden()), this, SLOT(window_potential_longitude_changes()));
        QObject::connect(this->windowTrackedLatitude, SIGNAL(windowIsHidden()), this, SLOT(window_tracked_latitude_changes()));
        QObject::connect(this->windowTrackedLongitude, SIGNAL(windowIsHidden()), this, SLOT(window_tracked_longitude_changes()));
        QObject::connect(this->windowTrackedSphere, SIGNAL(windowIsHidden()), this, SLOT(window_tracked_sphere_changes()));

        // +---------------------------------------------------+
        // | Dialogs                                           |
        // +---------------------------------------------------+

        this->aboutDialog = new AboutDialog();

    // +-------------------------------------------------------+
    // | Set events                                            |
    // +-------------------------------------------------------+

        // Graphs -> source buffers

        this->graphPotentialLatitude->setResizeReceiver(this->potentialSourcesManager);
        this->graphPotentialLongitude->setResizeReceiver(this->potentialSourcesManager);
        this->graphTrackedLatitude->setResizeReceiver(this->trackedSourcesManager);
        this->graphTrackedLongitude->setResizeReceiver(this->trackedSourcesManager);

        // Source buffers -> graphs

        this->potentialSourcesManager->pushEventReceiver(this->graphPotentialLatitude);
        this->potentialSourcesManager->pushEventReceiver(this->graphPotentialLongitude);
        this->trackedSourcesManager->pushEventReceiver(this->graphTrackedLatitude);
        this->trackedSourcesManager->pushEventReceiver(this->graphTrackedLongitude);
        this->trackedSourcesManager->pushEventReceiver(this->graphTrackedSphere);

        // Property editors -> Parameters

        this->propertyEditorView->setEventReceiver(this->parametersManager);
        this->propertyEditorSystem->setEventReceiver(this->parametersManager);

        // View parameters -> Source buffers

        this->parametersManager->pushEventReceiverView(this->potentialSourcesManager);
        this->parametersManager->pushEventReceiverView(this->trackedSourcesManager);

        // System parameters -> Core, Potential source buffer (level of energy ET)
        this->parametersManager->pushEventReceiverSystem(this->runningThread);
        this->parametersManager->pushEventReceiverSystem(this->potentialSourcesManager);

        // Running thread -> Source buffers

        runningThread->pushPotentialSourceEventReceiver(this->potentialSourcesManager);
        runningThread->pushTrackedSourceEventReceiver(this->trackedSourcesManager);
        runningThread->pushSeparatedSourceEventReceiver(this->windowStreamOutput->widget());


        // Input/Output Manager -> Main Window
        this->inputOutputManager->pushEventReceiver(this);

    // +-------------------------------------------------------+
    // | Apply parameters                                      |
    // +-------------------------------------------------------+

    this->parametersManager->triggerParametersRefreshEventSystem();
    this->parametersManager->triggerParametersRefreshEventView();

    // +-------------------------------------------------------+
    // | Show the graphs at initial states                     |
    // +-------------------------------------------------------+

    QCoreApplication::postEvent(this->graphPotentialLatitude, new UpdateGraphRefreshEvent());
    QCoreApplication::postEvent(this->graphPotentialLongitude, new UpdateGraphRefreshEvent());
    QCoreApplication::postEvent(this->graphTrackedLatitude, new UpdateGraphRefreshEvent());
    QCoreApplication::postEvent(this->graphTrackedLongitude, new UpdateGraphRefreshEvent());

    // +-------------------------------------------------------+
    // | Menu states                                           |
    // +-------------------------------------------------------+

    this->action_action_start->setEnabled(true);
    this->action_action_stop->setEnabled(false);
    this->action_parameters_show_view->setCheckable(true);
    this->action_parameters_show_view->setChecked(true);
    this->action_parameters_show_system->setCheckable(true);
    this->action_parameters_show_system->setChecked(true);
    this->action_windows_show_potential_latitude->setCheckable(true);
    this->action_windows_show_potential_latitude->setChecked(true);
    this->action_windows_show_potential_longitude->setCheckable(true);
    this->action_windows_show_potential_longitude->setChecked(true);
    this->action_windows_show_tracked_latitude->setCheckable(true);
    this->action_windows_show_tracked_latitude->setChecked(true);
    this->action_windows_show_tracked_longitude->setCheckable(true);
    this->action_windows_show_tracked_longitude->setChecked(true);
    this->action_windows_show_tracked_sphere->setCheckable(true);
    this->action_windows_show_tracked_sphere->setChecked(true);

    // +-------------------------------------------------------+
    // | Show graphs                                           |
    // +-------------------------------------------------------+

    QCoreApplication::postEvent(windowPotentialLatitude, new ShowEvent());
    QCoreApplication::postEvent(windowPotentialLongitude, new ShowEvent());
    QCoreApplication::postEvent(windowTrackedLatitude, new ShowEvent());
    QCoreApplication::postEvent(windowTrackedLongitude, new ShowEvent());
    QCoreApplication::postEvent(windowTrackedSphere, new ShowEvent());

    // +-------------------------------------------------------+
    // | Startup                                               |
    // +-------------------------------------------------------+

    this->startupTimer.singleShot(1, this, SLOT(startup_showSplash()));

}

/***********************************************************
* Destructor                                               *
***********************************************************/

MDIWindow::~MDIWindow()
{

}

/***********************************************************
* Events                                                   *
***********************************************************/

bool MDIWindow::event(QEvent *event)
{
    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | The stream has stopped                        |
        // +-----------------------------------------------+

        EndOfStreamEvent *endOfStreamEvent = dynamic_cast<EndOfStreamEvent*>(event);

        if (endOfStreamEvent != NULL)
        {

            // Update menu
            this->action_action_start->setEnabled(true);
            this->action_action_stop->setEnabled(false);

        }

        return true;

    }

    return QMainWindow::event(event);

}

/***********************************************************
* Slots                                                    *
***********************************************************/

// +-------------------------------------------------------+
// | Actions                                               |
// +-------------------------------------------------------+

void MDIWindow::action_start_slot()
{

    // Clear graphs
    QCoreApplication::postEvent(this->potentialSourcesManager, new GraphClearEvent());
    QCoreApplication::postEvent(this->trackedSourcesManager, new GraphClearEvent());

    // Start thread
    this->runningThread->startThread();

    // Update menu
    this->action_action_start->setEnabled(false);
    this->action_action_stop->setEnabled(true);

}

void MDIWindow::action_stop_slot()
{

    // Stop thread
    this->runningThread->stopThread();

    // Update menu
    this->action_action_start->setEnabled(true);
    this->action_action_stop->setEnabled(false);

}

void MDIWindow::action_select_slot()
{

    this->inputOutputManager->askUser(false);

}

void MDIWindow::action_quit_slot()
{
    QCoreApplication::quit();
}

void MDIWindow::action_parameters_save_view_slot()
{

    QString fileName = QFileDialog::getSaveFileName(this, "Save parameters for the view...", "", "View parameters (*.mev);;All files (*.*)");

    if (fileName != "")
    {
        this->parametersManager->saveParametersView(fileName);
    }

}

void MDIWindow::action_parameters_open_view_slot()
{

    QString fileName = QFileDialog::getOpenFileName(this, "Open parameters for the view...", "", "View parameters (*.mev);;All files (*.*)");

    if (fileName != "")
    {
        this->parametersManager->openParametersView(fileName);
    }

    // Update values
    this->propertyEditorView->loadValues(this->parametersManager->copyParametersView());

    this->propertyEditorView->update();

}

void MDIWindow::action_parameters_show_view_slot()
{

    if (this->action_parameters_show_view->isChecked())
    {
        this->dockConfigView->show();
    }
    else
    {
        this->dockConfigView->hide();
    }

}

void MDIWindow::action_parameters_save_system_slot()
{

    QString fileName = QFileDialog::getSaveFileName(this, "Save parameters for the system...", "", "System parameters (*.mes);;All files (*.*)");

    if (fileName != "")
    {
        this->parametersManager->saveParametersSystem(fileName);
    }

}

void MDIWindow::action_parameters_open_system_slot()
{

    QString fileName = QFileDialog::getOpenFileName(this, "Open parameters for the system...", "", "System parameters (*.mes);;All files (*.*)");

    if (fileName != "")
    {
        this->parametersManager->openParametersSystem(fileName);
    }

    // Update values
    this->propertyEditorSystem->loadValues(this->parametersManager->copyParametersSystem());

    this->propertyEditorSystem->update();

}

void MDIWindow::action_parameters_show_system_slot()
{
    if (this->action_parameters_show_system->isChecked())
    {
        this->dockConfigSystem->show();
    }
    else
    {
        this->dockConfigSystem->hide();
    }
}

void MDIWindow::action_windows_show_potential_longitude_slot()
{

    if (this->action_windows_show_potential_longitude->isChecked())
    {
        this->windowPotentialLongitude->show();
    }
    else
    {
        this->windowPotentialLongitude->hide();
    }

}

void MDIWindow::action_windows_show_potential_latitude_slot()
{

    if (this->action_windows_show_potential_latitude->isChecked())
    {
        this->windowPotentialLatitude->show();
    }
    else
    {
        this->windowPotentialLatitude->hide();
    }


}

void MDIWindow::action_windows_show_tracked_longitude_slot()
{

    if (this->action_windows_show_tracked_longitude->isChecked())
    {
        this->windowTrackedLongitude->show();
    }
    else
    {
        this->windowTrackedLongitude->hide();
    }

}

void MDIWindow::action_windows_show_tracked_latitude_slot()
{

    if (this->action_windows_show_tracked_latitude->isChecked())
    {
        this->windowTrackedLatitude->show();
    }
    else
    {
        this->windowTrackedLatitude->hide();
    }

}

void MDIWindow::action_windows_show_tracked_sphere_slot()
{

    if (this->action_windows_show_tracked_sphere->isChecked())
    {
        this->windowTrackedSphere->show();
    }
    else
    {
        this->windowTrackedSphere->hide();
    }

}

void MDIWindow::action_help_about_slot()
{
    this->aboutDialog->show();
}

void MDIWindow::dock_view_changes()
{
    this->action_parameters_show_view->setChecked(this->dockConfigView->isVisible());
}

void MDIWindow::dock_system_changes()
{
    this->action_parameters_show_system->setChecked(this->dockConfigSystem->isVisible());
}

void MDIWindow::window_potential_longitude_changes()
{
    this->action_windows_show_potential_longitude->setChecked(this->windowPotentialLongitude->isVisible());
}

void MDIWindow::window_potential_latitude_changes()
{
    this->action_windows_show_potential_latitude->setChecked(this->windowPotentialLatitude->isVisible());
}

void MDIWindow::window_tracked_longitude_changes()
{
    this->action_windows_show_tracked_longitude->setChecked(this->windowTrackedLongitude->isVisible());
}

void MDIWindow::window_tracked_latitude_changes()
{
    this->action_windows_show_tracked_latitude->setChecked(this->windowTrackedLatitude->isVisible());
}

void MDIWindow::window_tracked_sphere_changes()
{
    this->action_windows_show_tracked_sphere->setChecked(this->windowTrackedSphere->isVisible());
}

void MDIWindow::startup_showSplash()
{   

    QPixmap* splashScreenImage;
    splashScreenImage = new QPixmap(":/Images/logo5_bgwhite.png");

    this->splashScreen = new QSplashScreen(*splashScreenImage);

    this->splashScreen->show();

    this->startupTimer.singleShot(1000, this, SLOT(startup_showMain()));
}

void MDIWindow::startup_showMain()
{

    this->splashScreen->close();
    this->show();
    //We do not need a valid selection at startup...
    this->inputOutputManager->askUser(false);

}
