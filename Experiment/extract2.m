fid = fopen('/media/Data/Doctorat/Articles/AR2012ManyEars/Experiment/essai3_2sources_180_croise.raw','r');

nChannels = 8;
nSamplesSpeech = 1152000;

vectorSpeech = fread(fid, nChannels * nSamplesSpeech, 'int16');

fclose(fid);

oneChannelSpeech = vectorSpeech(1:8:(nChannels*nSamplesSpeech))/32768;

fid = fopen('/media/Data/Doctorat/Articles/AR2012ManyEars/Experiment/essai3_2sources_180_croise_cropped.raw','w');
fwrite(fid, vectorSpeech, 'int16');
fclose(fid);

figure(1);
plot(oneChannelSpeech);