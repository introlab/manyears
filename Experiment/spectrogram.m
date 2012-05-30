% 
% ************************************************************************* 
% * Authors: Francois Grondin                                             * 
% ************************************************************************* 
% * Inputs:  x               Vector                                       * 
% *          hop             Number of samples between adjacent windows   * 
% *          windowSize      Size of each window                          * 
% *          Fs              Sampling rate                                *
% *          minValue        Minimum dB value                             *
% *          maxValue        Maximum dB value                             *
% * Outputs: imageMatrix     Resulting matrix of the spectrogram          *
% *          numberSlices    Number of frames in the matrix               * 
% ************************************************************************* 
% * Description:                                                          * 
% *                                                                       * 
% * This function creates a 2D matrix with the spectorgram                *
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
function [imageMatrix,numberSlices] = spectrogram(x,hop,windowSize,Fs,minValue,maxValue)

    hanningWindow = hannWin(windowSize);

    [vectorFrames,numberSlices] = createFrames(x,hop,windowSize);
    
    imageMatrix = zeros(size(vectorFrames));
    
    for iFrame = 1:1:numberSlices
       
        thisFrameTime = vectorFrames(iFrame,:);
        thisFrameWin = thisFrameTime .* hanningWindow;
        thisFrameFFT = fft(thisFrameWin);
        thisFramePower = abs(thisFrameFFT).^2;
        thisFrameLog = 10 * log10(thisFramePower);
        
        thisFrameLogMinIndex = (1:1:windowSize) .* (thisFrameLog < minValue);
        thisFrameLogMinIndexSort = sort(thisFrameLogMinIndex);
        thisFrameLogMinIndexCrop = thisFrameLogMinIndexSort((sum(thisFrameLogMinIndexSort == 0)+1):1:windowSize);
        thisFrameLog(thisFrameLogMinIndexCrop) = minValue * ones(size(thisFrameLogMinIndexCrop));

        thisFrameLogMaxIndex = (1:1:windowSize) .* (thisFrameLog > maxValue);
        thisFrameLogMaxIndexSort = sort(thisFrameLogMaxIndex);
        thisFrameLogMaxIndexCrop = thisFrameLogMaxIndexSort((sum(thisFrameLogMaxIndexSort == 0)+1):1:windowSize);
        thisFrameLog(thisFrameLogMaxIndexCrop) = maxValue * ones(size(thisFrameLogMaxIndexCrop));
        
        imageMatrix(iFrame,:) = thisFrameLog;
        
    end

return