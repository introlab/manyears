% 
% ************************************************************************* 
% * Authors: Francois Grondin                                             * 
% ************************************************************************* 
% * Inputs:  nSamples        Size of the window                           * 
% * Outputs: hannWindow      Resulting window                             *
% ************************************************************************* 
% * Description:                                                          * 
% *                                                                       * 
% * This function creates a hann window                                   *
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

function [hannWindow] = hannWin(nSamples)

    n = 0:1:(nSamples-1);
    
    hannWindow = 0.5 * (1 - cos( 2 * pi * n / (nSamples - 1) ) );

return