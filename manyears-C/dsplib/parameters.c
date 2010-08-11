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
    // | A. Beamformer                                                         |
    // +-----------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | I. Microphones                                                |
            // +---------------------------------------------------------------+

                    // +-------------------------------------------------------+
                    // | i. Microphone 1                                       |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_BF_MICSGAIN_1 = 1.0476;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_BF_MICSPOSITIONS_1_X = -0.215;
                                        parametersStruct->P_BF_MICSPOSITIONS_1_Y = +0.1875;
                                        parametersStruct->P_BF_MICSPOSITIONS_1_Z = -0.15;

                    // +-------------------------------------------------------+
                    // | ii. Microphone 2                                      |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_BF_MICSGAIN_2 = 1.0000;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_BF_MICSPOSITIONS_2_X = -0.215;
                                        parametersStruct->P_BF_MICSPOSITIONS_2_Y = +0.1875;
                                        parametersStruct->P_BF_MICSPOSITIONS_2_Z = +0.15;

                    // +-------------------------------------------------------+
                    // | iii. Microphone 3                                     |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_BF_MICSGAIN_3 = 2.2288;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_BF_MICSPOSITIONS_3_X = +0.215;
                                        parametersStruct->P_BF_MICSPOSITIONS_3_Y = +0.1875;
                                        parametersStruct->P_BF_MICSPOSITIONS_3_Z = -0.15;

                    // +-------------------------------------------------------+
                    // | iv. Microphone 4                                      |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_BF_MICSGAIN_4 = 1.5242;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_BF_MICSPOSITIONS_4_X = +0.215;
                                        parametersStruct->P_BF_MICSPOSITIONS_4_Y = +0.1875;
                                        parametersStruct->P_BF_MICSPOSITIONS_4_Z = +0.15;

                    // +-------------------------------------------------------+
                    // | v. Microphone 5                                       |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_BF_MICSGAIN_5 = 1.3265;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_BF_MICSPOSITIONS_5_X = -0.215;
                                        parametersStruct->P_BF_MICSPOSITIONS_5_Y = -0.1875;
                                        parametersStruct->P_BF_MICSPOSITIONS_5_Z = -0.15;

                    // +-------------------------------------------------------+
                    // | vi. Microphone 6                                      |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_BF_MICSGAIN_6 = 1.9480;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_BF_MICSPOSITIONS_6_X = -0.215;
                                        parametersStruct->P_BF_MICSPOSITIONS_6_Y = -0.1875;
                                        parametersStruct->P_BF_MICSPOSITIONS_6_Z = +0.15;

                    // +-------------------------------------------------------+
                    // | vii. Microphone 7                                     |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_BF_MICSGAIN_7 = 1.0350;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_BF_MICSPOSITIONS_7_X = +0.215;
                                        parametersStruct->P_BF_MICSPOSITIONS_7_Y = -0.1875;
                                        parametersStruct->P_BF_MICSPOSITIONS_7_Z = -0.15;

                    // +-------------------------------------------------------+
                    // | viii. Microphone 8                                    |
                    // +-------------------------------------------------------+

                            // Define the gain of each microphone to equalize
                            parametersStruct->P_BF_MICSGAIN_8 = 1.9145;

                                // +-------------------------------------------+
                                // | a. Position                               |
                                // +-------------------------------------------+

                                        // Define the position of each microphone on the cube (in meters)
                                        // The center of the cube is at the origin
                                        parametersStruct->P_BF_MICSPOSITIONS_8_X = +0.215;
                                        parametersStruct->P_BF_MICSPOSITIONS_8_Y = -0.1875;
                                        parametersStruct->P_BF_MICSPOSITIONS_8_Z = +0.15;



            // +---------------------------------------------------------------+
            // | II. Potential sources                                         |
            // +---------------------------------------------------------------+

                    // Define the maximum number of sources that can be found
                    parametersStruct->P_BF_MAXSOURCES = 4;


                    // Define the number of maxima to keep in memory in order to find
                    // the one that best fits
                    parametersStruct->P_BF_NUMBERMAX = 50;


                    // Define the value threshold for the maxima comparison
                    parametersStruct->P_BF_VALUETS = 0.85;


                    // Define the horizontal angle tolerance for first source
                    parametersStruct->P_BF_HANGLETOL = 0.2;


                    // Define the horizontal angle tolerance for next sources
                    parametersStruct->P_BF_HANGLETOLNEXT = 1;


                    // Define the vertical angle tolerance
                    parametersStruct->P_BF_VANGLETOL = 0;


                    // Define the range where the neighbour delays are used to refine
                    // the result
                    parametersStruct->P_BF_RANGE = 6;


                    // Define the number of delays next to the main delay to set to zero
                    // to find the peaks after the first one
                    parametersStruct->P_BF_RIJDELAYS = 3;

    // +-----------------------------------------------------------------------+
    // | B. Particle filters                                                   |
    // +-----------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | I. Standard deviation                                         |
            // +---------------------------------------------------------------+

                    // Standard deviation of the new particles
                    parametersStruct->P_FILTER_STDDEVIATION = 0.1;

            // +---------------------------------------------------------------+
            // | II. Particle prediction                                       |
            // +---------------------------------------------------------------+

                    // +-------------------------------------------------------+
                    // | i. Stopped                                            |
                    // +-------------------------------------------------------+

                            // Stationary sources
                            parametersStruct->P_FILTER_ALPHASTOP = 2.0;
                            parametersStruct->P_FILTER_BETASTOP = 0.04;

                    // +-------------------------------------------------------+
                    // | ii. Constant velocity                                 |
                    // +-------------------------------------------------------+

                            // Constant velocity source
                            parametersStruct->P_FILTER_ALPHACONST = 0.05;
                            parametersStruct->P_FILTER_BETACONST = 0.5;

                    // +-------------------------------------------------------+
                    // | iii. Accelerated                                      |
                    // +-------------------------------------------------------+

                            // Accelerated source
                            parametersStruct->P_FILTER_ALPHAEXC = 0.5;
                            parametersStruct->P_FILTER_BETAEXC = 0.2;

            // +---------------------------------------------------------------+
            // | III. Particle internia                                        |
            // +---------------------------------------------------------------+

                    // Additional inertia factors
                    parametersStruct->P_FILTER_INERTIAX = 1.5;
                    parametersStruct->P_FILTER_INERTIAY = 1.5;
                    parametersStruct->P_FILTER_INERTIAZ = 0.5;

            // +---------------------------------------------------------------+
            // | IV. Others                                                    |
            // +---------------------------------------------------------------+

                    // Time interval between updates
                    parametersStruct->P_FILTER_DELTAT = 0.004;


                    // Percentage of particles to have its state updated
                    parametersStruct->P_FILTER_STATEUPDT = 0.03;


                    // Percentage of new particles to be stopped
                    // (make sure total sums up to 1 (100%))
                    parametersStruct->P_FILTER_NEWSTOP = 0.10;


                    // Percentage of new particles to have a constant velocity
                    // (make sure total sums up to 1 (100%))
                    parametersStruct->P_FILTER_NEWCONST = 0.40;


                    // Percentage of new particles to be excided
                    // (make sure total sums up to 1 (100%))
                    parametersStruct->P_FILTER_NEWEXC = 0.50;


                    // Probability that the source was active and is still active:
                    // P(Aj(t)|Aj(t-1))
                    parametersStruct->P_FILTER_AJT_AJTM1 = 0.95;


                    // Probability that the source was not active and becomes active:
                    // P(Aj(t)|¬Aj(t-1))
                    parametersStruct->P_FILTER_AJT_NOTAJTM1 = 0.05;


                    // Probability that a source is not observed even if it exists:
                    // P0
                    parametersStruct->P_FILTER_P0 = 0.2;


                    // This threshold must be reached in order to resample the filter
                    parametersStruct->P_FILTER_RSTHRESHOLD = 0.7;


                    // Number of past values to put into buffer
                    parametersStruct->P_FILTER_BUFFERSIZE = 50;

    // +-----------------------------------------------------------------------+
    // | C. Mixture                                                            |
    // +-----------------------------------------------------------------------+

            // Maximum number of filter
            parametersStruct->P_MIXTURE_NBFILTERS = 8;

            // Probability that a new source appears: Pnew
            parametersStruct->P_MIXTURE_PNEW = 0.005;

            // Probability that a false detection occurs
            parametersStruct->P_MIXTURE_PFALSE = 0.05;

            // Threshold in order to get Pq from beamformer values
            parametersStruct->P_MIXTURE_ET = 4.0;

                // +-----------------------------------------------------------+
                // | I. New source                                             |
                // +-----------------------------------------------------------+

                        // Threshold to reach in order to track a new source
                        parametersStruct->P_MIXTURE_NEWTHRESHOLD = 0.6;

                        // +---------------------------------------------------+
                        // | i. Confirm source exists                          |
                        // +---------------------------------------------------+

                                // Threshold to reach in order to confirm a source really exists
                                parametersStruct->P_MIXTURE_CONFIRMEXISTS = 0.98;

                                // Threshold to count a source as existing
                                parametersStruct->P_MIXTURE_CONFIRMCOUNTTS = 0.8;

                                // Number of times the threshold must be reached
                                parametersStruct->P_MIXTURE_CONFIRMCOUNT = 20;

                        // Minimum horizontal angle difference that the new source needs to
                        // have with the already existing filters (rad)
                        parametersStruct->P_MIXTURE_NEWANGLE = 0.6;

                // +-----------------------------------------------------------+
                // | II. Delete source                                         |
                // +-----------------------------------------------------------+

                        // +---------------------------------------------------+
                        // | i. Probabtion                                     |
                        // +---------------------------------------------------+

                                // Maximum number of frames while the source has not been tracked
                                // in order to delete this tracking for probation time
                                parametersStruct->P_MIXTURE_CUMULATIVETIMEPROB = 10;

                                // Minimum value to consider to say that source is not observed for
                                // probation time
                                parametersStruct->P_MIXTURE_TOBSPROB = 0.5;

                        // +---------------------------------------------------+
                        // | i. Level 1                                        |
                        // +---------------------------------------------------+

                                // Maximum number of frames while the source has not been tracked
                                // in order to delete this tracking for time 1
                                parametersStruct->P_MIXTURE_CUMULATIVETIME1 = 100;

                                // Minimum value to consider to say that source is not observed for
                                // time 1
                                parametersStruct->P_MIXTURE_TOBS1 = 0.95;

                        // +---------------------------------------------------+
                        // | i. Level 2                                        |
                        // +---------------------------------------------------+

                                // Maximum number of frames while the source has not been tracked
                                // in order to delete this tracking for time 2
                                parametersStruct->P_MIXTURE_CUMULATIVETIME2 = 200;

                                // Minimum value to consider to say that source is not observed for
                                // time 2
                                parametersStruct->P_MIXTURE_TOBS2 = 0.995;

    // +-----------------------------------------------------------------------+
    // | D. Mixture                                                            |
    // +-----------------------------------------------------------------------+

            // Adaptation rate (alphaD)
            parametersStruct->P_MICST_ALPHAD = 0.1;

            // Reverberation decay (gamma)
            parametersStruct->P_MICST_GAMMA = 0.3;

            // Level of reverberation (delta)
            parametersStruct->P_MICST_DELTA = 1.0;

            // +---------------------------------------------------------------+
            // | I. MCRA                                                       |
            // +---------------------------------------------------------------+

            // Coefficient to smooth the spectrum in time (alphaS)
            parametersStruct->P_MCRA_ALPHAS = 0.95;

            // Probability smoothing parameter (alphaP)
            parametersStruct->P_MCRA_ALPHAP = 0.2;

            // Constant smoothing parameter (alphaD)
            parametersStruct->P_MCRA_ALPHAD = 0.95;

            // Maximum number of frames to refresh (L)
            parametersStruct->P_MCRA_L = 150;

            // Decision parameter (delta)
            parametersStruct->P_MCRA_DELTA = 3.0;

}

