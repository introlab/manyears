% 
% ************************************************************************* 
% * Authors: Laurier Demers, Francois Grondin, Arash Vakili               * 
% ************************************************************************* 
% * Inputs:  x               Vector                                       * 
% *          hop             Number of samples between adjacent windows   * 
% *          windowSize      Size of each window                          * 
% * Outputs: vectorFrames    Resulting matrix made of all the frames      * 
% *          numberSlices    Number of frames in the matrix               * 
% ************************************************************************* 
% * Description:                                                          * 
% *                                                                       * 
% * This function splits a vector in overlapping frames and stores these  * 
% * frames into a matrix:                                                 * 
% *                                                                       * 
% * |------------------Input vector---------------------|                 * 
% *                                                                       * 
% * |------1------|                                                       * 
% *     |------2------|                                                   * 
% *         |------3------|                                               * 
% *             |------4------|                                           * 
% *                      ...                                              * 
% *                                                                       * 
% * Index            Frame                                                * 
% *   1         |------1------|                                           * 
% *   2         |------2------|                                           * 
% *   3         |------3------|                                           * 
% *   4         |------4------|                                           * 
% *  ...              ...                                                 * 
% *                                                                       * 
% ************************************************************************* 
% * DISCLAIMER:                                                           * 
% *                                                                       * 
% * Copyright and other intellectual property laws protect these          * 
% * materials. Reproduction or retransmission of the materials, in whole  * 
% * or in part, in any manner, without the prior consent of the copyright * 
% * holders, is a violation of copyright law.                             * 
% *                                                                       * 
% * The authors are not responsible for any damages whatsoever, including * 
% * any type of loss of information, interruption of business, personal   * 
% * injury and/or any damage or consequential damage without any          * 
% * limitation incurred before, during or after the use of this code.     * 
% ************************************************************************* 
% 
function [vectorFrames,numberSlices] = createFrames(x,hop,windowSize) 

% Find the max number of slices that can be obtained 
numberSlices = floor((length(x)-windowSize)/hop); 

% Truncate if needed to get only a integer number of hop 
x = x(1:(numberSlices*hop+windowSize)); 

% Create a matrix with time slices 
vectorFrames = zeros(floor(length(x)/hop),windowSize); 

% Fill the matrix 
for index = 1:numberSlices 

    indexTimeStart = (index-1)*hop + 1; 
    indexTimeEnd = (index-1)*hop + windowSize; 

    vectorFrames(index,:) = x(indexTimeStart: indexTimeEnd); 

end 

return