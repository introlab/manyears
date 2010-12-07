/*******************************************************************************
 * ManyEars: idList - Source Code                                              *
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

#include "Utilities/idList.h"

/*******************************************************************************
 * idListInit                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdList        The object to be initialized                   *
 *              maxNElements    The maximum number of IDs in the list          *
 *                                                                             *
 * Outputs:     myIdList        The initialized object                         *
 *                                                                             *
 * Description: This function initializes the objIdList object                 *
 *                                                                             *
 ******************************************************************************/

void idListInit(struct objIdList* myIdList, unsigned int maxNElements)
{
    // Define the maximum number of elements
    myIdList->maxNElements = maxNElements;

    // Create a list
    myIdList->list = (ID_TYPE*) newTable1D(myIdList->maxNElements, sizeof(ID_TYPE));

    // Initialize the list
    idListReset(myIdList);

}

/*******************************************************************************
 * idListTerminate                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdList        The object to be terminated                    *
 *                                                                             *
 * Outputs:    (none)                                                          *
 *                                                                             *
 * Description: This function terminates the objIdList object                  *
 *                                                                             *
 ******************************************************************************/

void idListTerminate(struct objIdList* myIdList)
{

    // Delete the list
    deleteTable1D(myIdList->list);

}

/*******************************************************************************
 * idListReset                                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdList        The list to be reset                           *
 *                                                                             *
 * Outputs:     myIdList        The reset list                                 *
 *                                                                             *
 * Description: This function resets all the IDs in the list.                  *
 *                                                                             *
 ******************************************************************************/

void idListReset(struct objIdList* myIdList)
{

    unsigned int indexElement;

    // Set all IDs to null
    for (indexElement = 0; indexElement < myIdList->maxNElements; indexElement++)
    {
        myIdList->list[indexElement] = ID_NOSOURCE;
    }

    // Define the actual number of elements
    myIdList->nElements = 0;

}

/*******************************************************************************
 * idListAdd                                                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdList        The list to be used                            *
 *              ID              The ID to be added                             *
 *                                                                             *
 * Outputs:     myIdList        The updated list                               *
 *                                                                             *
 * Description: This function adds the ID in the list.                         *
 *                                                                             *
 ******************************************************************************/

signed int idListAdd(struct objIdList* myIdList, ID_TYPE ID)
{

    signed int rtnIndex;

    rtnIndex = -1;

    // Add a new ID

    if (myIdList->nElements < myIdList->maxNElements)
    {
        myIdList->list[myIdList->nElements] = ID;
        myIdList->nElements++;
        rtnIndex = myIdList->nElements - 1;
    }

    return rtnIndex;

}

/*******************************************************************************
 * idListDelete                                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdList        The list to be used                            *
 *              ID              The ID to be deleted                           *
 *                                                                             *
 * Outputs:     myIdList        The updated list                               *
 *                                                                             *
 * Description: This function removes the ID from the list.                    *
 *                                                                             *
 ******************************************************************************/

signed int idListDelete(struct objIdList* myIdList, ID_TYPE ID)
{

    unsigned int indexElement;
    signed int rtnIndex;

    // Delete this ID

    rtnIndex = -1;

    for (indexElement = 0; indexElement < myIdList->maxNElements; indexElement++)
    {
        if (myIdList->list[indexElement] == ID)
        {
            rtnIndex = (signed int) indexElement;
            myIdList->nElements--;
        }

        if (rtnIndex != -1)
        {
            if (indexElement == (myIdList->maxNElements - 1))
            {
                myIdList->list[indexElement] = ID_NOSOURCE;
            }
            else
            {
                myIdList->list[indexElement] = myIdList->list[indexElement + 1];
            }
        }
    }

    return rtnIndex;

}

/*******************************************************************************
 * idListGetIndex                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdList        The list to be used                            *
 *              ID              The ID to be found                             *
 *                                                                             *
 * Outputs:     (signed int)    The index of the ID                            *
 *                                                                             *
 * Description: This function returns the index position of the ID according   *
 *              to its position in the list. If the ID is not found, the       *
 *              constant ID_NOTFOUND is returned instead.                      *
 *                                                                             *
 ******************************************************************************/

signed int idListGetIndex(struct objIdList* myIdList, ID_TYPE ID)
{

    unsigned int indexElement;
    signed int rtnIndex;

    char found;

    found = 0;

    // Find this ID
    for (indexElement = 0; indexElement < myIdList->nElements; indexElement++)
    {
        if (myIdList->list[indexElement] == ID)
        {
            rtnIndex = (signed int) indexElement;
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        rtnIndex = ID_NOTFOUND;
    }

    return rtnIndex;

}

/*******************************************************************************
 * idListGetID                                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdList        The list to be used                            *
 *              index           The index of the ID to be found                *
 *                                                                             *
 * Outputs:     (ID_TYPE)       The ID                                         *
 *                                                                             *
 * Description: This function returns the ID value at the position specified   *
 *              by the index.                                                  *
 *                                                                             *
 ******************************************************************************/

ID_TYPE idListGetID(struct objIdList* myIdList, unsigned int index)
{
    return myIdList->list[index];
}

/*******************************************************************************
 * idListGetMaxNElements                                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdList        The list to be used                            *
 *                                                                             *
 * Outputs:     (unsigned int)  The maximum number of elements                 *
 *                                                                             *
 * Description: This function returns the maximum number of elements that can  *
 *              be stored in the list.                                         *
 *                                                                             *
 ******************************************************************************/

unsigned int idListGetMaxNElements(struct objIdList* myIdList)
{
    return myIdList->maxNElements;
}

/*******************************************************************************
 * idListGetNElements                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdList        The list to be used                            *
 *                                                                             *
 * Outputs:     (unsigned int)  The actual number of elements                  *
 *                                                                             *
 * Description: This function returns the actual number of elements that are   *
 *              stored in the list.                                            *
 *                                                                             *
 ******************************************************************************/

unsigned int idListGetNElements(struct objIdList* myIdList)
{
    return myIdList->nElements;
}

/*******************************************************************************
 * idListCompare                                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myIdListOld     The old list                                   *
 *              myIdListNew     The new list                                   *
 *                                                                             *
 * Outputs:     myIdListAdded   A list with the new IDs                        *
 *              myIdListDeleted A list with the deleted IDs                    *
 *                                                                             *
 * Description: This function returns a list that contains the new IDs added   *
 *              and the IDs deleted.                                           *
 *                                                                             *
 *              Ex: Suppose this is the initial list (old):                    *
 *                                                                             *
 *                    IDs                                                      *
 *                  +-----+                                                    *
 *                  |  3  |                                                    *
 *                  +-----+                                                    *
 *                  |  4  |                                                    *
 *                  +-----+                                                    *
 *                  |  7  |                                                    *
 *                  +-----+                                                    *
 *                  |  9  |                                                    *
 *                  +-----+                                                    *
 *                                                                             *
 *                  Then this list is modified as follows (new):               *
 *                                                                             *
 *                    IDs                                                      *
 *                  +-----+                                                    *
 *                  |  6  |                                                    *
 *                  +-----+                                                    *
 *                  |  4  |                                                    *
 *                  +-----+                                                    *
 *                  |  12 |                                                    *
 *                  +-----+                                                    *
 *                  |  9  |                                                    *
 *                  +-----+                                                    *
 *                                                                             *
 *                  The following lists are thus produced:                     *
 *                                                                             *
 *                  added:                  deleted:                           *
 *                                                                             *
 *                    IDs                     IDs                              *
 *                  +-----+                 +-----+                            *
 *                  |  6  |                 |  3  |                            *
 *                  +-----+                 +-----+                            *
 *                  |  12 |                 |  7  |                            *
 *                  +-----+                 +-----+                            *
 *                                                                             *
 ******************************************************************************/

void idListCompare(struct objIdList* myIdListOld, struct objIdList* myIdListNew, struct objIdList* myIdListAdded, struct objIdList* myIdListDeleted)
{

    unsigned int indexOld;
    unsigned int indexNew;

    ID_TYPE newID;
    ID_TYPE oldID;

    char found;

    // *************************************************************************
    // * STEP 1: Find new IDs                                                  *
    // *************************************************************************

    // Initialize the list of new IDs
    idListReset(myIdListAdded);

    // Find new IDs
    for (indexNew = 0; indexNew < myIdListNew->nElements; indexNew++)
    {

        newID = myIdListNew->list[indexNew];

        found = 0;

        for (indexOld = 0; indexOld < myIdListOld->nElements; indexOld++)
        {

            oldID = myIdListOld->list[indexOld];

            if (oldID == newID)
            {
                found = 1;
                break;
            }

        }

        if (found == 0)
        {
            idListAdd(myIdListAdded, newID);
        }

    }

    // *************************************************************************
    // * STEP 2: Find deleted IDs                                              *
    // *************************************************************************

    // Initialize the list of deleted IDs
    idListReset(myIdListDeleted);

    // Find new IDs
    for (indexOld = 0; indexOld < myIdListOld->nElements; indexOld++)
    {

        oldID = myIdListOld->list[indexOld];

        found = 0;

        for (indexNew = 0; indexNew < myIdListNew->nElements; indexNew++)
        {

            newID = myIdListNew->list[indexNew];

            if (oldID == newID)
            {
                found = 1;
                break;
            }

        }

        if (found == 0)
        {
            idListAdd(myIdListDeleted, oldID);
        }

    }

}
