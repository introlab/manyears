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
 ******************************************************************************/

#ifndef PARAMETERS_H
#define PARAMETERS_H

// =============================================================================

/*******************************************************************************
 * Global parameters                                                           *
 ******************************************************************************/

// Number of samples per frame (need to be a power of 2)
#define     GLOBAL_FRAMESIZE     1024

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
    // * Beamformer                                                        *
    // *********************************************************************

    // Define the position of each microphone on the cube (in meters)
    // The center of the cube is at the origin
    float P_BF_MICSPOSITIONS_1_X;
    float P_BF_MICSPOSITIONS_1_Y;
    float P_BF_MICSPOSITIONS_1_Z;
    float P_BF_MICSPOSITIONS_2_X;
    float P_BF_MICSPOSITIONS_2_Y;
    float P_BF_MICSPOSITIONS_2_Z;
    float P_BF_MICSPOSITIONS_3_X;
    float P_BF_MICSPOSITIONS_3_Y;
    float P_BF_MICSPOSITIONS_3_Z;
    float P_BF_MICSPOSITIONS_4_X;
    float P_BF_MICSPOSITIONS_4_Y;
    float P_BF_MICSPOSITIONS_4_Z;
    float P_BF_MICSPOSITIONS_5_X;
    float P_BF_MICSPOSITIONS_5_Y;
    float P_BF_MICSPOSITIONS_5_Z;
    float P_BF_MICSPOSITIONS_6_X;
    float P_BF_MICSPOSITIONS_6_Y;
    float P_BF_MICSPOSITIONS_6_Z;
    float P_BF_MICSPOSITIONS_7_X;
    float P_BF_MICSPOSITIONS_7_Y;
    float P_BF_MICSPOSITIONS_7_Z;
    float P_BF_MICSPOSITIONS_8_X;
    float P_BF_MICSPOSITIONS_8_Y;
    float P_BF_MICSPOSITIONS_8_Z;

    // Define the gain of each microphone to equalize
    float P_BF_MICSGAIN_1;
    float P_BF_MICSGAIN_2;
    float P_BF_MICSGAIN_3;
    float P_BF_MICSGAIN_4;
    float P_BF_MICSGAIN_5;
    float P_BF_MICSGAIN_6;
    float P_BF_MICSGAIN_7;
    float P_BF_MICSGAIN_8;

    // Define the maximum number of sources that can be found
    int P_BF_MAXSOURCES;

    // Define the number of maxima to keep in memory in order to find
    // the one that best fits
    int P_BF_NUMBERMAX;

    // Define the value threshold for the maxima comparison
    float P_BF_VALUETS;

    // Define the horizontal angle tolerance for first source
    float P_BF_HANGLETOL;

    // Define the horizontal angle tolerance for next sources
    float P_BF_HANGLETOLNEXT;

    // Define the vertical angle tolerance
    float P_BF_VANGLETOL;

    // Define the range where the neighbour delays are used to refine
    // the result
    int P_BF_RANGE;

    // Define the number of delays next to the main delay to set to zero
    // to find the peaks after the first one
    int P_BF_RIJDELAYS;

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

    // Maximum number of filter
    int P_MIXTURE_NBFILTERS;

    // Probability that a new source appears: Pnew
    float P_MIXTURE_PNEW;

    // Probability that a false detection occurs
    float P_MIXTURE_PFALSE;

    // Threshold in order to get Pq from beamformer values
    float P_MIXTURE_ET;

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

};

/*******************************************************************************
 * Load default values                                                         *
 ******************************************************************************/

void ParametersLoadDefault(struct ParametersStruct* parametersStruct);

// =============================================================================

#endif // PARAMETERS_H
