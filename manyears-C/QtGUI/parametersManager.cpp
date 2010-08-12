#include "parametersManager.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

ParametersManager::ParametersManager()
{

}

ParametersManager::ParametersManager(const ParametersManager& _parametersManager)
{

    *this = _parametersManager;

}

/***********************************************************
* Destructor                                               *
***********************************************************/

ParametersManager::~ParametersManager()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

ParametersManager& ParametersManager::operator=(const ParametersManager& _parametersManager)
{

    // Copy fields

    this->systemParameters = _parametersManager.systemParameters;
    this->viewParameters = _parametersManager.viewParameters;

    this->eventReceiversSystem.clear();
    for (int indexList = 0; indexList < _parametersManager.eventReceiversSystem.size(); indexList++)
    {
        this->eventReceiversSystem.push_back(_parametersManager.eventReceiversSystem.at(indexList));
    }

    this->eventReceiversView.clear();
    for (int indexList = 0; indexList < _parametersManager.eventReceiversView.size(); indexList++)
    {
        this->eventReceiversView.push_back(_parametersManager.eventReceiversView.at(indexList));
    }

}

/***********************************************************
* Initialize to default values                             *
***********************************************************/

Property* ParametersManager::initView()
{

    // +-------------------------------------------------------+
    // | 1. View properties                                    |
    // +-------------------------------------------------------+

        Property* view = new Property("","","",Property::P_NOTYPE);

        // +---------------------------------------------------+
        // | A. Sphere                                         |
        // +---------------------------------------------------+

                Property* sphere = new Property("3D Sphere","","sphere",Property::P_NOTYPE);

                // SPHERE_BACKGROUND_COLOR
                Property* sphere_background = new Property("Background","Color of the sphere background","SPHERE_BACKGROUND_COLOR",Property::P_COLOR);

                // SPHERE_NORTH_COLOR
                Property* sphere_north = new Property("North hemisphere","Color of the north hemisphere mesh","SPHERE_NORTH_COLOR",Property::P_COLOR);

                // SPHERE_SOUTH_COLOR
                Property* sphere_south = new Property("South hemisphere","Color of the south hemisphere mesh","SPHERE_SOUTH_COLOR",Property::P_COLOR);

                // SPHERE_AXES_COLOR
                Property* sphere_axes = new Property("Axes","Color of the axes","SPHERE_AXES_COLOR",Property::P_COLOR);

                sphere->addChild(sphere_background);
                sphere->addChild(sphere_north);
                sphere->addChild(sphere_south);
                sphere->addChild(sphere_axes);

                view->addChild(sphere);

        // +---------------------------------------------------+
        // | B. Plots                                          |
        // +---------------------------------------------------+

                Property* plots = new Property("2D Plots","","plots",Property::P_NOTYPE);

                    // +---------------------------------------+
                    // | I. Potential                          |
                    // +---------------------------------------+

                        Property* plots_potential = new Property("Potential sources","","plots_potential",Property::P_NOTYPE);

                            // +---------------------------------------+
                            // | i. Longitude                          |
                            // +---------------------------------------+

                                Property* plots_potential_longitude = new Property("Longitude","","plots_potential_longitude",Property::P_NOTYPE);

                                    // POTENTIAL_LONGITUDE_BACKGROUND_COLOR
                                    Property* plots_potential_longitude_background = new Property("Background","Color of the background of the longitude plot","POTENTIAL_LONGITUDE_BACKGROUND_COLOR",Property::P_COLOR);

                                    // POTENTIAL_LONGITUDE_TRACE_SIZE
                                    Property* plots_potential_longitude_tracesize = new Property("Trace size","Size of the trace for each source","POTENTIAL_LONGITUDE_TRACE_SIZE",Property::P_INT_POS);

                                    // +-------------------------------+
                                    // | a. Axes                       |
                                    // +-------------------------------+

                                        Property* plots_potential_longitude_axes = new Property("Axes","","plots_longitude_axes",Property::P_NOTYPE);

                                            // POTENTIAL_LONGITUDE_HORIZONTALAXES_COLOR
                                            Property* plots_potential_longitude_axes_hcolor = new Property("Horizontal axis color","Color of the horizontal axis for the longitude plot","POTENTIAL_LONGITUDE_HORIZONTALAXES_COLOR",Property::P_COLOR);

                                            // POTENTIAL_LONGITUDE_VERTICALAXES_COLOR
                                            Property* plots_potential_longitude_axes_vcolor = new Property("Vertical axis color","Color of the vertical axis for the longitude plot","POTENTIAL_LONGITUDE_VERTICALAXES_COLOR",Property::P_COLOR);

                                        plots_potential_longitude_axes->addChild(plots_potential_longitude_axes_hcolor);
                                        plots_potential_longitude_axes->addChild(plots_potential_longitude_axes_vcolor);

                                    plots_potential_longitude->addChild(plots_potential_longitude_background);
                                    plots_potential_longitude->addChild(plots_potential_longitude_axes);
                                    plots_potential_longitude->addChild(plots_potential_longitude_tracesize);

                            // +---------------------------------------+
                            // | ii. Latitude                          |
                            // +---------------------------------------+

                                Property* plots_potential_latitude = new Property("Latitude","","plots_latitude",Property::P_NOTYPE);

                                    // POTENTIAL_LATITUDE_BACKGROUND_COLOR
                                    Property* plots_potential_latitude_background = new Property("Background","Color of the background of the latitude plot","POTENTIAL_LATITUDE_BACKGROUND_COLOR",Property::P_COLOR);

                                    // POTENTIAL_LATITUDE_TRACE_SIZE
                                    Property* plots_potential_latitude_tracesize = new Property("Trace size","Size of the trace for each source","POTENTIAL_LATITUDE_TRACE_SIZE",Property::P_INT_POS);

                                    // +-------------------------------+
                                    // | a. Axes                       |
                                    // +-------------------------------+

                                        Property* plots_potential_latitude_axes = new Property("Axes","","plots_latitude_axes",Property::P_NOTYPE);

                                            // POTENTIAL_LATITUDE_HORIZONTALAXES_COLOR
                                            Property* plots_potential_latitude_axes_hcolor = new Property("Horizontal axis color","Color of the horizontal axis for the latitude plot","POTENTIAL_LATITUDE_HORIZONTALAXES_COLOR",Property::P_COLOR);

                                            // POTENTIAL_LATITUDE_VERTICALAXES_COLOR
                                            Property* plots_potential_latitude_axes_vcolor = new Property("Vertical axis color","Color of the vertical axis for the latitude plot","POTENTIAL_LATITUDE_VERTICALAXES_COLOR",Property::P_COLOR);

                                        plots_potential_latitude_axes->addChild(plots_potential_latitude_axes_hcolor);
                                        plots_potential_latitude_axes->addChild(plots_potential_latitude_axes_vcolor);

                                    plots_potential_latitude->addChild(plots_potential_latitude_background);
                                    plots_potential_latitude->addChild(plots_potential_latitude_axes);
                                    plots_potential_latitude->addChild(plots_potential_latitude_tracesize);

                            plots_potential->addChild(plots_potential_longitude);
                            plots_potential->addChild(plots_potential_latitude);

                        plots->addChild(plots_potential);

                        // +---------------------------------------+
                        // | II. Tracked                           |
                        // +---------------------------------------+

                            Property* plots_tracked = new Property("Tracked sources","","plots_tracked",Property::P_NOTYPE);

                                // +---------------------------------------+
                                // | i. Longitude                          |
                                // +---------------------------------------+

                                    Property* plots_tracked_longitude = new Property("Longitude","","plots_tracked_longitude",Property::P_NOTYPE);

                                        // TRACKED_LONGITUDE_BACKGROUND_COLOR
                                        Property* plots_tracked_longitude_background = new Property("Background","Color of the background of the longitude plot","TRACKED_LONGITUDE_BACKGROUND_COLOR",Property::P_COLOR);

                                        // TRACKED_LONGITUDE_TRACE_SIZE
                                        Property* plots_tracked_longitude_tracesize = new Property("Trace size","Size of the trace for each source","TRACKED_LONGITUDE_TRACE_SIZE",Property::P_INT_POS);

                                        // +-------------------------------+
                                        // | a. Axes                       |
                                        // +-------------------------------+

                                            Property* plots_tracked_longitude_axes = new Property("Axes","","plots_longitude_axes",Property::P_NOTYPE);

                                                // TRACKED_LONGITUDE_HORIZONTALAXES_COLOR
                                                Property* plots_tracked_longitude_axes_hcolor = new Property("Horizontal axis color","Color of the horizontal axis for the longitude plot","TRACKED_LONGITUDE_HORIZONTALAXES_COLOR",Property::P_COLOR);

                                                // TRACKED_LONGITUDE_VERTICALAXES_COLOR
                                                Property* plots_tracked_longitude_axes_vcolor = new Property("Vertical axis color","Color of the vertical axis for the longitude plot","TRACKED_LONGITUDE_VERTICALAXES_COLOR",Property::P_COLOR);

                                            plots_tracked_longitude_axes->addChild(plots_tracked_longitude_axes_hcolor);
                                            plots_tracked_longitude_axes->addChild(plots_tracked_longitude_axes_vcolor);


                                        plots_tracked_longitude->addChild(plots_tracked_longitude_background);
                                        plots_tracked_longitude->addChild(plots_tracked_longitude_axes);
                                        plots_tracked_longitude->addChild(plots_tracked_longitude_tracesize);

                                // +---------------------------------------+
                                // | ii. Latitude                          |
                                // +---------------------------------------+

                                    Property* plots_tracked_latitude = new Property("Latitude","","plots_latitude",Property::P_NOTYPE);

                                        // TRACKED_LATITUDE_BACKGROUND_COLOR
                                        Property* plots_tracked_latitude_background = new Property("Background","Color of the background of the latitude plot","TRACKED_LATITUDE_BACKGROUND_COLOR",Property::P_COLOR);

                                        // TRACKED_LATITUDE_TRACE_SIZE
                                        Property* plots_tracked_latitude_tracesize = new Property("Trace size","Size of the trace for each source","TRACKED_LATITUDE_TRACE_SIZE",Property::P_INT_POS);

                                        // +-------------------------------+
                                        // | a. Axes                       |
                                        // +-------------------------------+

                                            Property* plots_tracked_latitude_axes = new Property("Axes","","plots_latitude_axes",Property::P_NOTYPE);

                                                // TRACKED_LATITUDE_HORIZONTALAXES_COLOR
                                                Property* plots_tracked_latitude_axes_hcolor = new Property("Horizontal axis color","Color of the horizontal axis for the latitude plot","TRACKED_LATITUDE_HORIZONTALAXES_COLOR",Property::P_COLOR);

                                                // TRACKED_LATITUDE_VERTICALAXES_COLOR
                                                Property* plots_tracked_latitude_axes_vcolor = new Property("Vertical axis color","Color of the vertical axis for the latitude plot","TRACKED_LATITUDE_VERTICALAXES_COLOR",Property::P_COLOR);

                                            plots_tracked_latitude_axes->addChild(plots_tracked_latitude_axes_hcolor);
                                            plots_tracked_latitude_axes->addChild(plots_tracked_latitude_axes_vcolor);

                                        plots_tracked_latitude->addChild(plots_tracked_latitude_background);
                                        plots_tracked_latitude->addChild(plots_tracked_latitude_axes);
                                        plots_tracked_latitude->addChild(plots_tracked_latitude_tracesize);

                            plots_tracked->addChild(plots_tracked_longitude);
                            plots_tracked->addChild(plots_tracked_latitude);

                        plots->addChild(plots_tracked);

                        // +---------------------------------------+
                        // | III. Time scale                       |
                        // +---------------------------------------+

                            Property* plots_timescale = new Property("Time scaling","","plots_timescale",Property::P_NOTYPE);

                                // TIME_BUFFER
                                Property* plots_timescale_xmin = new Property("Time in buffer","Amount of time for which the sources trace is shown","TIME_BUFFER",Property::P_FLOAT_POS);

                                // TIME_AFTER
                                Property* plots_timescale_xmax = new Property("Time before source","Amount of time before the sources trace is shown","TIME_AFTER",Property::P_FLOAT_POS);

                                // TIME_INTERVAL
                                Property* plots_timescale_xint = new Property("Time interval","Amount of time between each interval","TIME_INTERVAL",Property::P_FLOAT_POS);

                            plots_timescale->addChild(plots_timescale_xmin);
                            plots_timescale->addChild(plots_timescale_xmax);
                            plots_timescale->addChild(plots_timescale_xint);

                        plots->addChild(plots_timescale);

                    view->addChild(plots);

        // +---------------------------------------------------+
        // | C. Sources                                        |
        // +---------------------------------------------------+

                Property* sources = new Property("Sources","","sources",Property::P_NOTYPE);

                    // SOURCE1_COLOR
                    Property* sources_1 = new Property("Source 1","Color of source 1","SOURCE1_COLOR",Property::P_COLOR);

                    // SOURCE2_COLOR
                    Property* sources_2 = new Property("Source 2","Color of source 2","SOURCE2_COLOR",Property::P_COLOR);

                    // SOURCE3_COLOR
                    Property* sources_3 = new Property("Source 3","Color of source 3","SOURCE3_COLOR",Property::P_COLOR);

                    // SOURCE4_COLOR
                    Property* sources_4 = new Property("Source 4","Color of source 4","SOURCE4_COLOR",Property::P_COLOR);

                    // SOURCE5_COLOR
                    Property* sources_5 = new Property("Source 5","Color of source 5","SOURCE5_COLOR",Property::P_COLOR);

                    // SOURCE6_COLOR
                    Property* sources_6 = new Property("Source 6","Color of source 6","SOURCE6_COLOR",Property::P_COLOR);

                    // SOURCE7_COLOR
                    Property* sources_7 = new Property("Source 7","Color of source 7","SOURCE7_COLOR",Property::P_COLOR);

                    // SOURCE8_COLOR
                    Property* sources_8 = new Property("Source 8","Color of source 8","SOURCE8_COLOR",Property::P_COLOR);

                    // SOURCE9_COLOR
                    Property* sources_9 = new Property("Source 9","Color of source 9","SOURCE9_COLOR",Property::P_COLOR);

                    // SOURCE10_COLOR
                    Property* sources_10 = new Property("Source 10","Color of source 10","SOURCE10_COLOR",Property::P_COLOR);

                    sources->addChild(sources_1);
                    sources->addChild(sources_2);
                    sources->addChild(sources_3);
                    sources->addChild(sources_4);
                    sources->addChild(sources_5);
                    sources->addChild(sources_6);
                    sources->addChild(sources_7);
                    sources->addChild(sources_8);
                    sources->addChild(sources_9);
                    sources->addChild(sources_10);

                view->addChild(sources);

    return view;

}

Property* ParametersManager::initSystem()
{

    // +-------------------------------------------------------+
    // | 2. Core properties                                    |
    // +-------------------------------------------------------+

    Property* system = new Property("System","","",Property::P_NOTYPE);

        // +---------------------------------------------------+
        // | A. Beamformer                                     |
        // +---------------------------------------------------+

                Property* beamformer = new Property("Beamformer","","beamformer",Property::P_NOTYPE);

                // +-------------------------------------------+
                // | I. Microphones                            |
                // +-------------------------------------------+

                        Property* beamformer_mics = new Property("Microphones","","beamformer_mics",Property::P_NOTYPE);

                        // +-----------------------------------+
                        // | i. Microphone 1                   |
                        // +-----------------------------------+

                                Property* beamformer_mics_mic1 = new Property("Microphone 1","","beamformer_mics_mic1",Property::P_NOTYPE);

                                    // BEAMFORMER_MIC1_GAIN
                                    Property* beamformer_mics_mic1_gain = new Property("Gain","Gain of microphone 1","BEAMFORMER_MIC1_GAIN",Property::P_FLOAT_POS);

                                    beamformer_mics_mic1->addChild(beamformer_mics_mic1_gain);

                                    // +-----------------------+
                                    // | a. Position           |
                                    // +-----------------------+

                                        Property* beamformer_mics_mic1_pos = new Property("Position","","beamformer_mics_mic1_pos",Property::P_NOTYPE);

                                            // BEAMFORMER_MIC1_X
                                            Property* beamformer_mics_mic1_pos_x = new Property("x","Position of microphone 1 on x-axis (in meter)","BEAMFORMER_MIC1_X",Property::P_FLOAT);

                                            // BEAMFORMER_MIC1_Y
                                            Property* beamformer_mics_mic1_pos_y = new Property("y","Position of microphone 1 on y-axis (in meter)","BEAMFORMER_MIC1_Y",Property::P_FLOAT);

                                            // BEAMFORMER_MIC1_Z
                                            Property* beamformer_mics_mic1_pos_z = new Property("z","Position of microphone 1 on z-axis (in meter)","BEAMFORMER_MIC1_Z",Property::P_FLOAT);

                                        beamformer_mics_mic1_pos->addChild(beamformer_mics_mic1_pos_x);
                                        beamformer_mics_mic1_pos->addChild(beamformer_mics_mic1_pos_y);
                                        beamformer_mics_mic1_pos->addChild(beamformer_mics_mic1_pos_z);

                                    beamformer_mics_mic1->addChild(beamformer_mics_mic1_pos);

                        // +-----------------------------------+
                        // | ii. Microphone 2                  |
                        // +-----------------------------------+

                                Property* beamformer_mics_mic2 = new Property("Microphone 2","","beamformer_mics_mic2",Property::P_NOTYPE);

                                    // BEAMFORMER_MIC2_GAIN
                                    Property* beamformer_mics_mic2_gain = new Property("Gain","Gain of microphone 2","BEAMFORMER_MIC2_GAIN",Property::P_FLOAT_POS);

                                    beamformer_mics_mic2->addChild(beamformer_mics_mic2_gain);

                                    // +-----------------------+
                                    // | a. Position           |
                                    // +-----------------------+

                                        Property* beamformer_mics_mic2_pos = new Property("Position","","beamformer_mics_mic2_pos",Property::P_NOTYPE);

                                            // BEAMFORMER_MIC2_X
                                            Property* beamformer_mics_mic2_pos_x = new Property("x","Position of microphone 2 on x-axis (in meter)","BEAMFORMER_MIC2_X",Property::P_FLOAT);

                                            // BEAMFORMER_MIC2_Y
                                            Property* beamformer_mics_mic2_pos_y = new Property("y","Position of microphone 2 on y-axis (in meter)","BEAMFORMER_MIC2_Y",Property::P_FLOAT);

                                            // BEAMFORMER_MIC2_Z
                                            Property* beamformer_mics_mic2_pos_z = new Property("z","Position of microphone 2 on z-axis (in meter)","BEAMFORMER_MIC2_Z",Property::P_FLOAT);

                                            beamformer_mics_mic2_pos->addChild(beamformer_mics_mic2_pos_x);
                                            beamformer_mics_mic2_pos->addChild(beamformer_mics_mic2_pos_y);
                                            beamformer_mics_mic2_pos->addChild(beamformer_mics_mic2_pos_z);

                                        beamformer_mics_mic2->addChild(beamformer_mics_mic2_pos);

                        // +-----------------------------------+
                        // | iii. Microphone 3                 |
                        // +-----------------------------------+

                                Property* beamformer_mics_mic3 = new Property("Microphone 3","","beamformer_mics_mic3",Property::P_NOTYPE);

                                    // BEAMFORMER_MIC3_GAIN
                                    Property* beamformer_mics_mic3_gain = new Property("Gain","Gain of microphone 3","BEAMFORMER_MIC3_GAIN",Property::P_FLOAT_POS);

                                    beamformer_mics_mic3->addChild(beamformer_mics_mic3_gain);

                                    // +-----------------------+
                                    // | a. Position           |
                                    // +-----------------------+

                                        Property* beamformer_mics_mic3_pos = new Property("Position","","beamformer_mics_mic3_pos",Property::P_NOTYPE);

                                            // BEAMFORMER_MIC3_X
                                            Property* beamformer_mics_mic3_pos_x = new Property("x","Position of microphone 3 on x-axis (in meter)","BEAMFORMER_MIC3_X",Property::P_FLOAT);

                                            // BEAMFORMER_MIC3_Y
                                            Property* beamformer_mics_mic3_pos_y = new Property("y","Position of microphone 3 on y-axis (in meter)","BEAMFORMER_MIC3_Y",Property::P_FLOAT);

                                            // BEAMFORMER_MIC3_Z
                                            Property* beamformer_mics_mic3_pos_z = new Property("z","Position of microphone 3 on z-axis (in meter)","BEAMFORMER_MIC3_Z",Property::P_FLOAT);

                                            beamformer_mics_mic3_pos->addChild(beamformer_mics_mic3_pos_x);
                                            beamformer_mics_mic3_pos->addChild(beamformer_mics_mic3_pos_y);
                                            beamformer_mics_mic3_pos->addChild(beamformer_mics_mic3_pos_z);

                                        beamformer_mics_mic3->addChild(beamformer_mics_mic3_pos);

                        // +-----------------------------------+
                        // | iv. Microphone 4                  |
                        // +-----------------------------------+

                                Property* beamformer_mics_mic4 = new Property("Microphone 4","","beamformer_mics_mic4",Property::P_NOTYPE);

                                    // BEAMFORMER_MIC4_GAIN
                                    Property* beamformer_mics_mic4_gain = new Property("Gain","Gain of microphone 4","BEAMFORMER_MIC4_GAIN",Property::P_FLOAT_POS);

                                    beamformer_mics_mic4->addChild(beamformer_mics_mic4_gain);

                                    // +-----------------------+
                                    // | a. Position           |
                                    // +-----------------------+

                                        Property* beamformer_mics_mic4_pos = new Property("Position","","beamformer_mics_mic4_pos",Property::P_NOTYPE);

                                            // BEAMFORMER_MIC4_X
                                            Property* beamformer_mics_mic4_pos_x = new Property("x","Position of microphone 4 on x-axis (in meter)","BEAMFORMER_MIC4_X",Property::P_FLOAT);

                                            // BEAMFORMER_MIC4_Y
                                            Property* beamformer_mics_mic4_pos_y = new Property("y","Position of microphone 4 on y-axis (in meter)","BEAMFORMER_MIC4_Y",Property::P_FLOAT);

                                            // BEAMFORMER_MIC4_Z
                                            Property* beamformer_mics_mic4_pos_z = new Property("z","Position of microphone 4 on z-axis (in meter)","BEAMFORMER_MIC4_Z",Property::P_FLOAT);

                                            beamformer_mics_mic4_pos->addChild(beamformer_mics_mic4_pos_x);
                                            beamformer_mics_mic4_pos->addChild(beamformer_mics_mic4_pos_y);
                                            beamformer_mics_mic4_pos->addChild(beamformer_mics_mic4_pos_z);

                                        beamformer_mics_mic4->addChild(beamformer_mics_mic4_pos);

                        // +-----------------------------------+
                        // | v. Microphone 5                   |
                        // +-----------------------------------+

                                Property* beamformer_mics_mic5 = new Property("Microphone 5","","beamformer_mics_mic5",Property::P_NOTYPE);

                                    // BEAMFORMER_MIC5_GAIN
                                    Property* beamformer_mics_mic5_gain = new Property("Gain","Gain of microphone 5","BEAMFORMER_MIC5_GAIN",Property::P_FLOAT_POS);

                                    beamformer_mics_mic5->addChild(beamformer_mics_mic5_gain);

                                    // +-----------------------+
                                    // | a. Position           |
                                    // +-----------------------+

                                        Property* beamformer_mics_mic5_pos = new Property("Position","","beamformer_mics_mic5_pos",Property::P_NOTYPE);

                                            // BEAMFORMER_MIC5_X
                                            Property* beamformer_mics_mic5_pos_x = new Property("x","Position of microphone 5 on x-axis (in meter)","BEAMFORMER_MIC5_X",Property::P_FLOAT);

                                            // BEAMFORMER_MIC5_Y
                                            Property* beamformer_mics_mic5_pos_y = new Property("y","Position of microphone 5 on y-axis (in meter)","BEAMFORMER_MIC5_Y",Property::P_FLOAT);

                                            // BEAMFORMER_MIC5_Z
                                            Property* beamformer_mics_mic5_pos_z = new Property("z","Position of microphone 5 on z-axis (in meter)","BEAMFORMER_MIC5_Z",Property::P_FLOAT);

                                            beamformer_mics_mic5_pos->addChild(beamformer_mics_mic5_pos_x);
                                            beamformer_mics_mic5_pos->addChild(beamformer_mics_mic5_pos_y);
                                            beamformer_mics_mic5_pos->addChild(beamformer_mics_mic5_pos_z);

                                        beamformer_mics_mic5->addChild(beamformer_mics_mic5_pos);

                        // +-----------------------------------+
                        // | vi. Microphone 6                  |
                        // +-----------------------------------+

                                Property* beamformer_mics_mic6 = new Property("Microphone 6","","beamformer_mics_mic6",Property::P_NOTYPE);

                                    // BEAMFORMER_MIC6_GAIN
                                    Property* beamformer_mics_mic6_gain = new Property("Gain","Gain of microphone 6","BEAMFORMER_MIC6_GAIN",Property::P_FLOAT_POS);

                                    beamformer_mics_mic6->addChild(beamformer_mics_mic6_gain);

                                    // +-----------------------+
                                    // | a. Position           |
                                    // +-----------------------+

                                        Property* beamformer_mics_mic6_pos = new Property("Position","","beamformer_mics_mic6_pos",Property::P_NOTYPE);

                                            // BEAMFORMER_MIC6_X
                                            Property* beamformer_mics_mic6_pos_x = new Property("x","Position of microphone 6 on x-axis (in meter)","BEAMFORMER_MIC6_X",Property::P_FLOAT);

                                            // BEAMFORMER_MIC6_Y
                                            Property* beamformer_mics_mic6_pos_y = new Property("y","Position of microphone 6 on y-axis (in meter)","BEAMFORMER_MIC6_Y",Property::P_FLOAT);

                                            // BEAMFORMER_MIC6_Z
                                            Property* beamformer_mics_mic6_pos_z = new Property("z","Position of microphone 6 on z-axis (in meter)","BEAMFORMER_MIC6_Z",Property::P_FLOAT);

                                            beamformer_mics_mic6_pos->addChild(beamformer_mics_mic6_pos_x);
                                            beamformer_mics_mic6_pos->addChild(beamformer_mics_mic6_pos_y);
                                            beamformer_mics_mic6_pos->addChild(beamformer_mics_mic6_pos_z);

                                        beamformer_mics_mic6->addChild(beamformer_mics_mic6_pos);

                        // +-----------------------------------+
                        // | vii. Microphone 7                 |
                        // +-----------------------------------+

                                Property* beamformer_mics_mic7 = new Property("Microphone 7","","beamformer_mics_mic7",Property::P_NOTYPE);

                                    // BEAMFORMER_MIC7_GAIN
                                    Property* beamformer_mics_mic7_gain = new Property("Gain","Gain of microphone 7","BEAMFORMER_MIC7_GAIN",Property::P_FLOAT_POS);

                                    beamformer_mics_mic7->addChild(beamformer_mics_mic7_gain);

                                    // +-----------------------+
                                    // | a. Position           |
                                    // +-----------------------+

                                        Property* beamformer_mics_mic7_pos = new Property("Position","","beamformer_mics_mic7_pos",Property::P_NOTYPE);

                                            // BEAMFORMER_MIC7_X
                                            Property* beamformer_mics_mic7_pos_x = new Property("x","Position of microphone 7 on x-axis (in meter)","BEAMFORMER_MIC7_X",Property::P_FLOAT);

                                            // BEAMFORMER_MIC7_Y
                                            Property* beamformer_mics_mic7_pos_y = new Property("y","Position of microphone 7 on y-axis (in meter)","BEAMFORMER_MIC7_Y",Property::P_FLOAT);

                                            // BEAMFORMER_MIC7_Z
                                            Property* beamformer_mics_mic7_pos_z = new Property("z","Position of microphone 7 on z-axis (in meter)","BEAMFORMER_MIC7_Z",Property::P_FLOAT);

                                            beamformer_mics_mic7_pos->addChild(beamformer_mics_mic7_pos_x);
                                            beamformer_mics_mic7_pos->addChild(beamformer_mics_mic7_pos_y);
                                            beamformer_mics_mic7_pos->addChild(beamformer_mics_mic7_pos_z);

                                        beamformer_mics_mic7->addChild(beamformer_mics_mic7_pos);

                        // +-----------------------------------+
                        // | viii. Microphone 8                |
                        // +-----------------------------------+

                                Property* beamformer_mics_mic8 = new Property("Microphone 8","","beamformer_mics_mic8",Property::P_NOTYPE);

                                    // BEAMFORMER_MIC8_GAIN
                                    Property* beamformer_mics_mic8_gain = new Property("Gain","Gain of microphone 8","BEAMFORMER_MIC8_GAIN",Property::P_FLOAT_POS);

                                    beamformer_mics_mic8->addChild(beamformer_mics_mic8_gain);

                                    // +-----------------------+
                                    // | a. Position           |
                                    // +-----------------------+

                                        Property* beamformer_mics_mic8_pos = new Property("Position","","beamformer_mics_mic8_pos",Property::P_NOTYPE);

                                            // BEAMFORMER_MIC8_X
                                            Property* beamformer_mics_mic8_pos_x = new Property("x","Position of microphone 8 on x-axis (in meter)","BEAMFORMER_MIC8_X",Property::P_FLOAT);

                                            // BEAMFORMER_MIC8_Y
                                            Property* beamformer_mics_mic8_pos_y = new Property("y","Position of microphone 8 on y-axis (in meter)","BEAMFORMER_MIC8_Y",Property::P_FLOAT);

                                            // BEAMFORMER_MIC8_Z
                                            Property* beamformer_mics_mic8_pos_z = new Property("z","Position of microphone 8 on z-axis (in meter)","BEAMFORMER_MIC8_Z",Property::P_FLOAT);

                                            beamformer_mics_mic8_pos->addChild(beamformer_mics_mic8_pos_x);
                                            beamformer_mics_mic8_pos->addChild(beamformer_mics_mic8_pos_y);
                                            beamformer_mics_mic8_pos->addChild(beamformer_mics_mic8_pos_z);

                                        beamformer_mics_mic8->addChild(beamformer_mics_mic8_pos);

                        beamformer_mics->addChild(beamformer_mics_mic1);
                        beamformer_mics->addChild(beamformer_mics_mic2);
                        beamformer_mics->addChild(beamformer_mics_mic3);
                        beamformer_mics->addChild(beamformer_mics_mic4);
                        beamformer_mics->addChild(beamformer_mics_mic5);
                        beamformer_mics->addChild(beamformer_mics_mic6);
                        beamformer_mics->addChild(beamformer_mics_mic7);
                        beamformer_mics->addChild(beamformer_mics_mic8);

                // +-------------------------------------------+
                // | II. Potential sources                     |
                // +-------------------------------------------+

                        // BEAMFORMER_MAXPOTENTIAL
                        Property* beamformer_maxsources = new Property("Max number of potential sources","Maximum number of potential sources","BEAMFORMER_MAXPOTENTIAL",Property::P_INT_POS);

                        // BEAMFORMER_NUMBERMAX
                        Property* beamformer_numbermax = new Property("Number of maxima kept in memory","Number of maxima to keep in memory in order to find the one that best fits","BEAMFORMER_NUMBERMAX",Property::P_INT_POS);

                        // BEAMFORMER_MAXTHRESHOLD
                        Property* beamformer_maxthres = new Property("Threshold for maxima","Threshold for maxima that are considered relative to the maximum","BEAMFORMER_MAXTHRESHOLD",Property::P_FLOAT_POS);

                        // BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL
                        Property* beamformer_angleh = new Property("Horizontal angle tolerance (first)","Horizontal angle tolerance for the first source (rad)","BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL",Property::P_FLOAT_POS);

                        // BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL_NEXT
                        Property* beamformer_anglehnext = new Property("Horizontal angle tolerance (next)","Horizontal angle tolerance for the next sources (rad)","BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL_NEXT",Property::P_FLOAT_POS);

                        // BEAMFORMER_ANGLE_TOLERANCE_VERTICAL
                        Property* beamformer_anglev = new Property("Vertical angle tolerance","Vertical angle tolerance for the all sources (rad)","BEAMFORMER_ANGLE_TOLERANCE_VERTICAL",Property::P_FLOAT_POS);

                        // BEAMFORMER_MAXRANGE
                        Property* beamformer_scanrange = new Property("Scan range","Range of maxima scanned","BEAMFORMER_MAXRANGE",Property::P_INT_POS);

                        // BEAMFORMER_DELAYS_RESET
                        Property* beamformer_delaysreset = new Property("Number of delays reset","Number of delays reset after each potential source","BEAMFORMER_DELAYS_RESET",Property::P_INT_POS);

                        beamformer->addChild(beamformer_maxsources);
                        beamformer->addChild(beamformer_numbermax);
                        beamformer->addChild(beamformer_maxthres);
                        beamformer->addChild(beamformer_angleh);
                        beamformer->addChild(beamformer_anglehnext);
                        beamformer->addChild(beamformer_anglev);
                        beamformer->addChild(beamformer_scanrange);
                        beamformer->addChild(beamformer_delaysreset);

                beamformer->addChild(beamformer_mics);

        // +---------------------------------------------------+
        // | B. Particle filters                               |
        // +---------------------------------------------------+

                Property* filter = new Property("Particle filters","","filter",Property::P_NOTYPE);

                // +-----------------------------------------------+
                // | I. Standard deviation                         |
                // +-----------------------------------------------+

                        // FILTER_STANDARDDEVIATION
                        Property* filter_stddev = new Property("Standard deviation","Standard deviation of the particles","FILTER_STANDARDDEVIATION",Property::P_FLOAT_POS);

                        filter->addChild(filter_stddev);

                // +-----------------------------------------------+
                // | II. Particle prediction                       |
                // +-----------------------------------------------+

                        Property* filter_predict = new Property("Prediction parameters","","filter_predict",Property::P_NOTYPE);

                        // +-------------------------------------------+
                        // | i. Stopped                                |
                        // +-------------------------------------------+

                                Property* filter_predict_stop = new Property("Stopped particle","","filter_predict_stop",Property::P_NOTYPE);

                                    // FILTER_PREDICTION_STOP_ALPHA
                                    Property* filter_predict_stop_alpha = new Property("Alpha","Alpha parameter for a stopped particle","FILTER_PREDICTION_STOP_ALPHA",Property::P_FLOAT);

                                    // FILTER_PREDICTION_STOP_BETA
                                    Property* filter_predict_stop_beta = new Property("Beta","Beta parameter for a stopped particle","FILTER_PREDICTION_STOP_BETA",Property::P_FLOAT);

                                    filter_predict_stop->addChild(filter_predict_stop_alpha);
                                    filter_predict_stop->addChild(filter_predict_stop_beta);

                                filter_predict->addChild(filter_predict_stop);

                        // +-------------------------------------------+
                        // | ii. Constant velocity                     |
                        // +-------------------------------------------+

                                Property* filter_predict_cst = new Property("Constant velocity particle","","filter_predict_cst",Property::P_NOTYPE);

                                    // FILTER_PREDICTION_CONSTANT_ALPHA
                                    Property* filter_predict_cst_alpha = new Property("Alpha","Alpha parameter for a particle with a constant velocity","FILTER_PREDICTION_CONSTANT_ALPHA",Property::P_FLOAT);

                                    // FILTER_PREDICTION_CONSTANT_BETA
                                    Property* filter_predict_cst_beta = new Property("Beta","Beta parameter for a particle with a constant velocity","FILTER_PREDICTION_CONSTANT_BETA",Property::P_FLOAT);

                                    filter_predict_cst->addChild(filter_predict_cst_alpha);
                                    filter_predict_cst->addChild(filter_predict_cst_beta);

                                filter_predict->addChild(filter_predict_cst);

                        // +-------------------------------------------+
                        // | iii. Accelerated                          |
                        // +-------------------------------------------+

                                Property* filter_predict_acc = new Property("Accelerated particle","","filter_predict_acc",Property::P_NOTYPE);

                                    // FILTER_PREDICTION_ACCELERATED_ALPHA
                                    Property* filter_predict_acc_alpha = new Property("Alpha","Alpha parameter for an accelerated particle","FILTER_PREDICTION_ACCELERATED_ALPHA",Property::P_FLOAT);

                                    // FILTER_PREDICTION_ACCELERATED_BETA
                                    Property* filter_predict_acc_beta = new Property("Beta","Beta parameter for an accelerated particle","FILTER_PREDICTION_ACCELERATED_BETA",Property::P_FLOAT);

                                    filter_predict_acc->addChild(filter_predict_acc_alpha);
                                    filter_predict_acc->addChild(filter_predict_acc_beta);

                                filter_predict->addChild(filter_predict_acc);

                        filter->addChild(filter_predict);

                // +-----------------------------------------------+
                // | III. Particle internia                        |
                // +-----------------------------------------------+

                        Property* filter_inertia = new Property("Inertia factors","","filter_inertia",Property::P_NOTYPE);

                            // FILTER_INERTIA_X
                            Property* filter_inertia_x = new Property("Inertia factors","Inertia in the x-dimension","FILTER_INERTIA_X",Property::P_FLOAT_POS);

                            // FILTER_INERTIA_Y
                            Property* filter_inertia_y = new Property("Inertia factors","Inertia in the y-dimension","FILTER_INERTIA_Y",Property::P_FLOAT_POS);

                            // FILTER_INERTIA_Z
                            Property* filter_inertia_z = new Property("Inertia factors","Inertia in the z-dimension","FILTER_INERTIA_Z",Property::P_FLOAT_POS);

                            filter_inertia->addChild(filter_inertia_x);
                            filter_inertia->addChild(filter_inertia_y);
                            filter_inertia->addChild(filter_inertia_z);

                        filter->addChild(filter_inertia);

                // +-----------------------------------------------+
                // | IV. Others                                    |
                // +-----------------------------------------------+

                        // FILTER_DELTAT
                        Property* filter_deltat = new Property("Time interval","Time interval between each next particle","FILTER_DELTAT",Property::P_FLOAT_POS);

                        // FILTER_STATEUPDATE
                        Property* filter_stateupt = new Property("Update rate","Percentage of particles to be updated (between 0.0 and 1.0)","FILTER_STATEUPDATE",Property::P_FLOAT_POS);

                        // FILTER_STOP_PERCENTAGE
                        Property* filter_stop = new Property("Percentage of stop particles","Percentage of particles that are stopped","FILTER_STOP_PERCENTAGE",Property::P_FLOAT_POS);

                        // FILTER_CONSTANT_PERCENTAGE
                        Property* filter_cst = new Property("Percentage of constant velocity particles","Percentage of particles that have a constant velocity","FILTER_CONSTANT_PERCENTAGE",Property::P_FLOAT_POS);

                        // FILTER_ACCELERATED_PERCENTAGE
                        Property* filter_acc = new Property("Percentage of accelerated particles","Percentage of particles that are accelerating","FILTER_ACCELERATED_PERCENTAGE",Property::P_FLOAT_POS);

                        // FILTER_ACTIVE_ACTIVE
                        Property* filter_activeactive = new Property("Probability active source stays active","Probability that an active source stays active","FILTER_ACTIVE_ACTIVE",Property::P_FLOAT_POS);

                        // FILTER_INACTIVE_ACTIVE
                        Property* filter_inactiveactive = new Property("Probability inactive source stays active","Probability that an inactive source stays active","FILTER_INACTIVE_ACTIVE",Property::P_FLOAT_POS);

                        // FILTER_P0
                        Property* filter_notobserved = new Property("Probability existing source is not observed","Probability that a source is not observed even if it exists","FILTER_P0",Property::P_FLOAT_POS);

                        // FILTER_RESAMPLING_THRESHOLD
                        Property* filter_resampling = new Property("Resampling threshold","Threshold to reach to resample the filter","FILTER_RESAMPLING_THRESHOLD",Property::P_FLOAT_POS);

                        // FILTER_BUFFERSIZE
                        Property* filter_buffersize = new Property("Size of the buffer","Size of the buffer for smoohting the sources","FILTER_BUFFERSIZE",Property::P_INT_POS);

                        filter->addChild(filter_deltat);
                        filter->addChild(filter_stateupt);
                        filter->addChild(filter_stop);
                        filter->addChild(filter_cst);
                        filter->addChild(filter_acc);
                        filter->addChild(filter_activeactive);
                        filter->addChild(filter_inactiveactive);
                        filter->addChild(filter_notobserved);
                        filter->addChild(filter_resampling);
                        filter->addChild(filter_buffersize);



        // +---------------------------------------------------+
        // | C. Mixture                                        |
        // +---------------------------------------------------+

                Property* mixture = new Property("Mixture","","mixture",Property::P_NOTYPE);

                    // MIXTURE_MAX_FILTERS
                    Property* mixture_nbfilters = new Property("Max number of filters","Maximum number of filters to track sources","MIXTURE_MAX_FILTERS",Property::P_INT_POS);

                    // MIXTURE_PNEW
                    Property* mixture_pnew = new Property("P_new", "Probability a new source appears", "MIXTURE_PNEW", Property::P_FLOAT_POS);

                    // MIXTURE_PFALSE
                    Property* mixture_pfalse = new Property("P_false", "Probability a false detection occurs", "MIXTURE_PFALSE", Property::P_FLOAT_POS);

                    // MIXTURE_ET
                    Property* mixture_et = new Property("Et","Threshold to normalize the energy from the beamformer","MIXTURE_ET", Property::P_FLOAT_POS);

                    mixture->addChild(mixture_nbfilters);
                    mixture->addChild(mixture_pnew);
                    mixture->addChild(mixture_pfalse);
                    mixture->addChild(mixture_et);

                    // +-----------------------------------------------+
                    // | I. New source                                 |
                    // +-----------------------------------------------+

                            Property* mixture_new = new Property("New source","","mixture_new",Property::P_NOTYPE);

                                // MIXTURE_NEWSOURCE_THRESHOLD
                                Property* mixture_newthreshold = new Property("New source probabilty threshold","Threshold to reach to add a new source","MIXTURE_NEWSOURCE_THRESHOLD", Property::P_FLOAT_POS);

                                mixture_new->addChild(mixture_newthreshold);

                                Property* mixture_new_confirm = new Property("Source exists confirmation","","mixture_new_confirm",Property::P_NOTYPE);

                                    // +-------------------------------------------+
                                    // | i. Confirm source exists                  |
                                    // +-------------------------------------------+

                                        // MIXTURE_CONFIRM_SOURCE_EXISTS
                                        Property* mixture_confirmthreshold = new Property("New source confirmation threshold","Threshold to reach in order to confirm a source really exists","MIXTURE_CONFIRM_SOURCE_EXISTS", Property::P_FLOAT_POS);

                                        // MIXTURE_CONFIRM_COUNT_THRESHOLD
                                        Property* mixture_countthreshold = new Property("Source exists count threshold","Threshold to count a source as existing","MIXTURE_CONFIRM_COUNT_THRESHOLD", Property::P_FLOAT_POS);

                                        // MIXTURE_CONFIRM_COUNT_COUNTER
                                        Property* mixture_countcounter = new Property("Source exists counter","Number of times the threshold must be reached","MIXTURE_CONFIRM_COUNT_COUNTER", Property::P_INT_POS);

                                        mixture_new_confirm->addChild(mixture_confirmthreshold);
                                        mixture_new_confirm->addChild(mixture_countthreshold);
                                        mixture_new_confirm->addChild(mixture_countcounter);

                                mixture_new->addChild(mixture_new_confirm);

                                // MIXTURE_NEWSOURCE_HORIZONTALANGLE
                                Property* mixture_newangle = new Property("Horizontal angle difference", "Minimum horizontal angle difference that the new source needs to have with the already existing filters (rad)", "MIXTURE_NEWSOURCE_HORIZONTALANGLE", Property::P_FLOAT_POS);

                                mixture_new->addChild(mixture_newangle);

                            mixture->addChild(mixture_new);

                    // +-----------------------------------------------+
                    // | II. Delete source                             |
                    // +-----------------------------------------------+

                            Property* mixture_delete = new Property("Delete source","","mixture_delete",Property::P_NOTYPE);

                                // +-------------------------------------------+
                                // | i. Probabtion                             |
                                // +-------------------------------------------+

                                        Property* mixture_delete_prob = new Property("Probation","","mixture_delete_prob",Property::P_NOTYPE);

                                        // MIXTURE_CUMULATIVE_TIME_PROBATION
                                        Property* mixture_cumulativeprob = new Property("Maximum number of inactive frames","Maximum number of frames while the source has not been tracked in order to delete this tracking for probation time","MIXTURE_CUMULATIVE_TIME_PROBATION", Property::P_INT_POS);

                                        // MIXTURE_NOTOBSERVED_PROBATION_THRESHOLD
                                        Property* mixture_thresholdprob = new Property("Inactivity threshold","Minimum value to consider to say that the source is not observed while in probation","MIXTURE_NOTOBSERVED_PROBATION_THRESHOLD", Property::P_FLOAT_POS);

                                        mixture_delete_prob->addChild(mixture_cumulativeprob);
                                        mixture_delete_prob->addChild(mixture_thresholdprob);

                                mixture_delete->addChild(mixture_delete_prob);

                                // +-------------------------------------------+
                                // | i. Level 1                                |
                                // +-------------------------------------------+

                                        Property* mixture_delete_level1 = new Property("Level 1","","mixture_delete_level1",Property::P_NOTYPE);

                                        // MIXTURE_CUMULATIVE_TIME_LEVEL1
                                        Property* mixture_cumulativelevel1 = new Property("Maximum number of inactive frames","Maximum number of frames while the source has not been tracked in order to delete this tracking for level 1","MIXTURE_CUMULATIVE_TIME_LEVEL1", Property::P_INT_POS);

                                        // MIXTURE_NOTOBSERVED_LEVEL1_THRESHOLD
                                        Property* mixture_thresholdlevel1 = new Property("Level 1 value of inactive frames","Value of level 1 under which a frame is considered as not observed","MIXTURE_NOTOBSERVED_LEVEL1_THRESHOLD", Property::P_FLOAT_POS);

                                        mixture_delete_level1->addChild(mixture_cumulativelevel1);
                                        mixture_delete_level1->addChild(mixture_thresholdlevel1);

                                mixture_delete->addChild(mixture_delete_level1);

                                // +-------------------------------------------+
                                // | i. Level 2                                |
                                // +-------------------------------------------+

                                        Property* mixture_delete_level2 = new Property("Level 2","","mixture_delete_level2",Property::P_NOTYPE);

                                        // MIXTURE_CUMULATIVE_TIME_LEVEL2
                                        Property* mixture_cumulativelevel2 = new Property("Maximum number of inactive frames","Maximum number of frames while the source has not been tracked in order to delete this tracking for level 2","MIXTURE_CUMULATIVE_TIME_LEVEL2", Property::P_INT_POS);

                                        // MIXTURE_NOTOBSERVED_LEVEL2_THRESHOLD
                                        Property* mixture_thresholdlevel2 = new Property("Level 2 value of inactive frames","Value of level 2 under which a frame is considered as not observed","MIXTURE_NOTOBSERVED_LEVEL2_THRESHOLD", Property::P_FLOAT_POS);

                                        mixture_delete_level2->addChild(mixture_cumulativelevel2);
                                        mixture_delete_level2->addChild(mixture_thresholdlevel2);

                                mixture_delete->addChild(mixture_delete_level2);

                            mixture->addChild(mixture_delete);

        // +-----------------------------------------------------------------------+
        // | D. Microphone Sound Track                                             |
        // +-----------------------------------------------------------------------+

            Property* micst = new Property("Microphone Sound Track","","micst",Property::P_NOTYPE);

                // MICST_ALPHAD
                Property* micst_alphad = new Property("AlphaD","Adaptation Rate","MICST_ALPHAD",Property::P_FLOAT_POS);

                // MICST_GAMMA
                Property* micst_gamma = new Property("Gamma","Reverberation delay","MICST_GAMMA",Property::P_FLOAT_POS);

                // MICST_DELTA
                Property* micst_delta = new Property("Delta","Level of reverberation","MICST_DELTA",Property::P_FLOAT_POS);

                micst->addChild(micst_alphad);
                micst->addChild(micst_gamma);
                micst->addChild(micst_delta);

                // +---------------------------------------------------------------+
                // | I. MCRA                                                       |
                // +---------------------------------------------------------------+

                    Property* micst_mcra = new Property("Noise estimator","","micst_mcra",Property::P_NOTYPE);

                        // MICST_MCRA_ALPHAS
                        Property* micst_mcra_alphas = new Property("AlphaS","Coefficient to smooth the spectrum in time","MICST_MCRA_ALPHAS",Property::P_FLOAT_POS);

                        // MICST_MCRA_ALPHAP
                        Property* micst_mcra_alphap = new Property("AlphaP","Probability smoothing parameter","MICST_MCRA_ALPHAP",Property::P_FLOAT_POS);

                        // MICST_MCRA_ALPHAD
                        Property* micst_mcra_alphad = new Property("AlphaD","Constant smoothing parameter","MICST_MCRA_ALPHAD",Property::P_FLOAT_POS);

                        // MICST_MCRA_L
                        Property* micst_mcra_l = new Property("L","Maximum number of frames to refresh","MICST_MCRA_L",Property::P_INT_POS);

                        // MICST_MCRA_DELTA
                        Property* micst_mcra_delta = new Property("Delta","Decision parameter","MICST_MCRA_DELTA",Property::P_FLOAT_POS);

                        micst_mcra->addChild(micst_mcra_alphas);
                        micst_mcra->addChild(micst_mcra_alphap);
                        micst_mcra->addChild(micst_mcra_alphad);
                        micst_mcra->addChild(micst_mcra_l);
                        micst_mcra->addChild(micst_mcra_delta);

                micst->addChild(micst_mcra);

        system->addChild(beamformer);
        system->addChild(filter);
        system->addChild(mixture);
        system->addChild(micst);

    return system;

}

void ParametersManager::loadViewDefault()
{

    // Create a vector
    ConfigurationVector configurationVector;

    // Load properties
    configurationVector.addElement(QString::fromStdString("SPHERE_BACKGROUND_COLOR"),QString::fromStdString("Cffffff"));
    configurationVector.addElement(QString::fromStdString("SPHERE_NORTH_COLOR"),QString::fromStdString("C595959"));
    configurationVector.addElement(QString::fromStdString("SPHERE_SOUTH_COLOR"),QString::fromStdString("C707070"));
    configurationVector.addElement(QString::fromStdString("SPHERE_AXES_COLOR"),QString::fromStdString("C000000"));

    configurationVector.addElement(QString::fromStdString("POTENTIAL_LATITUDE_BACKGROUND_COLOR"),QString::fromStdString("Cffffff"));
    configurationVector.addElement(QString::fromStdString("POTENTIAL_LATITUDE_HORIZONTALAXES_COLOR"),QString::fromStdString("C6d6d6d"));
    configurationVector.addElement(QString::fromStdString("POTENTIAL_LATITUDE_VERTICALAXES_COLOR"),QString::fromStdString("C6d6d6d"));
    configurationVector.addElement(QString::fromStdString("POTENTIAL_LATITUDE_TRACE_SIZE"),QString::fromStdString("I1"));
    configurationVector.addElement(QString::fromStdString("POTENTIAL_LONGITUDE_BACKGROUND_COLOR"),QString::fromStdString("Cffffff"));
    configurationVector.addElement(QString::fromStdString("POTENTIAL_LONGITUDE_HORIZONTALAXES_COLOR"),QString::fromStdString("C6d6d6d"));
    configurationVector.addElement(QString::fromStdString("POTENTIAL_LONGITUDE_VERTICALAXES_COLOR"),QString::fromStdString("C6d6d6d"));
    configurationVector.addElement(QString::fromStdString("POTENTIAL_LONGITUDE_TRACE_SIZE"),QString::fromStdString("I1"));
    configurationVector.addElement(QString::fromStdString("TRACKED_LATITUDE_BACKGROUND_COLOR"),QString::fromStdString("Cffffff"));
    configurationVector.addElement(QString::fromStdString("TRACKED_LATITUDE_HORIZONTALAXES_COLOR"),QString::fromStdString("C6d6d6d"));
    configurationVector.addElement(QString::fromStdString("TRACKED_LATITUDE_VERTICALAXES_COLOR"),QString::fromStdString("C6d6d6d"));
    configurationVector.addElement(QString::fromStdString("TRACKED_LATITUDE_TRACE_SIZE"),QString::fromStdString("I1"));
    configurationVector.addElement(QString::fromStdString("TRACKED_LONGITUDE_BACKGROUND_COLOR"),QString::fromStdString("Cffffff"));
    configurationVector.addElement(QString::fromStdString("TRACKED_LONGITUDE_HORIZONTALAXES_COLOR"),QString::fromStdString("C6d6d6d"));
    configurationVector.addElement(QString::fromStdString("TRACKED_LONGITUDE_VERTICALAXES_COLOR"),QString::fromStdString("C6d6d6d"));
    configurationVector.addElement(QString::fromStdString("TRACKED_LONGITUDE_TRACE_SIZE"),QString::fromStdString("I1"));

    configurationVector.addElement(QString::fromStdString("TIME_BUFFER"),QString::fromStdString("F10"));
    configurationVector.addElement(QString::fromStdString("TIME_AFTER"),QString::fromStdString("F1"));
    configurationVector.addElement(QString::fromStdString("TIME_INTERVAL"),QString::fromStdString("F1"));

    configurationVector.addElement(QString::fromStdString("SOURCE1_COLOR"),QString::fromStdString("Cff0000"));
    configurationVector.addElement(QString::fromStdString("SOURCE2_COLOR"),QString::fromStdString("C0000ff"));
    configurationVector.addElement(QString::fromStdString("SOURCE3_COLOR"),QString::fromStdString("C00aa00"));
    configurationVector.addElement(QString::fromStdString("SOURCE4_COLOR"),QString::fromStdString("Caa00ff"));
    configurationVector.addElement(QString::fromStdString("SOURCE5_COLOR"),QString::fromStdString("C000073"));
    configurationVector.addElement(QString::fromStdString("SOURCE6_COLOR"),QString::fromStdString("C005500"));
    configurationVector.addElement(QString::fromStdString("SOURCE7_COLOR"),QString::fromStdString("Cffaa00"));
    configurationVector.addElement(QString::fromStdString("SOURCE8_COLOR"),QString::fromStdString("Cff00ff"));
    configurationVector.addElement(QString::fromStdString("SOURCE9_COLOR"),QString::fromStdString("C550000"));
    configurationVector.addElement(QString::fromStdString("SOURCE10_COLOR"),QString::fromStdString("Caa5500"));

    // Save
    this->viewParameters = configurationVector;

}

void ParametersManager::loadSystemDefault()
{

    ParametersStruct parametersStruct;

    // Create a vector
    ConfigurationVector configurationVector;

    // Load default parameters
    ParametersLoadDefault(&parametersStruct);

    // Load properties from the default parameters in the core

    // Beamformer
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MAXPOTENTIAL"), parametersStruct.P_BF_MAXSOURCES);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_NUMBERMAX"), parametersStruct.P_BF_NUMBERMAX);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MAXTHRESHOLD"), parametersStruct.P_BF_VALUETS);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL"), parametersStruct.P_BF_HANGLETOL);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL_NEXT"), parametersStruct.P_BF_HANGLETOLNEXT);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_ANGLE_TOLERANCE_VERTICAL"), parametersStruct.P_BF_VANGLETOL);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MAXRANGE"), parametersStruct.P_BF_RANGE);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_DELAYS_RESET"), parametersStruct.P_BF_RIJDELAYS);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC1_GAIN"), parametersStruct.P_BF_MICSGAIN_1);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC1_X"), parametersStruct.P_BF_MICSPOSITIONS_1_X);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC1_Y"), parametersStruct.P_BF_MICSPOSITIONS_1_Y);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC1_Z"), parametersStruct.P_BF_MICSPOSITIONS_1_Z);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC2_GAIN"), parametersStruct.P_BF_MICSGAIN_2);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC2_X"), parametersStruct.P_BF_MICSPOSITIONS_2_X);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC2_Y"), parametersStruct.P_BF_MICSPOSITIONS_2_Y);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC2_Z"), parametersStruct.P_BF_MICSPOSITIONS_2_Z);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC3_GAIN"), parametersStruct.P_BF_MICSGAIN_3);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC3_X"), parametersStruct.P_BF_MICSPOSITIONS_3_X);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC3_Y"), parametersStruct.P_BF_MICSPOSITIONS_3_Y);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC3_Z"), parametersStruct.P_BF_MICSPOSITIONS_3_Z);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC4_GAIN"), parametersStruct.P_BF_MICSGAIN_4);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC4_X"), parametersStruct.P_BF_MICSPOSITIONS_4_X);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC4_Y"), parametersStruct.P_BF_MICSPOSITIONS_4_Y);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC4_Z"), parametersStruct.P_BF_MICSPOSITIONS_4_Z);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC5_GAIN"), parametersStruct.P_BF_MICSGAIN_5);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC5_X"), parametersStruct.P_BF_MICSPOSITIONS_5_X);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC5_Y"), parametersStruct.P_BF_MICSPOSITIONS_5_Y);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC5_Z"), parametersStruct.P_BF_MICSPOSITIONS_5_Z);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC6_GAIN"), parametersStruct.P_BF_MICSGAIN_6);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC6_X"), parametersStruct.P_BF_MICSPOSITIONS_6_X);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC6_Y"), parametersStruct.P_BF_MICSPOSITIONS_6_Y);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC6_Z"), parametersStruct.P_BF_MICSPOSITIONS_6_Z);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC7_GAIN"), parametersStruct.P_BF_MICSGAIN_7);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC7_X"), parametersStruct.P_BF_MICSPOSITIONS_7_X);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC7_Y"), parametersStruct.P_BF_MICSPOSITIONS_7_Y);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC7_Z"), parametersStruct.P_BF_MICSPOSITIONS_7_Z);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC8_GAIN"), parametersStruct.P_BF_MICSGAIN_8);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC8_X"), parametersStruct.P_BF_MICSPOSITIONS_8_X);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC8_Y"), parametersStruct.P_BF_MICSPOSITIONS_8_Y);
    configurationVector.addElement(QString::fromStdString("BEAMFORMER_MIC8_Z"), parametersStruct.P_BF_MICSPOSITIONS_8_Z);

    // Filter
    configurationVector.addElement(QString::fromStdString("FILTER_STANDARDDEVIATION"), parametersStruct.P_FILTER_STDDEVIATION);
    configurationVector.addElement(QString::fromStdString("FILTER_PREDICTION_STOP_ALPHA"), parametersStruct.P_FILTER_ALPHASTOP);
    configurationVector.addElement(QString::fromStdString("FILTER_PREDICTION_STOP_BETA"), parametersStruct.P_FILTER_BETASTOP);
    configurationVector.addElement(QString::fromStdString("FILTER_PREDICTION_CONSTANT_ALPHA"), parametersStruct.P_FILTER_ALPHACONST);
    configurationVector.addElement(QString::fromStdString("FILTER_PREDICTION_CONSTANT_BETA"), parametersStruct.P_FILTER_BETACONST);
    configurationVector.addElement(QString::fromStdString("FILTER_PREDICTION_ACCELERATED_ALPHA"), parametersStruct.P_FILTER_ALPHAEXC);
    configurationVector.addElement(QString::fromStdString("FILTER_PREDICTION_ACCELERATED_BETA"), parametersStruct.P_FILTER_BETAEXC);
    configurationVector.addElement(QString::fromStdString("FILTER_INERTIA_X"), parametersStruct.P_FILTER_INERTIAX);
    configurationVector.addElement(QString::fromStdString("FILTER_INERTIA_Y"), parametersStruct.P_FILTER_INERTIAY);
    configurationVector.addElement(QString::fromStdString("FILTER_INERTIA_Z"), parametersStruct.P_FILTER_INERTIAZ);
    configurationVector.addElement(QString::fromStdString("FILTER_DELTAT"), parametersStruct.P_FILTER_DELTAT);
    configurationVector.addElement(QString::fromStdString("FILTER_STATEUPDATE"), parametersStruct.P_FILTER_STATEUPDT);
    configurationVector.addElement(QString::fromStdString("FILTER_STOP_PERCENTAGE"), parametersStruct.P_FILTER_NEWSTOP);
    configurationVector.addElement(QString::fromStdString("FILTER_CONSTANT_PERCENTAGE"), parametersStruct.P_FILTER_NEWCONST);
    configurationVector.addElement(QString::fromStdString("FILTER_ACCELERATED_PERCENTAGE"), parametersStruct.P_FILTER_NEWEXC);
    configurationVector.addElement(QString::fromStdString("FILTER_ACTIVE_ACTIVE"), parametersStruct.P_FILTER_AJT_AJTM1);
    configurationVector.addElement(QString::fromStdString("FILTER_INACTIVE_ACTIVE"), parametersStruct.P_FILTER_AJT_NOTAJTM1);
    configurationVector.addElement(QString::fromStdString("FILTER_P0"), parametersStruct.P_FILTER_P0);
    configurationVector.addElement(QString::fromStdString("FILTER_RESAMPLING_THRESHOLD"), parametersStruct.P_FILTER_RSTHRESHOLD);
    configurationVector.addElement(QString::fromStdString("FILTER_BUFFERSIZE"), parametersStruct.P_FILTER_BUFFERSIZE);

    // Mixture
    configurationVector.addElement(QString::fromStdString("MIXTURE_MAX_FILTERS"), parametersStruct.P_MIXTURE_NBFILTERS);
    configurationVector.addElement(QString::fromStdString("MIXTURE_PNEW"), parametersStruct.P_MIXTURE_PNEW);
    configurationVector.addElement(QString::fromStdString("MIXTURE_PFALSE"), parametersStruct.P_MIXTURE_PFALSE);
    configurationVector.addElement(QString::fromStdString("MIXTURE_ET"), parametersStruct.P_MIXTURE_ET);
    configurationVector.addElement(QString::fromStdString("MIXTURE_NEWSOURCE_THRESHOLD"), parametersStruct.P_MIXTURE_NEWTHRESHOLD);
    configurationVector.addElement(QString::fromStdString("MIXTURE_CONFIRM_SOURCE_EXISTS"), parametersStruct.P_MIXTURE_CONFIRMEXISTS);
    configurationVector.addElement(QString::fromStdString("MIXTURE_CONFIRM_COUNT_THRESHOLD"), parametersStruct.P_MIXTURE_CONFIRMCOUNTTS);
    configurationVector.addElement(QString::fromStdString("MIXTURE_CONFIRM_COUNT_COUNTER"), parametersStruct.P_MIXTURE_CONFIRMCOUNT);
    configurationVector.addElement(QString::fromStdString("MIXTURE_NEWSOURCE_HORIZONTALANGLE"), parametersStruct.P_MIXTURE_NEWANGLE);
    configurationVector.addElement(QString::fromStdString("MIXTURE_CUMULATIVE_TIME_PROBATION"), parametersStruct.P_MIXTURE_CUMULATIVETIMEPROB);
    configurationVector.addElement(QString::fromStdString("MIXTURE_NOTOBSERVED_PROBATION_THRESHOLD"), parametersStruct.P_MIXTURE_TOBSPROB);
    configurationVector.addElement(QString::fromStdString("MIXTURE_CUMULATIVE_TIME_LEVEL1"), parametersStruct.P_MIXTURE_CUMULATIVETIME1);
    configurationVector.addElement(QString::fromStdString("MIXTURE_NOTOBSERVED_LEVEL1_THRESHOLD"), parametersStruct.P_MIXTURE_TOBS1);
    configurationVector.addElement(QString::fromStdString("MIXTURE_CUMULATIVE_TIME_LEVEL2"), parametersStruct.P_MIXTURE_CUMULATIVETIME2);
    configurationVector.addElement(QString::fromStdString("MIXTURE_NOTOBSERVED_LEVEL2_THRESHOLD"), parametersStruct.P_MIXTURE_TOBS2);

    // Microphone Sound Track
    configurationVector.addElement(QString::fromStdString("MICST_ALPHAD"), parametersStruct.P_MICST_ALPHAD);
    configurationVector.addElement(QString::fromStdString("MICST_GAMMA"), parametersStruct.P_MICST_GAMMA);
    configurationVector.addElement(QString::fromStdString("MICST_DELTA"), parametersStruct.P_MICST_DELTA);
    configurationVector.addElement(QString::fromStdString("MICST_MCRA_ALPHAS"), parametersStruct.P_MCRA_ALPHAS);
    configurationVector.addElement(QString::fromStdString("MICST_MCRA_ALPHAP"), parametersStruct.P_MCRA_ALPHAP);
    configurationVector.addElement(QString::fromStdString("MICST_MCRA_ALPHAD"), parametersStruct.P_MCRA_ALPHAD);
    configurationVector.addElement(QString::fromStdString("MICST_MCRA_L"), parametersStruct.P_MCRA_L);
    configurationVector.addElement(QString::fromStdString("MICST_MCRA_DELTA"), parametersStruct.P_MCRA_DELTA);

    // Save
    this->systemParameters = configurationVector;

}

/***********************************************************
* Get property value                                       *
***********************************************************/

float ParametersManager::getPropertyViewFloat(QString _key)
{

    return this->viewParameters.getElementFloat(_key);

}

QString ParametersManager::getPropertyViewFloatString(QString _key)
{

    return this->viewParameters.getValue(_key);

}

int ParametersManager::getPropertyViewInt(QString _key)
{

    return this->viewParameters.getElementInt(_key);

}

QString ParametersManager::getPropertyViewIntString(QString _key)
{

    return this->viewParameters.getValue(_key);

}

QColor ParametersManager::getPropertyViewColor(QString _key)
{

    return this->viewParameters.getElementColor(_key);

}

QString ParametersManager::getPropertyViewColorString(QString _key)
{

    return this->viewParameters.getValue(_key);

}

QString ParametersManager::getPropertyViewValueString(QString _key)
{


    QString tmp;

    switch (this->viewParameters.getType(_key))
    {

    case Property::P_FLOAT:
    case Property::P_FLOAT_POS:
    case Property::P_INT:
    case Property::P_INT_POS:
    case Property::P_COLOR:

        tmp = this->viewParameters.getValue(_key);

        break;

    case Property::P_NOTYPE:

        tmp = "";

        break;

    }

    return tmp;

}

float ParametersManager::getPropertySystemFloat(QString _key)
{

    return this->systemParameters.getElementFloat(_key);

}

QString ParametersManager::getPropertySystemFloatString(QString _key)
{

    return this->systemParameters.getValue(_key);

}

int ParametersManager::getPropertySystemInt(QString _key)
{

    return this->systemParameters.getElementInt(_key);

}

QString ParametersManager::getPropertySystemIntString(QString _key)
{

    return this->systemParameters.getValue(_key);

}

QColor ParametersManager::getPropertySystemColor(QString _key)
{

    return this->systemParameters.getElementColor(_key);

}

QString ParametersManager::getPropertySystemColorString(QString _key)
{

    return this->systemParameters.getValue(_key);

}

QString ParametersManager::getPropertySystemValueString(QString _key)
{

    QString tmp;

    switch (this->systemParameters.getType(_key))
    {

    case Property::P_FLOAT:
    case Property::P_FLOAT_POS:
    case Property::P_INT:
    case Property::P_INT_POS:
    case Property::P_COLOR:

        tmp = this->viewParameters.getValue(_key);

        break;

    case Property::P_NOTYPE:

        tmp = "";

        break;

    }

    return tmp;

}

/***********************************************************
* Set property value                                       *
***********************************************************/

void ParametersManager::setPropertyViewFloat(QString _key, float _value)
{

    this->viewParameters.setValue(_key, _value);

}

void ParametersManager::setPropertyViewInt(QString _key, int _value)
{

    this->viewParameters.setValue(_key, _value);

}

void ParametersManager::setPropertyViewColor(QString _key, QColor _value)
{

    this->viewParameters.setValue(_key, _value);

}

void ParametersManager::setPropertyViewValueString(QString _key, QString _value)
{

    this->viewParameters.setValue(_key, _value);

}

void ParametersManager::setPropertySystemFloat(QString _key, float _value)
{

    this->systemParameters.setValue(_key, _value);

}

void ParametersManager::setPropertySystemInt(QString _key, int _value)
{

    this->systemParameters.setValue(_key, _value);

}

void ParametersManager::setPropertySystemColor(QString _key, QColor _value)
{

    this->systemParameters.setValue(_key, _value);

}

void ParametersManager::setPropertySystemValueString(QString _key, QString _value)
{

    this->systemParameters.setValue(_key, _value);

}

/***********************************************************
* Save parameters to a file                                *
***********************************************************/

void ParametersManager::saveParametersSystem(QString _filePath)
{

    ConfigurationVector configurationVector;

    // Copy
    configurationVector = copyParametersSystem();

    // Save in file
    configurationVector.saveToFile(_filePath);

}

void ParametersManager::saveParametersView(QString _filePath)
{

    ConfigurationVector configurationVector;

    // Copy
    configurationVector = copyParametersView();

    // Save in file
    configurationVector.saveToFile(_filePath);

}

/***********************************************************
* Open parameters from a file                              *
***********************************************************/

void ParametersManager::openParametersSystem(QString _filePath)
{

    ConfigurationVector configurationVector;

    // Open from file
    configurationVector.openFromFile(_filePath);

    // Load
    loadParametersSystem(configurationVector);

    // Then forward this event to all objects that need to update their parameters
    for (int indexReceiver = 0; indexReceiver < this->eventReceiversSystem.size(); indexReceiver++)
    {
        QCoreApplication::postEvent(this->eventReceiversSystem[indexReceiver], new UpdateParametersEvent(this->systemParameters, UpdateParametersEvent::SYSTEM));
    }

}

void ParametersManager::openParametersView(QString _filePath)
{

    ConfigurationVector configurationVector;

    // Open from file
    configurationVector.openFromFile(_filePath);

    // Load
    loadParametersView(configurationVector);

    // Then forward this event to all objects that need to update their parameters
    for (int indexReceiver = 0; indexReceiver < this->eventReceiversView.size(); indexReceiver++)
    {
        QCoreApplication::postEvent(this->eventReceiversView[indexReceiver], new UpdateParametersEvent(this->viewParameters, UpdateParametersEvent::VIEW));
    }

}

/***********************************************************
* Events                                                   *
***********************************************************/

bool ParametersManager::event(QEvent *event)
{

    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | Update the parameters                         |
        // +-----------------------------------------------+

        UpdateParametersEvent *updateParametersEvent = dynamic_cast<UpdateParametersEvent*>(event);

        if (updateParametersEvent != NULL)
        {

            if (updateParametersEvent->getTypeVector() == UpdateParametersEvent::VIEW)
            {
                loadParametersView(updateParametersEvent->getConfigurationVector());

                // Then forward this event to all objects that need to update their parameters
                for (int indexReceiver = 0; indexReceiver < this->eventReceiversView.size(); indexReceiver++)
                {
                    QCoreApplication::postEvent(this->eventReceiversView[indexReceiver], new UpdateParametersEvent(this->viewParameters, UpdateParametersEvent::VIEW));
                }

            }
            else
            {
                loadParametersSystem(updateParametersEvent->getConfigurationVector());

                // Then forward this event to all objects that need to update their parameters
                for (int indexReceiver = 0; indexReceiver < this->eventReceiversSystem.size(); indexReceiver++)
                {
                    QCoreApplication::postEvent(this->eventReceiversSystem[indexReceiver], new UpdateParametersEvent(this->systemParameters, UpdateParametersEvent::SYSTEM));
                }

            }

        }

        return true;

    }

    return ParametersManager::event(event);

}

/***********************************************************
* Copy parameters values to a configuration vector         *
***********************************************************/

ConfigurationVector ParametersManager::copyParametersView()
{

    return this->viewParameters;

}

ConfigurationVector ParametersManager::copyParametersSystem()
{

    return this->systemParameters;

}

void ParametersManager::copyParametersRecursive(Property *_property, ConfigurationVector* _configurationVector)
{

    QString tmp;

    tmp = _property->getKey();

    if (_property->isALeaf())
    {
        // If this the leaf, then add the value
        _configurationVector->addElement(_property);
    }
    else
    {
        // Loop for all children
        for (int indexChild = 0; indexChild < _property->getNumberChildren(); indexChild++)
        {
            this->copyParametersRecursive(_property->getChild(indexChild), _configurationVector);
        }

    }

}

/***********************************************************
* Load parameters values from the configuration vector     *
***********************************************************/

void ParametersManager::loadParametersView(ConfigurationVector _configurationVector)
{

    QString key;
    QString value;

    for (int indexElement = 0; indexElement < _configurationVector.getNumberElements(); indexElement++)
    {

        key = _configurationVector.getKey(indexElement);
        value = _configurationVector.getValue(key);

        setPropertyViewValueString(key, value);

    }

}

void ParametersManager::loadParametersSystem(ConfigurationVector _configurationVector)
{

    QString key;
    QString value;

    for (int indexElement = 0; indexElement < _configurationVector.getNumberElements(); indexElement++)
    {

        key = _configurationVector.getKey(indexElement);
        value = _configurationVector.getValue(key);

        setPropertySystemValueString(key, value);

    }

}

/***********************************************************
* Push event receivers                                     *
***********************************************************/

void ParametersManager::pushEventReceiverView(QObject* _receiver)
{

    this->eventReceiversView.push_back(_receiver);

}

void ParametersManager::pushEventReceiverSystem(QObject* _receiver)
{

    this->eventReceiversSystem.push_back(_receiver);

}

/***********************************************************
* Trigger parameters refresh event                         *
***********************************************************/

void ParametersManager::triggerParametersRefreshEventSystem()
{

    for (int indexReceiver = 0; indexReceiver < this->eventReceiversSystem.size(); indexReceiver++)
    {

        QCoreApplication::postEvent(this->eventReceiversSystem[indexReceiver], new UpdateParametersEvent(this->systemParameters, UpdateParametersEvent::SYSTEM));

    }

}

void ParametersManager::triggerParametersRefreshEventView()
{

    for (int indexReceiver = 0; indexReceiver < this->eventReceiversView.size(); indexReceiver++)
    {

        QCoreApplication::postEvent(this->eventReceiversView[indexReceiver], new UpdateParametersEvent(this->viewParameters, UpdateParametersEvent::VIEW));

    }

}

/***********************************************************
* Copy system parameters values to the core algorithm      *
***********************************************************/

struct ParametersStruct ParametersManager::getParametersSystem()
{

    ParametersStruct parametersStruct;

    // Create a vector
    ConfigurationVector configurationVector;

    // Load values in vector
    configurationVector = copyParametersSystem();

    // Load properties in the structure of parameters

    // Beamformer
    parametersStruct.P_BF_MAXSOURCES = configurationVector.getElementInt("BEAMFORMER_MAXPOTENTIAL");
    parametersStruct.P_BF_NUMBERMAX = configurationVector.getElementInt("BEAMFORMER_NUMBERMAX");
    parametersStruct.P_BF_VALUETS = configurationVector.getElementFloat("BEAMFORMER_MAXTHRESHOLD");
    parametersStruct.P_BF_HANGLETOL = configurationVector.getElementFloat("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL");
    parametersStruct.P_BF_HANGLETOLNEXT = configurationVector.getElementFloat("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL_NEXT");
    parametersStruct.P_BF_VANGLETOL = configurationVector.getElementFloat("BEAMFORMER_ANGLE_TOLERANCE_VERTICAL");
    parametersStruct.P_BF_RANGE = configurationVector.getElementInt("BEAMFORMER_MAXRANGE");
    parametersStruct.P_BF_RIJDELAYS = configurationVector.getElementInt("BEAMFORMER_DELAYS_RESET");
    parametersStruct.P_BF_MICSGAIN_1 = configurationVector.getElementFloat("BEAMFORMER_MIC1_GAIN");
    parametersStruct.P_BF_MICSPOSITIONS_1_X = configurationVector.getElementFloat("BEAMFORMER_MIC1_X");
    parametersStruct.P_BF_MICSPOSITIONS_1_Y = configurationVector.getElementFloat("BEAMFORMER_MIC1_Y");
    parametersStruct.P_BF_MICSPOSITIONS_1_Z = configurationVector.getElementFloat("BEAMFORMER_MIC1_Z");
    parametersStruct.P_BF_MICSGAIN_2 = configurationVector.getElementFloat("BEAMFORMER_MIC2_GAIN");
    parametersStruct.P_BF_MICSPOSITIONS_2_X = configurationVector.getElementFloat("BEAMFORMER_MIC2_X");
    parametersStruct.P_BF_MICSPOSITIONS_2_Y = configurationVector.getElementFloat("BEAMFORMER_MIC2_Y");
    parametersStruct.P_BF_MICSPOSITIONS_2_Z = configurationVector.getElementFloat("BEAMFORMER_MIC2_Z");
    parametersStruct.P_BF_MICSGAIN_3 = configurationVector.getElementFloat("BEAMFORMER_MIC3_GAIN");
    parametersStruct.P_BF_MICSPOSITIONS_3_X = configurationVector.getElementFloat("BEAMFORMER_MIC3_X");
    parametersStruct.P_BF_MICSPOSITIONS_3_Y = configurationVector.getElementFloat("BEAMFORMER_MIC3_Y");
    parametersStruct.P_BF_MICSPOSITIONS_3_Z = configurationVector.getElementFloat("BEAMFORMER_MIC3_Z");
    parametersStruct.P_BF_MICSGAIN_4 = configurationVector.getElementFloat("BEAMFORMER_MIC4_GAIN");
    parametersStruct.P_BF_MICSPOSITIONS_4_X = configurationVector.getElementFloat("BEAMFORMER_MIC4_X");
    parametersStruct.P_BF_MICSPOSITIONS_4_Y = configurationVector.getElementFloat("BEAMFORMER_MIC4_Y");
    parametersStruct.P_BF_MICSPOSITIONS_4_Z = configurationVector.getElementFloat("BEAMFORMER_MIC4_Z");
    parametersStruct.P_BF_MICSGAIN_5 = configurationVector.getElementFloat("BEAMFORMER_MIC5_GAIN");
    parametersStruct.P_BF_MICSPOSITIONS_5_X = configurationVector.getElementFloat("BEAMFORMER_MIC5_X");
    parametersStruct.P_BF_MICSPOSITIONS_5_Y = configurationVector.getElementFloat("BEAMFORMER_MIC5_Y");
    parametersStruct.P_BF_MICSPOSITIONS_5_Z = configurationVector.getElementFloat("BEAMFORMER_MIC5_Z");
    parametersStruct.P_BF_MICSGAIN_6 = configurationVector.getElementFloat("BEAMFORMER_MIC6_GAIN");
    parametersStruct.P_BF_MICSPOSITIONS_6_X = configurationVector.getElementFloat("BEAMFORMER_MIC6_X");
    parametersStruct.P_BF_MICSPOSITIONS_6_Y = configurationVector.getElementFloat("BEAMFORMER_MIC6_Y");
    parametersStruct.P_BF_MICSPOSITIONS_6_Z = configurationVector.getElementFloat("BEAMFORMER_MIC6_Z");
    parametersStruct.P_BF_MICSGAIN_7 = configurationVector.getElementFloat("BEAMFORMER_MIC7_GAIN");
    parametersStruct.P_BF_MICSPOSITIONS_7_X = configurationVector.getElementFloat("BEAMFORMER_MIC7_X");
    parametersStruct.P_BF_MICSPOSITIONS_7_Y = configurationVector.getElementFloat("BEAMFORMER_MIC7_Y");
    parametersStruct.P_BF_MICSPOSITIONS_7_Z = configurationVector.getElementFloat("BEAMFORMER_MIC7_Z");
    parametersStruct.P_BF_MICSGAIN_8 = configurationVector.getElementFloat("BEAMFORMER_MIC8_GAIN");
    parametersStruct.P_BF_MICSPOSITIONS_8_X = configurationVector.getElementFloat("BEAMFORMER_MIC8_X");
    parametersStruct.P_BF_MICSPOSITIONS_8_Y = configurationVector.getElementFloat("BEAMFORMER_MIC8_Y");
    parametersStruct.P_BF_MICSPOSITIONS_8_Z = configurationVector.getElementFloat("BEAMFORMER_MIC8_Z");

    // Filter
    parametersStruct.P_FILTER_STDDEVIATION = configurationVector.getElementFloat("FILTER_STANDARDDEVIATION");
    parametersStruct.P_FILTER_ALPHASTOP = configurationVector.getElementFloat("FILTER_PREDICTION_STOP_ALPHA");
    parametersStruct.P_FILTER_BETASTOP = configurationVector.getElementFloat("FILTER_PREDICTION_STOP_BETA");
    parametersStruct.P_FILTER_ALPHACONST = configurationVector.getElementFloat("FILTER_PREDICTION_CONSTANT_ALPHA");
    parametersStruct.P_FILTER_BETACONST = configurationVector.getElementFloat("FILTER_PREDICTION_CONSTANT_BETA");
    parametersStruct.P_FILTER_ALPHAEXC = configurationVector.getElementFloat("FILTER_PREDICTION_ACCELERATED_ALPHA");
    parametersStruct.P_FILTER_BETAEXC = configurationVector.getElementFloat("FILTER_PREDICTION_ACCELERATED_BETA");
    parametersStruct.P_FILTER_INERTIAX = configurationVector.getElementFloat("FILTER_INERTIA_X");
    parametersStruct.P_FILTER_INERTIAY = configurationVector.getElementFloat("FILTER_INERTIA_Y");
    parametersStruct.P_FILTER_INERTIAZ = configurationVector.getElementFloat("FILTER_INERTIA_Z");
    parametersStruct.P_FILTER_DELTAT = configurationVector.getElementFloat("FILTER_DELTAT");
    parametersStruct.P_FILTER_STATEUPDT = configurationVector.getElementFloat("FILTER_STATEUPDATE");
    parametersStruct.P_FILTER_NEWSTOP = configurationVector.getElementFloat("FILTER_STOP_PERCENTAGE");
    parametersStruct.P_FILTER_NEWCONST = configurationVector.getElementFloat("FILTER_CONSTANT_PERCENTAGE");
    parametersStruct.P_FILTER_NEWEXC = configurationVector.getElementFloat("FILTER_ACCELERATED_PERCENTAGE");
    parametersStruct.P_FILTER_AJT_AJTM1 = configurationVector.getElementFloat("FILTER_ACTIVE_ACTIVE");
    parametersStruct.P_FILTER_AJT_NOTAJTM1 = configurationVector.getElementFloat("FILTER_INACTIVE_ACTIVE");
    parametersStruct.P_FILTER_P0 = configurationVector.getElementFloat("FILTER_P0");
    parametersStruct.P_FILTER_RSTHRESHOLD = configurationVector.getElementFloat("FILTER_RESAMPLING_THRESHOLD");
    parametersStruct.P_FILTER_BUFFERSIZE = configurationVector.getElementInt("FILTER_BUFFERSIZE");

    // Mixture
    parametersStruct.P_MIXTURE_NBFILTERS = configurationVector.getElementInt("MIXTURE_MAX_FILTERS");
    parametersStruct.P_MIXTURE_PNEW = configurationVector.getElementFloat("MIXTURE_PNEW");
    parametersStruct.P_MIXTURE_PFALSE = configurationVector.getElementFloat("MIXTURE_PFALSE");
    parametersStruct.P_MIXTURE_ET = configurationVector.getElementFloat("MIXTURE_ET");
    parametersStruct.P_MIXTURE_NEWTHRESHOLD = configurationVector.getElementFloat("MIXTURE_NEWSOURCE_THRESHOLD");
    parametersStruct.P_MIXTURE_CONFIRMEXISTS = configurationVector.getElementFloat("MIXTURE_CONFIRM_SOURCE_EXISTS");
    parametersStruct.P_MIXTURE_CONFIRMCOUNTTS = configurationVector.getElementFloat("MIXTURE_CONFIRM_COUNT_THRESHOLD");
    parametersStruct.P_MIXTURE_CONFIRMCOUNT = configurationVector.getElementInt("MIXTURE_CONFIRM_COUNT_COUNTER");
    parametersStruct.P_MIXTURE_NEWANGLE = configurationVector.getElementFloat("MIXTURE_NEWSOURCE_HORIZONTALANGLE");
    parametersStruct.P_MIXTURE_CUMULATIVETIMEPROB = configurationVector.getElementInt("MIXTURE_CUMULATIVE_TIME_PROBATION");
    parametersStruct.P_MIXTURE_TOBSPROB = configurationVector.getElementFloat("MIXTURE_NOTOBSERVED_PROBATION_THRESHOLD");
    parametersStruct.P_MIXTURE_CUMULATIVETIME1 = configurationVector.getElementInt("MIXTURE_CUMULATIVE_TIME_LEVEL1");
    parametersStruct.P_MIXTURE_TOBS1 = configurationVector.getElementFloat("MIXTURE_NOTOBSERVED_LEVEL1_THRESHOLD");
    parametersStruct.P_MIXTURE_CUMULATIVETIME2 = configurationVector.getElementInt("MIXTURE_CUMULATIVE_TIME_LEVEL2");
    parametersStruct.P_MIXTURE_TOBS2 = configurationVector.getElementFloat("MIXTURE_NOTOBSERVED_LEVEL2_THRESHOLD");

    // Microphone Sound Track
    parametersStruct.P_MICST_ALPHAD = configurationVector.getElementFloat("MICST_ALPHAD");
    parametersStruct.P_MICST_GAMMA = configurationVector.getElementFloat("MICST_GAMMA");
    parametersStruct.P_MICST_DELTA = configurationVector.getElementFloat("MICST_DELTA");
    parametersStruct.P_MCRA_ALPHAS = configurationVector.getElementFloat("MICST_MCRA_ALPHAS");
    parametersStruct.P_MCRA_ALPHAP = configurationVector.getElementFloat("MICST_MCRA_ALPHAP");
    parametersStruct.P_MCRA_ALPHAD = configurationVector.getElementFloat("MICST_MCRA_ALPHAD");
    parametersStruct.P_MCRA_L = configurationVector.getElementInt("MICST_MCRA_L");
    parametersStruct.P_MCRA_DELTA = configurationVector.getElementFloat("MICST_MCRA_DELTA");

    return parametersStruct;

}

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Events                                                x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

/***********************************************************
* Constructor                                              *
***********************************************************/

UpdateParametersEvent::UpdateParametersEvent(): QEvent(QEvent::User)
{

}

UpdateParametersEvent::UpdateParametersEvent(const UpdateParametersEvent& _updateParametersEvent): QEvent(QEvent::User)
{
    *this = _updateParametersEvent;
}

UpdateParametersEvent::UpdateParametersEvent(const ConfigurationVector _configurationVector, const int _typeVector): QEvent(QEvent::User)
{
    this->configurationVector = _configurationVector;
    this->typeVector = _typeVector;
}


/***********************************************************
* Destructor                                               *
***********************************************************/

UpdateParametersEvent::~UpdateParametersEvent()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

UpdateParametersEvent& UpdateParametersEvent::operator=(const UpdateParametersEvent& _updateParametersEvent)
{
    this->configurationVector = _updateParametersEvent.getConfigurationVector();
    this->typeVector = _updateParametersEvent.getTypeVector();

    return *this;
}

/***********************************************************
* Accessor                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Configuration vector                                  |
// +-------------------------------------------------------+

ConfigurationVector UpdateParametersEvent::getConfigurationVector() const
{
    return (this->configurationVector);
}

// +-------------------------------------------------------+
// | Type of vector                                        |
// +-------------------------------------------------------+

int UpdateParametersEvent::getTypeVector() const
{
    return (this->typeVector);
}
