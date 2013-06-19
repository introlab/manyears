/*******************************************************************************
 * ManyEars: Parameters - Header                                               *
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

#ifndef PARAMETERS_H
#define PARAMETERS_H

// =============================================================================

/*******************************************************************************
 * Global parameters                                                           *
 ******************************************************************************/

// Number of samples per frame (need to be a power of 2)
#define     GLOBAL_FRAMESIZE     1024

// Number of samples per frame to allow zero padding (need to be a power of 2)
#define     GLOBAL_LONGFRAMESIZE 2048

// Frame overlap (1 = 100%, 0.5 = 50%, 0.25 = 25%, ...)
#define     GLOBAL_OVERLAP       0.5

// Speed of sound (in meters / sec)
#define     GLOBAL_C             343

// Sampling rate (in samples / sec)
#define     GLOBAL_FS            48000

// MCRA: Size of the window to smooth spectrum in frequency
#define     GLOBAL_BSIZE         3

// MCRA: Window to smooth spectrum in frequency
#define     GLOBAL_WINDOW0       0.25
#define     GLOBAL_WINDOW1       0.50
#define     GLOBAL_WINDOW2       0.25

// Sphere: Number of levels
#define     GLOBAL_SPHERE_NUMBERLEVELS 4

// Number of microphones
#define     GLOBAL_MICSNUMBER   8

// Number of particles in filters
#define     GLOBAL_NBPARTICLES  500

/*******************************************************************************
 * List of all the parameters that can be changed dynamically                  *
 ******************************************************************************/

struct ParametersStruct{

    // *********************************************************************
    // * General                                                           *
    // *********************************************************************

    // Maximum number of tracked/separated sources
    int P_GEN_DYNSOURCES;

    // *********************************************************************
    // * Geometry                                                          *
    // *********************************************************************

    // Define the position of each microphone on the cube (in meters)
    // The center of the cube is at the origin
    float P_GEO_MICS_MIC1_X;
    float P_GEO_MICS_MIC1_Y;
    float P_GEO_MICS_MIC1_Z;
    float P_GEO_MICS_MIC2_X;
    float P_GEO_MICS_MIC2_Y;
    float P_GEO_MICS_MIC2_Z;
    float P_GEO_MICS_MIC3_X;
    float P_GEO_MICS_MIC3_Y;
    float P_GEO_MICS_MIC3_Z;
    float P_GEO_MICS_MIC4_X;
    float P_GEO_MICS_MIC4_Y;
    float P_GEO_MICS_MIC4_Z;
    float P_GEO_MICS_MIC5_X;
    float P_GEO_MICS_MIC5_Y;
    float P_GEO_MICS_MIC5_Z;
    float P_GEO_MICS_MIC6_X;
    float P_GEO_MICS_MIC6_Y;
    float P_GEO_MICS_MIC6_Z;
    float P_GEO_MICS_MIC7_X;
    float P_GEO_MICS_MIC7_Y;
    float P_GEO_MICS_MIC7_Z;
    float P_GEO_MICS_MIC8_X;
    float P_GEO_MICS_MIC8_Y;
    float P_GEO_MICS_MIC8_Z;

    // Define the gain of each microphone to equalize
    float P_GEO_MICS_MIC1_GAIN;
    float P_GEO_MICS_MIC2_GAIN;
    float P_GEO_MICS_MIC3_GAIN;
    float P_GEO_MICS_MIC4_GAIN;
    float P_GEO_MICS_MIC5_GAIN;
    float P_GEO_MICS_MIC6_GAIN;
    float P_GEO_MICS_MIC7_GAIN;
    float P_GEO_MICS_MIC8_GAIN;

    // *********************************************************************
    // * Beamformer                                                        *
    // *********************************************************************

    // Define the maximum number of sources that can be found
    int P_BF_MAXSOURCES;

    // Define the range where the neighbour delays are used to refine
    // the result
    int P_BF_FILTERRANGE;

    // Define the number of delays next to the main delay to set to zero
    // to find the peaks after the first one
    int P_BF_RESETRANGE;

    // Threshold in order to get Pq from beamformer values
    float P_BF_ET;

    // *********************************************************************
    // * Filter                                                            *
    // *********************************************************************

    // Standard deviation of the new particles
    float P_FILTER_STDDEVIATION;

    // Stationary sources
    float P_FILTER_ALPHASTOP;
    float P_FILTER_BETASTOP;

    // Constant velocity source
    float P_FILTER_ALPHACONST;
    float P_FILTER_BETACONST;

    // Accelerated source
    float P_FILTER_ALPHAEXC;
    float P_FILTER_BETAEXC;

    // Additional inertia factors
    float P_FILTER_INERTIAX;
    float P_FILTER_INERTIAY;
    float P_FILTER_INERTIAZ;

    // Time interval between updates
    float P_FILTER_DELTAT;

    // Percentage of particles to have its state updated
    float P_FILTER_STATEUPDT;

    // Percentage of new particles to be stopped
    // (make sure total sums up to 1 (100%))
    float P_FILTER_NEWSTOP;

    // Percentage of new particles to have a constant velocity
    // (make sure total sums up to 1 (100%))
    float P_FILTER_NEWCONST;

    // Percentage of new particles to be excided
    // (make sure total sums up to 1 (100%))
    float P_FILTER_NEWEXC;

    // Probability that the source was active and is still active:
    // P(Aj(t)|Aj(t-1))
    float P_FILTER_AJT_AJTM1;

    // Probability that the source was not active and becomes active:
    // P(Aj(t)|¬Aj(t-1))
    float P_FILTER_AJT_NOTAJTM1;

    // Probability that a source is not observed even if it exists:
    // P0
    float P_FILTER_P0;

    // This threshold must be reached in order to resample the filter
    float P_FILTER_RSTHRESHOLD;

    // Number of past values to put into buffer
    int P_FILTER_BUFFERSIZE;

    // *********************************************************************
    // * Mixture                                                           *
    // *********************************************************************

    // Probability that a new source appears: Pnew
    float P_MIXTURE_PNEW;

    // Probability that a false detection occurs
    float P_MIXTURE_PFALSE;

    // Threshold to reach in order to track a new source
    float P_MIXTURE_NEWTHRESHOLD;

    // Threshold to reach in order to confirm a source really exists
    float P_MIXTURE_CONFIRMEXISTS;

    // Threshold to count a source as existing
    float P_MIXTURE_CONFIRMCOUNTTS;

    // Number of times the threshold must be reached
    int P_MIXTURE_CONFIRMCOUNT;

    // Maximum number of frames while the source has not been tracked
    // in order to delete this tracking for probation time
    int P_MIXTURE_CUMULATIVETIMEPROB;

    // Maximum number of frames while the source has not been tracked
    // in order to delete this tracking for time 1
    int P_MIXTURE_CUMULATIVETIME1;

    // Maximum number of frames while the source has not been tracked
    // in order to delete this tracking for time 2
    int P_MIXTURE_CUMULATIVETIME2;

    // Minimum value to consider to say that source is not observed for
    // probation time
    float P_MIXTURE_TOBSPROB;

    // Minimum value to consider to say that source is not observed for
    // time 1
    float P_MIXTURE_TOBS1;

    // Minimum value to consider to say that source is not observed for
    // time 2
    float P_MIXTURE_TOBS2;

    // Minimum horizontal angle difference that the new source needs to
    // have with the already existing filters (rad)
    float P_MIXTURE_NEWANGLE;

    // *********************************************************************
    // * Microphone sound track                                            *
    // *********************************************************************

    // Adaptation rate (alphaD)
    float P_MICST_ALPHAD;

    // Reverberation decay (gamma)
    float P_MICST_GAMMA;

    // Level of reverberation (delta)
    float P_MICST_DELTA;

    // Window size
    float P_MCRA_BSIZE;

    // Window
    float P_MCRA_B;

    // MCRA: Coefficient to smooth the spectrum in time (alphaS)
    float P_MCRA_ALPHAS;

    // MCRA: Probability smoothing parameter (alphaP)
    float P_MCRA_ALPHAP;

    // MCRA: Constant smoothing parameter (alphaD)
    float P_MCRA_ALPHAD;

    // MCRA: Maximum number of frames to refresh (L)
    int P_MCRA_L;

    // MCRA: Decision parameter (delta)
    float P_MCRA_DELTA;

    // *********************************************************************
    // * Geometric Source Separation (GSS)                                 *
    // *********************************************************************

    // Estimated distance between source and center of the cube
    float P_GSS_SOURCEDISTANCE;

    // Regularisation parameter (lambda)
    float P_GSS_LAMBDA;

    // Adaptation rate (mu)
    float P_GSS_MU;

    // *********************************************************************
    // * Postfilter                                                        *
    // *********************************************************************

    // AlphaS: smoothing
    float P_POSTFILTER_ALPHAS;

    // Eta: reducing factor
    float P_POSTFILTER_ETA;

    // Gamma: Reverberation time
    float P_POSTFILTER_GAMMA;

    // Delta: Signal-to-reverberation ratio
    float P_POSTFILTER_DELTA;

    // Teta: Local
    float P_POSTFILTER_TETA_LOCAL;

    // Teta: Global
    float P_POSTFILTER_TETA_GLOBAL;

    // Teta: Frame
    float P_POSTFILTER_TETA_FRAME;

    // Alpha_zeta
    float P_POSTFILTER_ALPHAZETA;

    // Maximum a priori probability of speech absence
    float P_POSTFILTER_MAXQ;

    // Minimum gain allowed when speech is absent
    float P_POSTFILTER_GMIN;

    // Size of the local window
    int P_POSTFILTER_LOCALWINDOWSIZE;

    // Size of the global window
    int P_POSTFILTER_GLOBALWINDOWSIZE;

    // Minimum value of alphaP
    float P_POSTFILTER_ALPHAPMIN;

    // *********************************************************************
    // * Post-processing                                                   *
    // *********************************************************************

    // *********************************************************************
    // * Output                                                            *
    // *********************************************************************

    // Gain
    float P_OUT_GAIN;

    // Size of the wave header
    int P_OUT_WAVEHEADERSIZE;

    // Minimum duration of a separated signal
    int P_OUT_MINDURATION;

    // Interval of separated signal
    int P_OUT_INTERVALDURATION;

};

/*******************************************************************************
 * Load default values                                                         *
 ******************************************************************************/

void ParametersLoadDefault(struct ParametersStruct* parametersStruct);

// =============================================================================

#endif // PARAMETERS_H
