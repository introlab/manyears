#ifndef INPUTOUTPUTMANAGER_H
#define INPUTOUTPUTMANAGER_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QSemaphore>
#include <QString>
#include <QTimer>
#include <QFile>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include <QListWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QCoreApplication>

#include <QDebug>

#include "RTAudio/RtAudio.h"
#include "stdio.h"

// Define the maximum number of buffers that are reserved
// in memory
#define BUFFERS_RESERVE     100

// Define the dimensions of the input data
#define INPUT_HOPSIZE           512
#define INPUT_FRAMESIZE         1024
#define INPUT_SAMPLERATE        48000
#define INPUT_NUMBERCHANNELS    8

class InputOutputManager: public QObject
{

    Q_OBJECT

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    InputOutputManager();

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~InputOutputManager();

    /***********************************************************
    * Thread control                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Start                                                 |
    // +-------------------------------------------------------+

    void startThread();

    // +-------------------------------------------------------+
    // | Stop                                                  |
    // +-------------------------------------------------------+

    void stopThread();

    /***********************************************************
    * Stream callbacks                                         *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Sound card / File                                     |
    // +-------------------------------------------------------+

    void streamCallBack(signed short* inputBuffer);

    /***********************************************************
    * External buffers management                              *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Wait for a buffer to be ready to processed            |
    // +-------------------------------------------------------+

    float** waitForBuffer();

    // +-------------------------------------------------------+
    // | Return a buffer that is no longer used                |
    // +-------------------------------------------------------+

    void giveBackBuffer(float** _buffer);

    // +-------------------------------------------------------+
    // | Clean the stream                                      |
    // +-------------------------------------------------------+

    void cleanStream();

    /***********************************************************
    * User interface                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Ask the user to select input/output                   |
    // +-------------------------------------------------------+

    void askUser(bool _validSelectionRequired);

    /***********************************************************
    * Event receivers                                          *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Add an event receiver                                 |
    // +-------------------------------------------------------+

    void pushEventReceiver(QObject* _receiver);

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | General information regarding data format             |
    // +-------------------------------------------------------+

    const unsigned int getHopSize();
    const unsigned int getFrameSize();
    const unsigned int getSampleRate();
    const unsigned int getNumberChannels();

    RtAudio* getRtAudio() {return rtaudio;}

private:

    /***********************************************************
    * Internal buffers management                              *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Create a new buffer                                   |
    // +-------------------------------------------------------+

    float** createBuffer();

    // +-------------------------------------------------------+
    // | Delete an existing buffer                             |
    // +-------------------------------------------------------+

    void deleteBuffer(float** _buffer);

    // +-------------------------------------------------------+
    // | Initialize the memory for the buffers                 |
    // +-------------------------------------------------------+

    void initBuffers();

    // +-------------------------------------------------------+
    // | Clear the memory for the buffers                      |
    // +-------------------------------------------------------+

    void deleteBuffers();

    // +-------------------------------------------------------+
    // | Get an available buffer to fill                       |
    // +-------------------------------------------------------+

    float** getUnusedBuffer();

    // +-------------------------------------------------------+
    // | Push an unused buffer in the stack                    |
    // +-------------------------------------------------------+

    void pushUnusedBuffer(float** _unusedBuffer);

    // +-------------------------------------------------------+
    // | Get an available buffer to process                    |
    // +-------------------------------------------------------+

    float** getFilledBuffer();

    // +-------------------------------------------------------+
    // | Push a filled buffer in the stack                     |
    // +-------------------------------------------------------+

    void pushFilledBuffer(float** _filledBuffer);

    // +-------------------------------------------------------+
    // | Move all filled buffers to unused buffers             |
    // +-------------------------------------------------------+

    void moveAllFilledToUnused();

    // +-------------------------------------------------------+
    // | Flag end of acquisition                               |
    // +-------------------------------------------------------+

    void flagEndAcquisition();

    /***********************************************************
    * Sound card                                               *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Get the list of the sound cards                       |
    // +-------------------------------------------------------+

    QVector<QString> getListSoundcards();

    /***********************************************************
    * Stream                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Open the stream                                       |
    // +-------------------------------------------------------+

    void openStream();

    // +-------------------------------------------------------+
    // | Open the stream if closed                             |
    // +-------------------------------------------------------+

    void openStreamIfClosed();

    // +-------------------------------------------------------+
    // | Close the stream if already opened                    |
    // +-------------------------------------------------------+

    void closeStreamIfOpened();

    // +-------------------------------------------------------+
    // | Start the stream                                      |
    // +-------------------------------------------------------+

    void startStream();

    // +-------------------------------------------------------+
    // | Stop the stream                                       |
    // +-------------------------------------------------------+

    void stopStream();

    // +-------------------------------------------------------+
    // | Handle the end of the stream                          |
    // +-------------------------------------------------------+

    void endOfStream();

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | General information regarding data format             |
    // +-------------------------------------------------------+

    unsigned int hopSize;
    unsigned int frameSize;
    unsigned int sampleRate;
    unsigned int numberChannels;
    QMutex mutexHopSize;
    QMutex mutexFrameSize;
    QMutex mutexSampleRate;
    QMutex mutexNumberChannels;

    // +-------------------------------------------------------+
    // | Stacks to store audio data                            |
    // +-------------------------------------------------------+

    QVector<float**> unusedBuffersStack;
    QVector<float**> filledBuffersStack;
    QMutex unusedBuffersStackIsUsed;
    QMutex filledBuffersStackIsUsed;
    QSemaphore waitingFilledBuffers;

    // +-------------------------------------------------------+
    // | Stream status                                         |
    // +-------------------------------------------------------+

    bool streamIsRunning;

    // +-------------------------------------------------------+
    // | Stream event receiver                                 |
    // +-------------------------------------------------------+

    QVector<QObject*> streamEventReceiver;

    // +-------------------------------------------------------+
    // | Stream objects                                        |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Data acquisition                                  |
        // +---------------------------------------------------+

            // +-----------------------------------------------+
            // | Sound card                                    |
            // +-----------------------------------------------+

            RtAudio* rtaudio;

            // +-----------------------------------------------+
            // | File                                          |
            // +-----------------------------------------------+

            QFile inputFile;
            bool inputFileOpened;
            QTimer inputFileTimer;
            signed short* inputFileBuffer;

        // +---------------------------------------------------+
        // | Data output                                       |
        // +---------------------------------------------------+

        FILE* outputFile;
        bool outputFileOpened;

    // +-------------------------------------------------------+
    // | Stream parameters (actual)                            |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Data acquisition                                  |
        // +---------------------------------------------------+

            // +-----------------------------------------------+
            // | Sound card                                    |
            // +-----------------------------------------------+

            bool fromSoundCard;
            int soundcardIndex;

            // +-----------------------------------------------+
            // | File                                          |
            // +-----------------------------------------------+

            bool fromFile;
            QString inputFileName;

        // +---------------------------------------------------+
        // | Data output                                       |
        // +---------------------------------------------------+

        bool toFile;
        QString outputFileName;

    // +-------------------------------------------------------+
    // | Stream parameters (projected)                         |
    // +-------------------------------------------------------+

        bool projectionIsValid;

        // +---------------------------------------------------+
        // | Data acquisition                                  |
        // +---------------------------------------------------+

            // +-----------------------------------------------+
            // | Sound card                                    |
            // +-----------------------------------------------+

            bool projFromSoundCard;
            int projSoundcardIndex;

            // +-----------------------------------------------+
            // | File                                          |
            // +-----------------------------------------------+

            bool projFromFile;
            QString projInputFileName;

        // +---------------------------------------------------+
        // | Data output                                       |
        // +---------------------------------------------------+

        bool projToFile;
        QString projOutputFileName;

    // +-------------------------------------------------------+
    // | User interface                                        |
    // +-------------------------------------------------------+

        // +---------------------------------------------------+
        // | Create objects                                    |
        // +---------------------------------------------------+

            // +-----------------------------------------------+
            // | Dialog                                        |
            // +-----------------------------------------------+

            QDialog* dialog;

            // +-----------------------------------------------+
            // | Group Boxes                                   |
            // +-----------------------------------------------+

            QGroupBox* groupBoxParameters;
            QGroupBox* groupBoxInput;
            QGroupBox* groupBoxOutput;

            // +-----------------------------------------------+
            // | Objects                                       |
            // +-----------------------------------------------+

                // +-------------------------------------------+
                // | Labels                                    |
                // +-------------------------------------------+

                QLabel* labelSampleRateTitle;
                QLabel* labelSampleRateValue;
                QLabel* labelFrameSizeTitle;
                QLabel* labelFrameSizeValue;
                QLabel* labelHopSizeTitle;
                QLabel* labelHopSizeValue;
                QLabel* labelNumberChannelsTitle;
                QLabel* labelNumberChannelsValue;

                // +-------------------------------------------+
                // | Radio Buttons                             |
                // +-------------------------------------------+

                QRadioButton* radioButtonSoundCard;
                QRadioButton* radioButtonInputFile;

                // +-------------------------------------------+
                // | List                                      |
                // +-------------------------------------------+

                QListWidget* listSoundCard;

                // +-------------------------------------------+
                // | CheckBox                                  |
                // +-------------------------------------------+

                QCheckBox* checkBox;

                // +-------------------------------------------+
                // | Push Buttons                              |
                // +-------------------------------------------+

                QPushButton* pushButtonOk;
                QPushButton* pushButtonCancel;

public slots:

    /***********************************************************
    * User interface                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Ok push button is clicked                             |
    // +-------------------------------------------------------+

    void okPushButtonClicked();

    // +-------------------------------------------------------+
    // | Cancel push button is clicked                         |
    // +-------------------------------------------------------+

    void cancelPushButtonClicked();

    // +-------------------------------------------------------+
    // | Sound card radio button is clicked                    |
    // +-------------------------------------------------------+

    void soundCardRadioButtonClicked();

    // +-------------------------------------------------------+
    // | File input radio button is clicked                    |
    // +-------------------------------------------------------+

    void fileInputRadioButtonClicked();

    // +-------------------------------------------------------+
    // | Item selection changed in the list                    |
    // +-------------------------------------------------------+

    void listItemSelectionChanged();

    // +-------------------------------------------------------+
    // | List is double-clicked                                |
    // +-------------------------------------------------------+

    void listDoubleClicked(QListWidgetItem* _item);

    /***********************************************************
    * File callback function                                   *
    ***********************************************************/

    void fileCallBack();

};

/***********************************************************
* RTAudio callback function                                *
***********************************************************/

int rtAudioCallBack(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void* inputOutputManager);

class EndOfStreamEvent: public QEvent
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    EndOfStreamEvent();

    EndOfStreamEvent(const EndOfStreamEvent &e);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~EndOfStreamEvent();

    /***********************************************************
    * Operators                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    EndOfStreamEvent& operator=(const EndOfStreamEvent& _endOfStreamEvent);

};

#endif
