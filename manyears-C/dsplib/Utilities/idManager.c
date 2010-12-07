/*******************************************************************************
 * ManyEars: idManager - Source Code                                           *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: November 1st , 2010                                                   *
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

#include "Utilities/idManager.h"

/*******************************************************************************
 * idManagerInit                                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIDManager     The object to be initialized                   *
 *                                                                             *
 * Outputs:     myIDManager     The initialized object                         *
 *                                                                             *
 * Description: This function initializes the objIDManager object              *
 *                                                                             *
 ******************************************************************************/

void idManagerInit(struct objIDManager* myIDManager)
{

    unsigned int nObjects;
    unsigned int indexObject;

    struct objID* nextPtr;
    ID_TYPE currentID;

    // Find the number of ID objects
    nObjects = (ID_MAX - ID_MIN) + 1;

    // Create the array to hold all the ID objects
    myIDManager->listIDs = (struct objID*) newTable1D(nObjects, sizeof(struct objID));

    // Instantiate all ID objects
    currentID = ID_MIN;

    for (indexObject = 0; indexObject < nObjects; indexObject++)
    {

        if (indexObject < (nObjects - 1))
        {
            nextPtr = &myIDManager->listIDs[indexObject+1];
        }
        else
        {
            nextPtr = ID_NULLPTR;
        }

        myIDManager->listIDs[indexObject].id = currentID;
        myIDManager->listIDs[indexObject].next = nextPtr;

        currentID++;

    }

    // Instantiate first pointers
    myIDManager->firstUsed = 0x0;
    myIDManager->firstValid = myIDManager->listIDs;

    // Instantiate last pointer
    myIDManager->lastValid = &myIDManager->listIDs[nObjects - 1];

    // Instantiate counters
    myIDManager->nAvailableIDs = nObjects;
    myIDManager->nUsedIDs = 0;

}

/*******************************************************************************
 * idManagerTerminate                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIDManager     The object to be terminated                    *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the objIDManager object               *
 *                                                                             *
 ******************************************************************************/

void idManagerTerminate(struct objIDManager* myIDManager)
{

    // Free memory
    deleteTable1D(myIDManager->listIDs);

}

/*******************************************************************************
 * idManagerReserveID                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIDManager     The object to be used                          *
 *                                                                             *
 * Outputs:     (ID_TYPE)       The generated ID                               *
 *              myIDManager     The updated object                             *
 *                                                                             *
 * Description: This function returns an available ID                          *
 *                                                                             *
 ******************************************************************************/

ID_TYPE idManagerReserveID(struct objIDManager* myIDManager)
{

    struct objID* validPtr;
    struct objID* usedPtr;
    ID_TYPE newID;

    // Check if there are any available IDs
    if (myIDManager->nAvailableIDs > 0)
    {

        //
        // (firstValid) -> (id1) -> (id2) -> (id3) -> 0x0
        //
        // becomes:
        //
        // validPtr -> (id1)
        // (firstValid) -> (id2) -> (id3) -> 0x0
        //

        // Take the first valid ID
        validPtr = myIDManager->firstValid;

        // Point to the next valid ID
        myIDManager->firstValid = validPtr->next;

        //
        // Case 1:
        // -------
        //
        // (firstUsed) -> (idA) -> (idB) -> (idC) -> 0x0
        //
        // becomes:
        //
        // (firstUsed) -> (idA) -> (idB) -> (idC) -> (id1) -> 0x0
        //
        // Case 2:
        // -------
        //
        // (firstUsed) -> 0x0
        //
        // becomes:
        //
        // (firstUsed) -> (id1)
        //


        // Stack in the list of used ID

        if (myIDManager->firstUsed != ID_NULLPTR)
        {

            usedPtr = myIDManager->firstUsed;
            while(usedPtr->next != ID_NULLPTR)
            {
                usedPtr = usedPtr->next;
            }
            usedPtr->next = validPtr;

        }
        else
        {

            myIDManager->firstUsed = validPtr;

        }

        validPtr->next = ID_NULLPTR;

        // Update counters
        myIDManager->nAvailableIDs--;
        myIDManager->nUsedIDs++;

        // Check if some IDs are still available
        if (myIDManager->nAvailableIDs == 0)
        {
            myIDManager->firstValid = ID_NULLPTR;
            myIDManager->lastValid = ID_NULLPTR;
        }

        // New ID value
        newID = validPtr->id;

    }
    else
    {
        newID = ID_NOID;
    }

    return newID;

}

/*******************************************************************************
 * idManagerDeleteID                                                           *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIDManager     The object to be used                          *
 *              ID_TYPE         The ID to be removed                           *
 *                                                                             *
 * Outputs:     myIDManager     The updated object                             *
 *                                                                             *
 * Description: This function removes the ID from the list of used ID. The ID  *
 *              cannot be reused later (to ensure no two sources have the      *
 *              ID).                                                           *
 *                                                                             *
 ******************************************************************************/

void idManagerDeleteID(struct objIDManager* myIDManager, ID_TYPE ID)
{

    struct objID* usedPrevPtr;
    struct objID* usedPtr;

    //
    // Case 1:
    // -------
    //
    // (firstUsed) -> (id1) -> (idA) -> (idB) -> (idC) -> (idD) -> 0x0
    //
    // becomes:
    //
    // (firstUsed) -> (idA) -> (idB) -> (idC) -> (idD) -> 0x0
    //
    //
    // Case 2:
    // -------
    //
    // (firstUsed) -> (idA) -> (idB) -> (idC) -> (id1) -> (idD) -> 0x0
    //
    // becomes:
    //
    // (firstUsed) -> (idA) -> (idB) -> (idC) -> (idD) -> 0x0
    //

    usedPrevPtr = ID_NULLPTR;
    usedPtr = myIDManager->firstUsed;

    while(usedPtr != ID_NULLPTR)
    {
        if (usedPtr->id == ID)
        {
            // Remove pointer from the chain
            if (usedPrevPtr == ID_NULLPTR)
            {
                myIDManager->firstUsed = usedPtr->next;

            }
            else
            {
                usedPrevPtr->next = usedPtr->next;
            }

            break;

        }
        else
        {
            // Point to the next element
            usedPrevPtr = usedPtr;
            usedPtr = usedPtr->next;
        }
    }

}

/*******************************************************************************
 * idManagerRestoreID                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIDManager     The object to be used                          *
 *              ID_TYPE         The ID to be restored                          *
 *                                                                             *
 * Outputs:     myIDManager     The updated object                             *
 *                                                                             *
 * Description: This function returns the ID in the list of available ID. The  *
 *              ID can be reused later.                                        *
 *                                                                             *
 ******************************************************************************/

void idManagerRestoreID(struct objIDManager* myIDManager, ID_TYPE ID)
{

    struct objID* usedPtr;

    //
    // Case 1:
    // -------
    //
    // (firstValid) -> 0x0
    // (firstUsed) -> (idA) -> (idB) -> (idC) -> (id1) -> (idD) -> 0x0
    //
    // becomes:
    //
    // (firstValid) -> (id1) -> 0x0
    // (firstUsed) -> (idA) -> (idB) -> (idC) -> (idD) -> 0x0
    //
    //
    // Case 2:
    // -------
    //
    // (firstValid) -> (id2) -> (id3) -> 0x0
    // (firstUsed) -> (idA) -> (idB) -> (idC) -> (id1) -> (idD) -> 0x0
    //
    // becomes:
    //
    // (firstValid) -> (id2) -> (id3) -> (id1) -> 0x0
    // (firstUsed) -> (idA) -> (idB) -> (idC) -> (idD) -> 0x0
    //

    usedPtr = myIDManager->firstUsed;

    while(usedPtr != ID_NULLPTR)
    {
        if (usedPtr->id == ID)
        {

            if (myIDManager->firstValid == ID_NULLPTR)
            {
                myIDManager->firstValid = usedPtr;
                myIDManager->lastValid = usedPtr;
            }
            else
            {
                myIDManager->lastValid->next = usedPtr;
                myIDManager->lastValid = usedPtr;
            }

            break;

        }
        else
        {
            // Point to the next element
            usedPtr = usedPtr->next;
        }
    }

    // Then remove IDs from the used ID list
    idManagerDeleteID(myIDManager, ID);

    // Update counters
    myIDManager->nAvailableIDs++;
    myIDManager->nUsedIDs--;

    // Then point to a null pointer at the last valid
    // ID (this needs to be done after deleting to
    // avoid breaking the chain)
    myIDManager->lastValid->next = ID_NULLPTR;

}

/*******************************************************************************
 * idManagerPrintf                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIDManager     The object to be used                          *
 *                                                                             *
 * Outputs:     (none but text is printed in the terminal)                     *
 *                                                                             *
 * Description: This function prints the current status of the ID manager.     *
 *                                                                             *
 ******************************************************************************/

void idManagerPrintf(struct objIDManager* myIDManager)
{

    struct objID* usedPtr;
    struct objID* validPtr;

    // Print used ID

    printf("Used IDs: ");

    usedPtr = myIDManager->firstUsed;
    while (usedPtr != ID_NULLPTR)
    {
        printf("%i -> ", usedPtr->id);
        usedPtr = usedPtr->next;
    }

    printf("0x0 \n");

    // Print valid IDs

    printf("Valid IDs: ");

    validPtr = myIDManager->firstValid;
    while (validPtr != ID_NULLPTR)
    {
        printf("%i -> ", validPtr->id);
        validPtr = validPtr->next;
    }

    printf("0x0 \n");


}
