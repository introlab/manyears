#CREATE Log Directory
LOG_DIR=`date +samples/%F_%Hh%Mm%S`
mkdir -p -v $LOG_DIR
ln -s -f -T $LOG_DIR log

killall audio_server2
killall gflow
killall audioview

#START AUDIO SERVER
src/audio_server2 plughw:2 &

sleep 5

#START AUDIO VIEWER
GUI/audioviewer &

sleep 5

#START LOCALIZATION & SEPARATION
gflow n-files/LocalizeSeparAndSaveWav.n &







