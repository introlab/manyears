#ifndef MDIWINDOW_H
#define MDIWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDockWidget>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QPixmap>
#include <QSplashScreen>

#include "potentialSourceManager.h"
#include "trackedSourceManager.h"
#include "graph2D.h"
#include "parametersManager.h"
#include "propertyEditor.h"
#include "graph3D.h"
#include "mdiSubWindow.h"
#include "aboutDialog.h"
#include "coreThread.h"

class MDIWindow: public QMainWindow
{

    Q_OBJECT

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    MDIWindow();

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~MDIWindow();

protected:

    /***********************************************************
    * Events                                                   *
    ***********************************************************/

    bool event(QEvent *event);

private slots:

    /***********************************************************
    * Slots                                                    *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Actions                                               |
    // +-------------------------------------------------------+

    void action_start_slot();
    void action_stop_slot();
    void action_select_slot();
    void action_quit_slot();
    void action_parameters_save_view_slot();
    void action_parameters_open_view_slot();
    void action_parameters_show_view_slot();
    void action_parameters_save_system_slot();
    void action_parameters_open_system_slot();
    void action_parameters_show_system_slot();
    void action_windows_show_potential_longitude_slot();
    void action_windows_show_potential_latitude_slot();
    void action_windows_show_tracked_longitude_slot();
    void action_windows_show_tracked_latitude_slot();
    void action_windows_show_tracked_sphere_slot();
    void action_help_about_slot();

    void dock_view_changes();
    void dock_system_changes();
    void window_potential_longitude_changes();
    void window_potential_latitude_changes();
    void window_tracked_longitude_changes();
    void window_tracked_latitude_changes();
    void window_tracked_sphere_changes();

    void startup_showSplash();
    void startup_showMain();

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Running thread                                        |
    // +-------------------------------------------------------+

    CoreThread* runningThread;

    // +-------------------------------------------------------+
    // | Input/Output Manager                                  |
    // +-------------------------------------------------------+

    InputOutputManager* inputOutputManager;

    // +-------------------------------------------------------+
    // | General                                               |
    // +-------------------------------------------------------+

    QMdiArea* mdiArea;
    QSplashScreen* splashScreen;
    QTimer startupTimer;

    // +-------------------------------------------------------+
    // | User input                                            |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Parameters                                        |
        // +---------------------------------------------------+

        QDockWidget* dockConfigSystem;
        QDockWidget* dockConfigView;
        PropertyEditor* propertyEditorSystem;
        PropertyEditor* propertyEditorView;
        ParametersManager* parametersManager;
        Property* rootSystem;
        Property* rootView;

        // +---------------------------------------------------+
        // | Menu                                              |
        // +---------------------------------------------------+

        QMenu* menu_action;
        QMenu* menu_parameters;
        QMenu* menu_windows;
        QMenu* menu_help;

        QAction* action_action_start;
        QAction* action_action_stop;
        QAction* action_action_select;
        QAction* action_action_quit;
        QAction* action_parameters_open_view;
        QAction* action_parameters_save_view;
        QAction* action_parameters_open_system;
        QAction* action_parameters_save_system;
        QAction* action_parameters_show_view;
        QAction* action_parameters_show_system;
        QAction* action_windows_show_potential_longitude;
        QAction* action_windows_show_potential_latitude;
        QAction* action_windows_show_tracked_longitude;
        QAction* action_windows_show_tracked_latitude;
        QAction* action_windows_show_tracked_sphere;
        QAction* action_help_about;


    // +-------------------------------------------------------+
    // | Core                                                  |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Sources managers                                  |
        // +---------------------------------------------------+

        PotentialSourceBufferManager* potentialSourcesManager;
        TrackedSourceBufferManager* trackedSourcesManager;

    // +-------------------------------------------------------+
    // | User output                                           |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Graphs                                            |
        // +---------------------------------------------------+

        Graph2D* graphPotentialLatitude;
        Graph2D* graphPotentialLongitude;
        Graph3D* graphEnergySphere;
        Graph2D* graphTrackedLatitude;
        Graph2D* graphTrackedLongitude;
        Graph3D* graphTrackedSphere;

        MdiSubWindow* windowPotentialLatitude;
        MdiSubWindow* windowPotentialLongitude;
        MdiSubWindow* windowEnergySphere;
        MdiSubWindow* windowTrackedLatitude;
        MdiSubWindow* windowTrackedLongitude;
        MdiSubWindow* windowTrackedSphere;

        // +---------------------------------------------------+
        // | Dialogs                                           |
        // +---------------------------------------------------+

        AboutDialog* aboutDialog;

};

#endif
