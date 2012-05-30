% 
% ************************************************************************* 
% * Authors: Laurier Demers, Francois Grondin, Arash Vakili               * 
% ************************************************************************* 
% * Inputs:  framesMatrix    Matrix made of all the frames                * 
% *          hop             Number of samples between adjacent windows   * 
% * Outputs: vectorTime      Resulting vector from overlapp add           * 
% ************************************************************************* 
% * Description:                                                          * 
% *                                                                       * 
% * This function overlap adds the frames from the input matrix           * 
% *                                                                       * 
% * Index            Frame                                                * 
% *   1         |------1------|                                           * 
% *   2         |------2------|                                           * 
% *   3         |------3------|                                           * 
% *   4         |------4------|                                           * 
% *  ...              ...                                                 * 
% *                                                                       * 
% * |------1------|                                                       * 
% *   + |------2------|                                                   * 
% *       + |------3------|                                               * 
% *           + |------4------|                                           * 
% *               +      ...                                              * 
% *                                                                       * 
% * |------------------Input vector---------------------|                 * 
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
function vectorTime = fusionFrames(framesMatrix, hop) 

sizeMatrix = size(framesMatrix); 

% Get the number of frames 
numberFrames = sizeMatrix(1); 

% Get the size of each frame 
sizeFrames = sizeMatrix(2); 

% Define an empty vector to receive result 
vectorTime = zeros(numberFrames*hop-hop+sizeFrames,1); 

timeIndex = 1; 

% Loop for each frame and overlap-add 
for index=1:numberFrames 

    vectorTime(timeIndex:timeIndex+sizeFrames-1) = vectorTime(timeIndex:timeIndex+sizeFrames-1) + framesMatrix(index,:)'; 

    timeIndex = timeIndex + hop; 

end 

return