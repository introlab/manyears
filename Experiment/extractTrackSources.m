% 
% ************************************************************************* 
% * Authors: Francois Grondin                                             * 
% ************************************************************************* 
% * Inputs:  trackFilePath   Path of the track file                       * 
% * Outputs: idMatrix        Matrix of IDs                                *
% *          xMatrix         Matrix of x-coordinates                      *
% *          yMatrix         Matrix of y-coordinates                      *
% *          zMatrix         Matrix of z-coordinates                      *
% ************************************************************************* 
% * Description:                                                          * 
% *                                                                       * 
% * This function extracts the track information                          *
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
function [idMatrix, xMatrix, yMatrix, zMatrix] = extractTrackSources(trackFilePath)

    % Get number of sources

    nSources = -1;
    
    fid = fopen(trackFilePath);

    thisLine = 'empty';
    
    while (max(size(thisLine)) ~= 1)
    
        thisLine = fgets(fid);
        nSources = nSources + 1;
        
    end   
    
    fclose(fid);

    % Get number of frames
    
    nFrames = 0;
    
    fid = fopen(trackFilePath);
    
    while(~feof(fid))
    
        for iSource = 1:1:nSources

            thisLine = fgets(fid);

        end

        thisLine = fgets(fid);
        nFrames = nFrames + 1;
        
    end
    
    fclose(fid);
    
    % Define matrices
    
    idMatrix = zeros(nFrames,nSources);
    xMatrix = zeros(nFrames,nSources);
    yMatrix = zeros(nFrames,nSources);
    zMatrix = zeros(nFrames,nSources);
    
    fid = fopen(trackFilePath);
    
    for iFrame = 1:1:nFrames
        
        for iSource = 1:1:nSources
       
            thisLine = fgets(fid);
            
            spaces = strfind(thisLine, ' ');
            
            idStr = thisLine(1:(spaces(1)-1));
            xStr = thisLine(spaces(1):(spaces(2)-1));
            yStr = thisLine(spaces(2):(spaces(3)-1));
            zStr = thisLine(spaces(3):length(thisLine));
            
            id = str2num(idStr);
            x = str2num(xStr);
            y = str2num(yStr);
            z = str2num(zStr);
            
            idMatrix(iFrame,iSource) = id;
            xMatrix(iFrame,iSource) = x;
            yMatrix(iFrame,iSource) = y;
            zMatrix(iFrame,iSource) = z;
            
        end
        
        thisLine = fgets(fid);
        
    end
    
    fclose(fid);
    
return