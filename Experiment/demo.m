% Select experiment
experiment = 1;   % 1 = Static
                  % 2 = Moving, non-crossing
                  % 3 = Moving, crossing

% Set exe path                  
exePath = '../manyears-C/bin/manyears_demo';              

switch experiment

    case 1
    
        mode = '-c';
        cleanFile = 'static_clean.wav';
        rawFile = 'static_recorded.raw';

        src1LongTextX = 2.5;
        src1LongTextY = 125;
        src2LongTextX = 2.5;
        src2LongTextY = -75;        

        src1LatTextX = 2.5;
        src1LatTextY = 40;
        src2LatTextX = 2.5;
        src2LatTextY = -15;        
                
    case 2
        
        mode = '-r';
        cleanFile = '';
        rawFile = 'moving_noncrossing.raw';

        src1LongTextX = 2.5;
        src1LongTextY = 150;
        src2LongTextX = 2.5;
        src2LongTextY = -75;        

        src1LatTextX = 2.5;
        src1LatTextY = 55;
        src2LatTextX = 2.5;
        src2LatTextY = 0;           
        
    case 3
        
        mode = '-r';
        cleanFile = '';
        rawFile = 'moving_crossing.raw';

        src1LongTextX = 2.5;
        src1LongTextY = 150;
        src2LongTextX = 2.5;
        src2LongTextY = -120;        

        src1LatTextX = 2.5;
        src1LatTextY = 55;
        src2LatTextX = 2.5;
        src2LatTextY = 0;                   
        
end

% Clean cropping
wave1Offset = 1.3;
wave1Duration = 16;
wave2Offset = 1.3;
wave2Duration = 16;

%% Execute script

commandLine = [exePath,' ',mode,' ',rawFile];
system(commandLine);

%% Parameters

frameSize = 1024;                       % Size of a frame in samples
hopSize = 512;                          % Size of the hop in samples
Fs = 48000;                             % Sampling rate (samples/sec)
minDB = -40;                            % Minimum value of power in dB
maxDB = 20;                             % Maximum value of power in dB

close all

%% Extract theoretical and experimental data

if strcmp(cleanFile,'') == 0

    cleanWave = wavread(cleanFile);

    cleanWave1 = cleanWave(:,1);
    cleanWave2 = cleanWave(:,2);

    cleanWave1 = [zeros(round(Fs*wave1Offset),1) ; cleanWave1];
    cleanWave1 = cleanWave1(1:1:(round(Fs*wave1Duration)));

    cleanWave2 = [zeros(round(Fs*wave2Offset),1) ; cleanWave2];
    cleanWave2 = cleanWave2(1:1:(round(Fs*wave2Duration)));

    spectrum1 = spectrogram(cleanWave1,hopSize,frameSize,Fs,minDB,maxDB);
    spectrum2 = spectrogram(cleanWave2,hopSize,frameSize,Fs,minDB,maxDB);

end

[idMatrix, xMatrix, yMatrix, zMatrix] = extractTrackSources(strcat(rawFile,'-tracked.dat'));

%% Find ids

nIds = 2;                               % Number of ids in total
maxId = 1000;                           % Maximum value for any id
ids = zeros(nIds,1);                    % Initialize the list of ids

iId = 0;

% Loop for each potential value of id
for idValue = 0:1:maxId

    idMatrixTmp = idMatrix;

    % If there is a match
    if sum(sum(idMatrixTmp == idValue)) ~= 0

        % Save this id value
        id = idValue;
        
        % Remove id from list
        idMatrixTmp = (idMatrixTmp ~= idValue) .* idMatrixTmp + (idMatrixTmp == idValue) * -1;

        % Save this id in the vector
        iId = iId + 1;
        ids(iId) = id;

    end

    % If the total number of id is reached, stop looping
    if (iId == nIds)
       break 
    end
    
end

%% Load vectors and matrices for each id

nFrames = size(idMatrix,1);             % Get the number of frames


% List of colors for source tracking
colorsIndexes = [ 1 0 0 ; 0 1 0 ];

idVectors = cell(nIds,1);               % List of all ids vectors
posVectors = cell(nIds,1);              % List of all positions vectors
colorVectors = cell(nIds,1);            % List of all colors vectors
separatedMatrices = cell(nIds,1);       % List of all separated spectra
postfilteredMatrices = cell(nIds,1);    % List of all postfiltered spectra

% Loop for each id
for iId = 1:1:nIds
   
    % Get the id
    id = ids(iId);
    
    % Initialize all vectors
    idVector = zeros(nFrames,1);
    posVector = zeros(nFrames,3);
    colorVector = zeros(nFrames,3);
    separatedMatrix = zeros(nFrames,frameSize);
    postfilteredMatrix = zeros(nFrames,frameSize);
    
    % Pad the id name with zeros
    idStr = '00000';
    idStr(max(size(idStr))-(floor(log10(id))):1:max(size(idStr))) = num2str(id);
    
    % Get the separated and postfiltered wav vectors
    separatedWave = wavread(strcat(rawFile,'-separated',idStr,'.wav'));
    postfilteredWave = wavread(strcat(rawFile,'-postfiltered',idStr,'.wav'));
    
    separatedGain = sqrt(mean(separatedWave.^2));
    postfilteredGain = sqrt(mean(postfilteredWave.^2));
    
    postfilteredWave = postfilteredWave * (separatedGain/postfilteredGain);
        
    % Generate spectrogram for separated and postfiltered vectors
    separatedFrames = spectrogram(separatedWave,hopSize,frameSize,Fs,minDB,maxDB);
    postfilteredFrames = spectrogram(postfilteredWave,hopSize,frameSize,Fs,minDB,maxDB);
    
    iSpectrogramFrame = 1;
    
    % Loop for each frame    
    for iFrame = 1:1:nFrames
       
        % If the current id is found in the current frame
        [maxValue,maxIndex] = max(idMatrix(iFrame,:) == id);
        
        % If the id is found
        if (maxValue ~= 0)
            
            % Load id, position, corresponding colors, separated and
            % postfiltered spectra
            idVector(iFrame) = id;
            posVector(iFrame,:) = [xMatrix(iFrame,maxIndex) yMatrix(iFrame,maxIndex) zMatrix(iFrame,maxIndex)];
            colorVector(iFrame,:) = colorsIndexes(iId,:);
            separatedMatrix(iFrame,:) = separatedFrames(iSpectrogramFrame,:);
            postfilteredMatrix(iFrame,:) = postfilteredFrames(iSpectrogramFrame,:);
            
            iSpectrogramFrame = iSpectrogramFrame + 1;
            
        else
            
            % Set null values
            idVector(iFrame) = -1;
            posVector(iFrame,:) = [ 0 0 0 ];
            colorVector(iFrame,:) = colorsIndexes(iId,:);
            separatedMatrix(iFrame,:) = minDB * ones(1,frameSize);
            postfilteredMatrix(iFrame,:) = minDB * ones(1,frameSize);
            
        end
        
    end
    
    % Save vectors in the list
    idVectors{iId} = idVector;
    posVectors{iId} = posVector;
    colorVectors{iId} = colorVector;
    separatedMatrices{iId} = separatedMatrix;
    postfilteredMatrices{iId} = postfilteredMatrix;
    
end

%% Display results

time = transpose((1:1:nFrames) * (hopSize/Fs));

timeInterval = Fs / hopSize;
freqMax = 100;
freqInterval = 10;

figureSize = [ 0 0 500 150 ];

iFigure = 1;

cmap = colormap(gray);

if strcmp(cleanFile,'') == 0

    spectrum1Matrix = size(cmap,1) * (1 - (spectrum1 - minDB) / (maxDB - minDB));
    spectrum2Matrix = size(cmap,1) * (1 - (spectrum2 - minDB) / (maxDB - minDB));

    figure(iFigure);
    set(iFigure, 'Position', figureSize);
    image(transpose(fliplr(spectrum1Matrix(:,1:1:freqMax-1))));
    set(gca, 'XTick', 1:timeInterval:nFrames);
    set(gca, 'XTickLabel', 0:1:(floor(nFrames/timeInterval)));
    set(gca, 'YTick', 0:freqInterval:freqMax);
    set(gca, 'YTickLabel', round((((freqMax/frameSize)*Fs):((-1*freqInterval/frameSize)*Fs):0)/10)/100);
    colormap(gray);
    xlabel('Time (sec)');
    ylabel('Frequency (Hz)');
    iFigure = iFigure + 1;

    figure(iFigure);
    set(iFigure, 'Position', figureSize);
    image(transpose(fliplr(spectrum2Matrix(:,1:1:freqMax-1))));
    set(gca, 'XTick', 1:timeInterval:nFrames);
    set(gca, 'XTickLabel', 0:1:(floor(nFrames/timeInterval)));
    set(gca, 'YTick', 0:freqInterval:freqMax);
    set(gca, 'YTickLabel', round((((freqMax/frameSize)*Fs):((-1*freqInterval/frameSize)*Fs):0)/10)/100);
    colormap(gray);
    xlabel('Time (sec)');
    ylabel('Frequency (Hz)');
    iFigure = iFigure + 1;

end

totalPosVector = [];
totalColorVector = [];
totalTimeVector = [];

for iId = 1:1:nIds
   
    idVector = idVectors{iId};
    posVector = posVectors{iId};
    colorVector = colorVectors{iId};
    separatedMatrix = size(cmap,1) * (1 - (separatedMatrices{iId} - minDB) / (maxDB - minDB));
    postfilteredMatrix = size(cmap,1) * (1 - (postfilteredMatrices{iId} - minDB) / (maxDB - minDB));
    
    [idVectorSorted,sortedIndexes] = sort(idVector);
    firstIndex = sum(idVectorSorted == -1) + 1;
    lastIndex = length(idVectorSorted);
    croppedIndexes = sortedIndexes(firstIndex:1:lastIndex);
    
    figure(iFigure);
    set(iFigure, 'Position', figureSize);
    image(transpose(fliplr(separatedMatrix(:,1:1:freqMax-1))));
    set(gca, 'XTick', 1:timeInterval:nFrames);
    set(gca, 'XTickLabel', 0:1:(floor(nFrames/timeInterval)));
    set(gca, 'YTick', 0:freqInterval:freqMax);
    set(gca, 'YTickLabel', round((((freqMax/frameSize)*Fs):((-1*freqInterval/frameSize)*Fs):0)/10)/100);
    colormap(gray);
    xlabel('Time (sec)');
    ylabel('Frequency (Hz)');
    iFigure = iFigure + 1;

    figure(iFigure);
    set(iFigure, 'Position', figureSize);
    image(transpose(fliplr(postfilteredMatrix(:,1:1:freqMax-1))));
    set(gca, 'XTick', 1:timeInterval:nFrames);
    set(gca, 'XTickLabel', 0:1:(floor(nFrames/timeInterval)));
    set(gca, 'YTick', 0:freqInterval:freqMax);
    set(gca, 'YTickLabel', round((((freqMax/frameSize)*Fs):((-1*freqInterval/frameSize)*Fs):0)/10)/100);
    colormap(gray);
    xlabel('Time (sec)');
    ylabel('Frequency (Hz)');
    iFigure = iFigure + 1;
        
    posVectorCropped = posVector(croppedIndexes,:);
    colorVectorCropped = colorVector(croppedIndexes,:);
    timeVectorCropped = time(croppedIndexes,:);
    
    totalPosVector = [totalPosVector ; posVectorCropped];
    totalColorVector = [totalColorVector ; colorVectorCropped];
    totalTimeVector = [totalTimeVector ; timeVectorCropped];
    
end

x = totalPosVector(:,1);
y = totalPosVector(:,2);
z = totalPosVector(:,3);

theta = 360 * atan2(y,x) / (2 * pi);
phi = 180 * asin(z) / (pi);

figure(iFigure);
set(iFigure, 'Position', figureSize);
scatter(totalTimeVector,theta,3,totalColorVector);
set(gca, 'XTick', 1:1:floor((nFrames-1)/timeInterval));
set(gca, 'XTickLabel', 1:1:floor((nFrames-1)/timeInterval));
ylim([-180 +180]);
xlim([0 nFrames / timeInterval]);
xlabel('Time (sec)');
ylabel('Longitude (degrees)');
text(src1LongTextX,src1LongTextY,'Source1');
text(src2LongTextX,src2LongTextY,'Source2');
iFigure = iFigure + 1;

figure(iFigure);
set(iFigure, 'Position', figureSize);
scatter(totalTimeVector,phi,3,totalColorVector);
set(gca, 'XTick', 1:1:floor((nFrames-1)/timeInterval));
set(gca, 'XTickLabel', 1:1:floor((nFrames-1)/timeInterval));
ylim([-90 +90]);
xlim([0 nFrames / timeInterval]);
xlabel('Time (sec)');
ylabel('Latitude (degrees)');
text(src1LatTextX,src1LatTextY,'Source1');
text(src2LatTextX,src2LatTextY,'Source2');
iFigure = iFigure + 1;
