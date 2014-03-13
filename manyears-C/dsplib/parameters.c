/*******************************************************************************
 * ManyEars: Parameters - Source Code                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: June 29th, 2010                                                       *
 *                                                                             *
 * Disclaimer: This software is provided "as is". Use it at your own risk.     *
 *                                                                             *
 *******************************************************************************
 *                                                                             *
 * ManyEars has been created and developped at:                                *
 *                                                                             *
 * IntroLab, Universite de Sherbrooke, Sherbrooke, Quebec, Canada              *
 *                                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * The following articles relate to the ManyEars project:                      *
 *                                                                             *
 * S. Brière, J.-M. Valin, F. Michaud, Dominic Létourneau, Embedded Auditory   *
 *     System for Small Mobile Robots, Proc. International Conference on       *
 *     Robotics and Automation (ICRA), 2008.                                   *
 *                                                                             *
 * J.-M. Valin, S. Yamamoto, J. Rouat, F. Michaud, K. Nakadai, H. G. Okuno,    *
 *     Robust Recognition of Simultaneous Speech By a Mobile Robot, IEEE       *
 *     Transactions on Robotics, Vol. 23, No. 4, pp. 742-752, 2007.            *
 *                                                                             *
 * J.-M. Valin, F. Michaud, J. Rouat, Robust Localization and Tracking of      *
 *     Simultaneous Moving Sound Sources Using Beamforming and Particle        *
 *     Filtering. Robotics and Autonomous Systems Journal (Elsevier), Vol. 55, *
 *     No. 3, pp. 216-228, 2007.                                               *
 *                                                                             *
 * S. Yamamoto, K. Nakadai, M. Nakano, H. Tsujino, J.-M. Valin, K. Komatani,   *
 *     T. Ogata, H. G. Okuno, Simultaneous Speech Recognition based on         *
 *     Automatic Missing-Feature Mask Generation integrated with Sound Source  *
 *     Separation (in Japanese). Journal of Robotic Society of Japan, Vol. 25, *
 *     No. 1, 2007.                                                            *
 *                                                                             *
 * J.-M. Valin, F. Michaud, J. Rouat, Robust 3D Localization and Tracking of   *
 *     Sound Sources Using Beamforming and Particle Filtering. Proc. IEEE      *
 *     International Conference on Acoustics, Speech and Signal Processing     *
 *     (ICASSP), pp. 841-844, 2006.                                            *
 *                                                                             *
 * S. Briere, D. Letourneau, M. Frechette, J.-M. Valin, F. Michaud, Embedded   *
 *     and integration audition for a mobile robot. Proceedings AAAI Fall      *
 *     Symposium Workshop Aurally Informed Performance: Integrating Machine    *
 *     Listening and Auditory Presentation in Robotic Systems, FS-06-01, 6-10, *
 *     2006                                                                    *
 *                                                                             *
 * S. Yamamoto, K. Nakadai, J.-M. Valin, J. Rouat, F. Michaud, K. Komatani,    *
 *     T. Ogata, H. G. Okuno, Making a robot recognize three simultaneous      *
 *     sentences in real-time. Proceedings of IEEE/RSJ International           *
 *     Conference on Intelligent Robots and Systems (IROS), 2005.              *
 *                                                                             *
 * M. Murase, S. Yamamoto, J.-M. Valin, K. Nakadai, K. Yamada, K. Komatani,    *
 *     T. Ogata, H. G. Okuno, Multiple Moving Speaker Tracking by Microphone   *
 *     Array on Mobile Robot. Proc. European Conference on Speech              *
 *     Communication and Technology (Interspeech), 2005.                       *
 *                                                                             *
 * S. Yamamoto, J.-M. Valin, K. Nakadai, J. Rouat, F. Michaud, T. Ogata, H.    *
 *     G. Okuno, Enhanced Robot Speech Recognition Based on Microphone Array   *
 *     Source Separation and Missing Feature Theory. Proc. International       *
 *     Conference on Robotics and Automation (ICRA), 2005.                     *
 *                                                                             *
 * J.-M. Valin, J. Rouat, F. Michaud, Enhanced Robot Audition Based on         *
 *     Microphone Array Source Separation with Post-Filter. Proc. IEEE/RSJ     *
 *     International Conference on Intelligent Robots and Systems (IROS),      *
 *     pp. 2123-2128, 2004.                                                    *
 *                                                                             *
 * J.-M. Valin, F. Michaud, J. Rouat, D. Létourneau, Robust Sound Source       *
 *     Localization Using a Microphone Array on a Mobile Robot. Proc. IEEE/RSJ *
 *     International Conference on Intelligent Robots and Systems (IROS),      *
 *     pp. 1228-1233, 2003.                                                    *
 *                                                                             *
 * J.-M. Valin, F. Michaud, B. Hadjou, J. Rouat, Localization of Simultaneous  *
 *     Moving Sound Sources for Mobile Robot Using a Frequency-Domain Steered  *
 *     Beamformer Approach. Proc. IEEE International Conference on Robotics    *
 *     and Automation (ICRA), pp. 1033-1038, 2004.                             *
 *                                                                             *
 * J.-M. Valin, J. Rouat, F. Michaud, Microphone Array Post-Filter for         *
 *     Separation of Simultaneous Non-Stationary Sources. Proc. IEEE           *
 *     International Conference on Acoustics, Speech and Signal Processing     *
 *     (ICASSP), pp. 221-224, 2004.                                            *
 *                                                                             *
 ******************************************************************************/

#include "parameters.h"

/*******************************************************************************
 * ParametersLoadDefault                                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      (none)                                                         *
 *                                                                             *
 * Outputs:     parametersStruct    Structure to be loaded with the parameters *
 *                                                                             *
 * Description: This function loads the default parameters in the structure.   *
 *                                                                             *
 ******************************************************************************/

void ParametersLoadDefault(struct ParametersStruct* parametersStruct)
{

    // +-----------------------------------------------------------------------+
    // | A. General                                                            |
    // +-----------------------------------------------------------------------+

    // Maximum number of tracked/separated sources
    parametersStruct->P_GEN_DYNSOURCES = 8;

    // +-----------------------------------------------------------------------+
    // | A. Geometry                                                           |
    // +-----------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | I. Microphones                                                |
            // +---------------------------------------------------------------+

                    // +-------------------------------------------------------+
                    // | i. Microphone 1                                       |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_GEO_MICS_MIC1_GAIN = 1.0000f;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_GEO_MICS_MIC1_X = -0.16f;
                                        parametersStruct->P_GEO_MICS_MIC1_Y = +0.16f;
                                        parametersStruct->P_GEO_MICS_MIC1_Z = -0.16f;

                    // +-------------------------------------------------------+
                    // | ii. Microphone 2                                      |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_GEO_MICS_MIC2_GAIN = 1.0000f;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_GEO_MICS_MIC2_X = -0.16f;
                                        parametersStruct->P_GEO_MICS_MIC2_Y = +0.16f;
                                        parametersStruct->P_GEO_MICS_MIC2_Z = +0.16f;

                    // +-------------------------------------------------------+
                    // | iii. Microphone 3                                     |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_GEO_MICS_MIC3_GAIN = 1.0000f;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_GEO_MICS_MIC3_X = +0.16f;
                                        parametersStruct->P_GEO_MICS_MIC3_Y = +0.16f;
                                        parametersStruct->P_GEO_MICS_MIC3_Z = -0.16f;

                    // +-------------------------------------------------------+
                    // | iv. Microphone 4                                      |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_GEO_MICS_MIC4_GAIN = 1.0000f;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_GEO_MICS_MIC4_X = +0.16f;
                                        parametersStruct->P_GEO_MICS_MIC4_Y = +0.16f;
                                        parametersStruct->P_GEO_MICS_MIC4_Z = +0.16f;

                    // +-------------------------------------------------------+
                    // | v. Microphone 5                                       |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_GEO_MICS_MIC5_GAIN = 1.0000f;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_GEO_MICS_MIC5_X = -0.16f;
                                        parametersStruct->P_GEO_MICS_MIC5_Y = -0.16f;
                                        parametersStruct->P_GEO_MICS_MIC5_Z = -0.16f;

                    // +-------------------------------------------------------+
                    // | vi. Microphone 6                                      |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_GEO_MICS_MIC6_GAIN = 1.0000f;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_GEO_MICS_MIC6_X = -0.16f;
                                        parametersStruct->P_GEO_MICS_MIC6_Y = -0.16f;
                                        parametersStruct->P_GEO_MICS_MIC6_Z = +0.16f;

                    // +-------------------------------------------------------+
                    // | vii. Microphone 7                                     |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_GEO_MICS_MIC7_GAIN = 1.0000f;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_GEO_MICS_MIC7_X = +0.16f;
                                        parametersStruct->P_GEO_MICS_MIC7_Y = -0.16f;
                                        parametersStruct->P_GEO_MICS_MIC7_Z = -0.16f;

                    // +-------------------------------------------------------+
                    // | viii. Microphone 8                                    |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_GEO_MICS_MIC8_GAIN = 1.0000f;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_GEO_MICS_MIC8_X = +0.16f;
                                        parametersStruct->P_GEO_MICS_MIC8_Y = -0.16f;
                                        parametersStruct->P_GEO_MICS_MIC8_Z = +0.16f;

    // +-----------------------------------------------------------------------+
    // | B. Beamformer                                                         |
    // +-----------------------------------------------------------------------+


            // +---------------------------------------------------------------+
            // | I. Potential sources                                          |
            // +---------------------------------------------------------------+

                    // Define the maximum number of sources that can be found
                    parametersStruct->P_BF_MAXSOURCES = 4;

                    // Define the range where the neighbour delays are used to refine
                    // the result
                    parametersStruct->P_BF_FILTERRANGE = 5;

                    // Define the number of delays next to the main delay to set to zero
                    // to find the peaks after the first one
                    parametersStruct->P_BF_RESETRANGE = 5;

                    // Define the energy level
                    parametersStruct->P_BF_ET = 600.0f;

    // +-----------------------------------------------------------------------+
    // | C. Particle filters                                                   |
    // +-----------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | I. Standard deviation                                         |
            // +---------------------------------------------------------------+

                    // Standard deviation of the new particles
                    parametersStruct->P_FILTER_STDDEVIATION = 0.2f;

            // +---------------------------------------------------------------+
            // | II. Particle prediction                                       |
            // +---------------------------------------------------------------+

                    // +-------------------------------------------------------+
                    // | i. Stopped                                            |
                    // +-------------------------------------------------------+

                            // Stationary sources
                            parametersStruct->P_FILTER_ALPHASTOP = 2.0f;
                            parametersStruct->P_FILTER_BETASTOP = 0.04f;

                    // +-------------------------------------------------------+
                    // | ii. Constant velocity                                 |
                    // +-------------------------------------------------------+

                            // Constant velocity source
                            parametersStruct->P_FILTER_ALPHACONST = 0.05f;
                            parametersStruct->P_FILTER_BETACONST = 0.5f;

                    // +-------------------------------------------------------+
                    // | iii. Accelerated                                      |
                    // +-------------------------------------------------------+

                            // Accelerated source
                            parametersStruct->P_FILTER_ALPHAEXC = 0.5f;
                            parametersStruct->P_FILTER_BETAEXC = 0.2f;

            // +---------------------------------------------------------------+
            // | III. Particle internia                                        |
            // +---------------------------------------------------------------+

                    // Additional inertia factors
                    parametersStruct->P_FILTER_INERTIAX = 1.0f;
                    parametersStruct->P_FILTER_INERTIAY = 1.0f;
                    parametersStruct->P_FILTER_INERTIAZ = 1.0f;

            // +---------------------------------------------------------------+
            // | IV. Others                                                    |
            // +---------------------------------------------------------------+

                    // Time interval between updates
                    parametersStruct->P_FILTER_DELTAT = 0.008f;


                    // Percentage of particles to have its state updated
                    parametersStruct->P_FILTER_STATEUPDT = 0.03f;


                    // Percentage of new particles to be stopped
                    // (make sure total sums up to 1 (100%))
                    parametersStruct->P_FILTER_NEWSTOP = 0.50f;


                    // Percentage of new particles to have a constant velocity
                    // (make sure total sums up to 1 (100%))
                    parametersStruct->P_FILTER_NEWCONST = 0.20f;


                    // Percentage of new particles to be excided
                    // (make sure total sums up to 1 (100%))
                    parametersStruct->P_FILTER_NEWEXC = 0.30f;


                    // Probability that the source was active and is still active:
                    // P(Aj(t)|Aj(t-1))
                    parametersStruct->P_FILTER_AJT_AJTM1 = 0.7; //0.95f;


                    // Probability that the source was not active and becomes active:
                    // P(Aj(t)|¬Aj(t-1))
                    parametersStruct->P_FILTER_AJT_NOTAJTM1 = 0.3; //0.05f;


                    // Probability that a source is not observed even if it exists:
                    // P0
                    parametersStruct->P_FILTER_P0 = 0.5f;


                    // This threshold must be reached in order to resample the filter
                    parametersStruct->P_FILTER_RSTHRESHOLD = 0.7f;


                    // Number of past values to put into buffer
                    parametersStruct->P_FILTER_BUFFERSIZE = 1;

    // +-----------------------------------------------------------------------+
    // | D. Mixture                                                            |
    // +-----------------------------------------------------------------------+

            // Probability that a new source appears: Pnew
            parametersStruct->P_MIXTURE_PNEW = 0.005f;

            // Probability that a false detection occurs
            parametersStruct->P_MIXTURE_PFALSE = 0.05f;

                // +-----------------------------------------------------------+
                // | I. New source                                             |
                // +-----------------------------------------------------------+

                        // Threshold to reach in order to track a new source
                        parametersStruct->P_MIXTURE_NEWTHRESHOLD = 0.5f; //0.7f;

                        // +---------------------------------------------------+
                        // | i. Confirm source exists                          |
                        // +---------------------------------------------------+

                                // Threshold to reach in order to confirm a source really exists
                                parametersStruct->P_MIXTURE_CONFIRMEXISTS = 0.7f;

                                // Threshold to count a source as existing
                                parametersStruct->P_MIXTURE_CONFIRMCOUNTTS = 0.5f;

                                // Number of times the threshold must be reached
                                parametersStruct->P_MIXTURE_CONFIRMCOUNT = 50;

                        // Minimum horizontal angle difference that the new source needs to
                        // have with the already existing filters (rad)
                        parametersStruct->P_MIXTURE_NEWANGLE = 1.0f;

                // +-----------------------------------------------------------+
                // | II. Delete source                                         |
                // +-----------------------------------------------------------+

                        // +---------------------------------------------------+
                        // | i. Probation                                      |
                        // +---------------------------------------------------+

                                // Maximum number of frames while the source has not been tracked
                                // in order to delete this tracking for probation time
                                parametersStruct->P_MIXTURE_CUMULATIVETIMEPROB = 50;

                                // Minimum value to consider to say that source is not observed for
                                // probation time
                                parametersStruct->P_MIXTURE_TOBSPROB = 0.95f; //0.5f;

                        // +---------------------------------------------------+
                        // | i. Level 1                                        |
                        // +---------------------------------------------------+

                                // Maximum number of frames while the source has not been tracked
                                // in order to delete this tracking for time 1
                                parametersStruct->P_MIXTURE_CUMULATIVETIME1 = 25;//100;

                                // Minimum value to consider to say that source is not observed for
                                // time 1
                                parametersStruct->P_MIXTURE_TOBS1 = 0.5f;

                        // +---------------------------------------------------+
                        // | i. Level 2                                        |
                        // +---------------------------------------------------+

                                // Maximum number of frames while the source has not been tracked
                                // in order to delete this tracking for time 2
                                parametersStruct->P_MIXTURE_CUMULATIVETIME2 = 50;//200;

                                // Minimum value to consider to say that source is not observed for
                                // time 2
                                parametersStruct->P_MIXTURE_TOBS2 = 0.95f; //0.7f;

            // Adaptation rate (alphaD)
            parametersStruct->P_MICST_ALPHAD = 0.1f;

            // Reverberation decay (gamma)
            parametersStruct->P_MICST_GAMMA = 0.3f; //0.1f;

            // Level of reverberation (delta)
            parametersStruct->P_MICST_DELTA = 1.0f;

            // +---------------------------------------------------------------+
            // | I. MCRA                                                       |
            // +---------------------------------------------------------------+

            // Coefficient to smooth the spectrum in time (alphaS)
            parametersStruct->P_MCRA_ALPHAS = 0.95f;

            // Probability smoothing parameter (alphaP)
            parametersStruct->P_MCRA_ALPHAP = 0.2f;

            // Constant smoothing parameter (alphaD)
            parametersStruct->P_MCRA_ALPHAD = 0.95f;

            // Maximum number of frames to refresh (L)
            parametersStruct->P_MCRA_L = 150;

            // Decision parameter (delta)
            parametersStruct->P_MCRA_DELTA = 0.01f;

    // +-----------------------------------------------------------------------+
    // | E. Geometric Source Separation (GSS)                                  |
    // +-----------------------------------------------------------------------+

            // Distance of each separated source from the center
            // of the cube
            parametersStruct->P_GSS_SOURCEDISTANCE = 3.0f;

            // Adaptation rate
            parametersStruct->P_GSS_MU = 0.001f;

            // Regularisation parameter
            parametersStruct->P_GSS_LAMBDA = 0.5f;

    // +-----------------------------------------------------------------------+
    // | F. Postfilter                                                         |
    // +-----------------------------------------------------------------------+

            // AlphaS: smoothing
            parametersStruct->P_POSTFILTER_ALPHAS = 0.3f;

            // Eta: reducing factor
            parametersStruct->P_POSTFILTER_ETA = 0.3f;

            // Gamma: Reverberation time
            parametersStruct->P_POSTFILTER_GAMMA = 0.3f;

            // Delta: Signal-to-reverberation ratio
            parametersStruct->P_POSTFILTER_DELTA = 1.0f;

            // Teta: Local
            parametersStruct->P_POSTFILTER_TETA_LOCAL = 0.01f;

            // Teta: Global
            parametersStruct->P_POSTFILTER_TETA_GLOBAL = 0.1f;

            // Teta: Frame
            parametersStruct->P_POSTFILTER_TETA_FRAME = 0.5f;

            // Alpha_zeta
            parametersStruct->P_POSTFILTER_ALPHAZETA = 0.3f;

            // Maximum a priori probability of speech absence
            parametersStruct->P_POSTFILTER_MAXQ = 0.9f;

            // Minimum gain allowed when speech is absent
            parametersStruct->P_POSTFILTER_GMIN = 0.1f;

            // Size of the local window
            parametersStruct->P_POSTFILTER_LOCALWINDOWSIZE = 3;

            // Size of the global window
            parametersStruct->P_POSTFILTER_GLOBALWINDOWSIZE = 31;

            // Minimum value of alphaP
            parametersStruct->P_POSTFILTER_ALPHAPMIN = 0.07f;


    // +-----------------------------------------------------------------------+
    // | G. Post-processing                                                    |
    // +-----------------------------------------------------------------------+

    // +-----------------------------------------------------------------------+
    // | H. Output                                                             |
    // +-----------------------------------------------------------------------+

            // Gain
            parametersStruct->P_OUT_GAIN = 5.0f;

            // Size of the wave header
            parametersStruct->P_OUT_WAVEHEADERSIZE = 100;


            // Minimum duration of a separated signal
            parametersStruct->P_OUT_MINDURATION = 5 * GLOBAL_FS;

            // Interval of separated signal
            parametersStruct->P_OUT_INTERVALDURATION = 1 * GLOBAL_FS;

}

