fid = fopen('/media/Data/Maîtrise/Expériences/Experience3/Test/TwoSpeakers/Recorded/2011-05-22_13-09-12.raw','r');

nChannels = 8;
nSamplesNoise = 400000;
nSamplesSpeech = 768000;

vectorNoise = fread(fid, nChannels * nSamplesNoise, 'int16');
vectorSpeech = fread(fid, nChannels * nSamplesSpeech, 'int16');

fclose(fid);

oneChannelNoise = vectorNoise(1:8:(nChannels*nSamplesNoise))/32768;
oneChannelSpeech = vectorSpeech(1:8:(nChannels*nSamplesSpeech))/32768;

fid = fopen('/media/Data/Maîtrise/Expériences/Experience3/Test/TwoSpeakers/Recorded/01-02.raw','w');
fwrite(fid, vectorSpeech, 'int16');
fclose(fid);

figure(1);
plot(oneChannelNoise);
figure(2);
plot(oneChannelSpeech);