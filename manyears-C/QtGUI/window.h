#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QDockWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDialog>
#include <QFileDialog>
#include <QStringList>

#include "potentialSourceManager.h"
#include "sourceManager.h"
#include "colorPalette.h"
#include "scrollListProperty.h"
#include "winLongitude.h"
#include "winLatitude.h"
#include "winSphere.h"
#include "winBeamformerLongitude.h"
#include "winBeamformerLatitude.h"
#include "soundInputConfig.h"
#include "parametersManager.h"

/***************************************************************
* File type                                                    *
***************************************************************/

#define     FILE_SYSTEM_TYPE        "mys"
#define     FILE_VIEW_TYPE          "myv"



class Window: public QMainWindow
{

    Q_OBJECT

public:

    /***********************************************************
    * Operation types                                          *
    ***********************************************************/

    enum USER_OPERATION { NOP , START , STOP , SYSTOPEN , SYSTSAVE , SYSTREFRESH , VIEWOPEN , VIEWSAVE , VIEWREFRESH };

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    Window();

    /***********************************************************
    * Initialization                                           *
    ***********************************************************/

    void init();

    /***********************************************************
    * Refresh                                                  *
    ***********************************************************/

    void loadWindow();

    PotentialSourceManager *getPotentialSourceManager();
    SourceManager *getSourceManager();

    void updateGraphs();
    void askForUpdateGraphs();

    void setViewLongitude(bool _enabled);
    void setViewLatitude(bool _enabled);
    void setViewSphere(bool _enabled);
    void setSystemConfig(bool _shown);
    void setViewConfig(bool _shown); 

    void setSystemRunning(bool _running);

    void promptSoundCard();
    void refreshSoundCard();

    void updateDocks();

    void refreshViewAppearance();
    void refreshSystemCore();

    /***********************************************************
    * Save / Open Files                                        *
    ***********************************************************/

    void saveSystemConfig();
    void openSystemConfig();
    void saveViewConfig();
    void openViewConfig();

    /***********************************************************
    * Default dimensions                                       *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Window                                                |
    // +-------------------------------------------------------+

    static const int windowWidth = 1000;
    static const int windowHeight = 1000;

signals:

    /***********************************************************
    * Signals                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Ask for a refresh                                     |
    // +-------------------------------------------------------+

    void refreshNeeded();

    // +-------------------------------------------------------+
    // | Start the system                                      |
    // +-------------------------------------------------------+

    void startCore();

    // +-------------------------------------------------------+
    // | Stop the system                                       |
    // +-------------------------------------------------------+

    void stopCore();

    // +-------------------------------------------------------+
    // | Confirm operation                                     |
    // +-------------------------------------------------------+

    void confirmOperation();

public slots:

    /***********************************************************
    * Slots                                                    *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Process the graphs refresh query                      |
    // +-------------------------------------------------------+

    void performRefresh();

    // +-------------------------------------------------------+
    // | Confirm the system is ready for operation             |
    // +-------------------------------------------------------+

    void confirmReadyOperation();

private slots:

    /***********************************************************
    * User interface                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Properties updated                                    |
    // +-------------------------------------------------------+

    void slotRefreshViewAppearance();
    void slotRefreshSystemCore();

    // +-------------------------------------------------------+
    // | Menubar                                               |
    // +-------------------------------------------------------+

    void menuSystemStartClicked();
    void menuSystemStopClicked();
    void menuSystemConfigurationToggled();
    void menuSystemConfigurationOpenClicked();
    void menuSystemConfigurationSaveClicked();
    void menuSystemQuitClicked();
    void menuViewLongitudeToggled();
    void menuViewLatitudeToggled();
    void menuViewSphereToggled();
    void menuViewConfigurationToggled();
    void menuViewConfigurationOpenClicked();
    void menuViewConfigurationSaveClicked();

private:

    /***********************************************************
    * User interface                                           *
    ***********************************************************/

    void triggerOperation(USER_OPERATION _operation);


    /***********************************************************
    * Widgets                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Menubar                                               |
    // +-------------------------------------------------------+

    QMenuBar *menuBar;
    QMenu *menuSystem;
    QAction *menuSystemStart;
    QAction *menuSystemStop;
    QAction *menuSystemConfiguration;
    QAction *menuSystemConfigurationOpen;
    QAction *menuSystemConfigurationSave;
    QAction *menuSystemQuit;
    QMenu *menuView;
    QAction *menuViewLongitude;
    QAction *menuViewLatitude;
    QAction *menuViewSphere;
    QAction *menuViewConfiguration;
    QAction *menuViewConfigurationOpen;
    QAction *menuViewConfigurationSave;

    // +-------------------------------------------------------+
    // | Dock                                                  |
    // +-------------------------------------------------------+

    QDockWidget *dockConfigSystem;
    QDockWidget *dockConfigView;
    ScrollListProperty *listPropertiesSystem;
    ScrollListProperty *listPropertiesView;

    // +-------------------------------------------------------+
    // | Windows                                               |
    // +-------------------------------------------------------+

    QWidget *dockLongitude;
    QWidget *dockLatitude;
    QWidget *dockSphere;
    SoundInputConfig *soundInputConfig;

    // +-------------------------------------------------------+
    // | MDI                                                   |
    // +-------------------------------------------------------+

    QMdiArea *mdiArea;
    WinLongitude *mdiWinLongitude;
    WinLatitude *mdiWinLatitude;
    //WinBeamformerLongitude *mdiWinBeamformerLongitude;
    //WinBeamformerLatitude *mdiWinBeamformerLatitude;
    WinSphere *mdiWinSphere;


    /***********************************************************
    * Status                                                   *
    ***********************************************************/

    bool viewLongitude;
    bool viewLatitude;
    bool viewSphere;
    bool showSystemConfig;
    bool showViewConfig;
    bool running;

    /***********************************************************
    * Source visualiation                                      *
    ***********************************************************/

    ColorPalette *colorPalette;
    PotentialSourceManager *potentialSourceManager;
    SourceManager *sourceManager;

    /***********************************************************
    * Parameters                                               *
    ***********************************************************/

    ParametersManager *parametersManager;

    /***********************************************************
    * User interface                                           *
    ***********************************************************/

    USER_OPERATION userOperation;


};

#endif // WINDOW_H
