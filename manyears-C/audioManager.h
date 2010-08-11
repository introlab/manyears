#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMutex>
#include <QTimer>

#include <QDebug>

#include "RTAudio/RtAudio.h"

typedef void (*callBackFcn)( void *objPtr );

class AudioManager : public QObject
{

    Q_OBJECT;

public:

    static const char TYPE_RECORD = 1;
    static const char TYPE_FILE = 2;

    AudioManager(int _numberChannels, int _frameSize, int _numberHopPerFrame, int _samplingRate, callBackFcn _callback, void *_parentObj, char _inputType);
    AudioManager();

    virtual ~AudioManager();

    unsigned int getAudioDevicesCount();

    QString getAudioDeviceName(unsigned int _deviceIndex);

    int getAudioDeviceIndex(QString _deviceName);

    void openStream(int _deviceIndex);
    void openStream(QString _filePath);

    void startRecording();

    void stopRecording();

    void closeStream();

    int getNumberChannels();

    float *getBuffers();

    float **newBuffers();

    int getIndexHop();

    int getHopSize();

    int getNumberHopPerFrame();

    void updateIndexHop();

    void gotoCallBack();

    void postEmptyFrame();

    int getInputType() const;

    QString getFileName() const;

private slots:

    void processTimer();

private:

    RtAudio *audioDriver;

    FILE *fileDriver;

    char inputType;

    QString filePath;

    QTimer* fileTimer;

    int numberChannels;

    unsigned int hopSize;

    int frameSize;

    int numberHopPerFrame;

    int indexHop;

    int samplingRate;

    float *buffers;

    callBackFcn callback;

    void *parentObj;

    QVector<float**> newBuffersStack;

    QMutex stackIsUsed;

    signed short *recordedStream;

    int sampleIndex;

    int fileSize;

};

int fillBuffer(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *manager);

#endif // AUDIOMANAGER_H
