#include "inputOutputManager.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

InputOutputManager::InputOutputManager()
{

    // +-------------------------------------------------------+
    // | General information regarding data format             |
    // +-------------------------------------------------------+

    this->hopSize = INPUT_HOPSIZE;
    this->frameSize  = INPUT_FRAMESIZE;
    this->sampleRate = INPUT_SAMPLERATE;
    this->numberChannels = INPUT_NUMBERCHANNELS;

    // +-------------------------------------------------------+
    // | Create the RTAudio object                             |
    // +-------------------------------------------------------+

    this->rtaudio = new RtAudio();

    // +-------------------------------------------------------+
    // | Initialize fields                                     |
    // +-------------------------------------------------------+

    this->fromSoundCard = false;
    this->fromFile = false;
    this->inputFileOpened = false;
    this->soundcardIndex = -1;
    this->inputFileName = "";
    this->toFile = false;
    this->outputFileOpened = false;
    this->outputFileName = "";

    this->streamIsRunning = false;

    // +---------------------------------------------------+
    // | Memory allocation                                 |
    // +---------------------------------------------------+

    initBuffers();

    // +-------------------------------------------------------+
    // | Connect the timer event to the slot                   |
    // +-------------------------------------------------------+

    QObject::connect(&this->inputFileTimer, SIGNAL(timeout()), this, SLOT(fileCallBack()));

}

/***********************************************************
* Destructor                                               *
***********************************************************/

InputOutputManager::~InputOutputManager()
{

    // +-------------------------------------------------------+
    // | Stop stream (in case it is running)                   |
    // +-------------------------------------------------------+

    this->stopStream();

    // +-------------------------------------------------------+
    // | Close stream (in case it is opened)                   |
    // +-------------------------------------------------------+

    this->closeStreamIfOpened();

    // +---------------------------------------------------+
    // | Memory liberation                                 |
    // +---------------------------------------------------+

    deleteBuffers();

    // +-------------------------------------------------------+
    // | Delete the RTAudio object                             |
    // +-------------------------------------------------------+

    delete this->rtaudio;

}

/***********************************************************
* Thread control                                           *
***********************************************************/

// +-------------------------------------------------------+
// | Start                                                 |
// +-------------------------------------------------------+

void InputOutputManager::startThread()
{
    this->startStream();
}

// +-------------------------------------------------------+
// | Stop                                                  |
// +-------------------------------------------------------+

void InputOutputManager::stopThread()
{
    bool streamWasRunning;

    streamWasRunning = this->streamIsRunning;

    this->stopStream();

    if (streamWasRunning == true)
    {
        this->flagEndAcquisition();
    }
}

/***********************************************************
* Stream callbacks                                         *
***********************************************************/

// +-------------------------------------------------------+
// | Sound card / File                                     |
// +-------------------------------------------------------+

void InputOutputManager::streamCallBack(signed short* inputBuffer)
{

    float** newBufferToFill;
    signed short* bufferToCopy;

    // Point to the buffer to copy
    bufferToCopy = inputBuffer;

    // Get an unused buffer to fill
    newBufferToFill = this->getUnusedBuffer();

    // Fill the buffer
    for (unsigned int indexSample = 0; indexSample < this->getHopSize(); indexSample++)
    {
        for (unsigned int indexChannel = 0; indexChannel < this->getNumberChannels(); indexChannel++)
        {
            newBufferToFill[indexChannel][indexSample] = ((float) *(bufferToCopy++)) / 32768.0;
        }
    }

    // Point to the buffer to copy
    bufferToCopy = inputBuffer;

    // Write in file if required
    if (this->toFile == true)
    {
        for (unsigned int indexSample = 0; indexSample < this->getHopSize(); indexSample++)
        {
            for (unsigned int indexChannel = 0; indexChannel < this->getNumberChannels(); indexChannel++)
            {
                fwrite(bufferToCopy++, sizeof(short), 1, this->outputFile);
            }
        }
    }

    // Add the filled buffer to the stack of buffers
    // ready to be processed
    this->pushFilledBuffer(newBufferToFill);

}

/***********************************************************
* External buffers management                              *
***********************************************************/

// +-------------------------------------------------------+
// | Flag end of acquisition                               |
// +-------------------------------------------------------+

void InputOutputManager::flagEndAcquisition()
{

    // Push a NULL pointer and this is known by the
    // running thread as the end of the acquisition
    this->pushFilledBuffer(NULL);

}

// +-------------------------------------------------------+
// | Wait for a buffer to be ready to processed            |
// +-------------------------------------------------------+

float** InputOutputManager::waitForBuffer()
{

    return this->getFilledBuffer();

}

// +-------------------------------------------------------+
// | Return a buffer that is no longer used                |
// +-------------------------------------------------------+

void InputOutputManager::giveBackBuffer(float** _buffer)
{

    // Make sure the buffer is not a null pointer
    if (_buffer != NULL)
    {
        this->pushUnusedBuffer(_buffer);
    }

}

// +-------------------------------------------------------+
// | Clean the stream                                      |
// +-------------------------------------------------------+

void InputOutputManager::cleanStream()
{

    moveAllFilledToUnused();

}

/***********************************************************
* User interface                                           *
***********************************************************/

// +-------------------------------------------------------+
// | Ask the user to select input/output                   |
// +-------------------------------------------------------+

void InputOutputManager::askUser(bool _validSelectionRequired)
{

    // +---------------------------------------------------+
    // | Predefined dimensions                             |
    // +---------------------------------------------------+

        // +-----------------------------------------------+
        // | Objects                                       |
        // +-----------------------------------------------+

            // +-------------------------------------------+
            // | Labels                                    |
            // +-------------------------------------------+

            unsigned int labelHeight = 15;
            unsigned int labelCenterSpace = 5;
            unsigned int labelMiddleSpace = 10;
            unsigned int labelTitleWidth = 150;
            unsigned int labelValueWidth = 150;

            // +-------------------------------------------+
            // | Radio buttons                             |
            // +-------------------------------------------+

            unsigned int radioButtonHeight = 15;
            unsigned int radioButtonBottomSpace = 5;
            unsigned int radioButtonWidth = 200;

            // +-------------------------------------------+
            // | List                                      |
            // +-------------------------------------------+

            unsigned int listHeight = 100;
            unsigned int listIndentation = 20;
            unsigned int listBottomSpace = 5;

            // +-------------------------------------------+
            // | Check Box                                 |
            // +-------------------------------------------+

            unsigned int checkBoxHeight = 15;
            unsigned int checkBoxWidth = 200;

            // +-------------------------------------------+
            // | Push Buttons                              |
            // +-------------------------------------------+

            unsigned int pushButtonHeight = 25;
            unsigned int pushButtonWidth = 75;
            unsigned int pushButtonSpace = 5;

        // +-----------------------------------------------+
        // | GroupBoxes                                    |
        // +-----------------------------------------------+

        unsigned int groupBoxLeftMargin = 15;
        unsigned int groupBoxRightMargin = 15;
        unsigned int groupBoxTopMargin = 20;
        unsigned int groupBoxBottomMargin = 15;

        // +-----------------------------------------------+
        // | Dialog                                        |
        // +-----------------------------------------------+

        unsigned int dialogLeftMargin = 5;
        unsigned int dialogRightMargin = 5;
        unsigned int dialogTopMargin = 5;
        unsigned int dialogBottomMargin = 5;
        unsigned int dialogMiddleSpace = 5;

    // +---------------------------------------------------+
    // | Auto-scale dimensions                             |
    // +---------------------------------------------------+

        // +-----------------------------------------------+
        // | Objects                                       |
        // +-----------------------------------------------+

            // +-------------------------------------------+
            // | Labels                                    |
            // +-------------------------------------------+

            unsigned int labelSampleRateTitleHeight = labelHeight;
            unsigned int labelSampleRateTitleWidth = labelTitleWidth;
            unsigned int labelSampleRateValueHeight = labelHeight;
            unsigned int labelSampleRateValueWidth = labelValueWidth;

            unsigned int labelFrameSizeTitleHeight = labelHeight;
            unsigned int labelFrameSizeTitleWidth = labelTitleWidth;
            unsigned int labelFrameSizeValueHeight = labelHeight;
            unsigned int labelFrameSizeValueWidth = labelValueWidth;

            unsigned int labelHopSizeTitleHeight = labelHeight;
            unsigned int labelHopSizeTitleWidth = labelTitleWidth;
            unsigned int labelHopSizeValueHeight = labelHeight;
            unsigned int labelHopSizeValueWidth = labelValueWidth;

            unsigned int labelNumberChannelsTitleHeight = labelHeight;
            unsigned int labelNumberChannelsTitleWidth = labelTitleWidth;
            unsigned int labelNumberChannelsValueHeight = labelHeight;
            unsigned int labelNumberChannelsValueWidth = labelValueWidth;

            // +-------------------------------------------+
            // | Radio buttons                             |
            // +-------------------------------------------+

            unsigned int radioButtonSoundCardHeight = radioButtonHeight;
            unsigned int radioButtonSoundCardWidth = radioButtonWidth;
            unsigned int radioButtonInputFileHeight = radioButtonHeight;
            unsigned int radioButtonInputFileWidth = radioButtonWidth;

            // +-------------------------------------------+
            // | Check Box                                 |
            // +-------------------------------------------+

            unsigned int checkBoxOutputFileHeight = checkBoxHeight;
            unsigned int checkBoxOutputFileWidth = checkBoxWidth;

            // +-------------------------------------------+
            // | List                                      |
            // +-------------------------------------------+

            unsigned int listSoundCardHeight = listHeight;
            unsigned int listSoundCardWidth = labelTitleWidth +
                                              labelCenterSpace +
                                              labelValueWidth -
                                              listIndentation;

            // +-------------------------------------------+
            // | Push Buttons                              |
            // +-------------------------------------------+

            unsigned int pushButtonOkHeight = pushButtonHeight;
            unsigned int pushButtonOkWidth = pushButtonWidth;
            unsigned int pushButtonCancelHeight = pushButtonHeight;
            unsigned int pushButtonCancelWidth = pushButtonWidth;

        // +-----------------------------------------------+
        // | GroupBoxes                                    |
        // +-----------------------------------------------+

        unsigned int groupBoxWidth = groupBoxLeftMargin +
                                     labelTitleWidth +
                                     labelCenterSpace +
                                     labelValueWidth +
                                     groupBoxRightMargin;

        unsigned int groupBoxParametersHeight = groupBoxTopMargin +
                                                labelHeight +
                                                labelMiddleSpace +
                                                labelHeight +
                                                labelMiddleSpace +
                                                labelHeight +
                                                labelMiddleSpace +
                                                labelHeight +
                                                groupBoxBottomMargin;

        unsigned int groupBoxParametersWidth = groupBoxLeftMargin +
                                               labelTitleWidth +
                                               labelCenterSpace +
                                               labelValueWidth +
                                               groupBoxRightMargin;

        unsigned int groupBoxInputHeight = groupBoxTopMargin +
                                           radioButtonHeight +
                                           radioButtonBottomSpace +
                                           listHeight +
                                           listBottomSpace +
                                           radioButtonHeight +
                                           groupBoxBottomMargin;

        unsigned int groupBoxInputWidth = groupBoxLeftMargin +
                                          labelTitleWidth +
                                          labelCenterSpace +
                                          labelValueWidth +
                                          groupBoxRightMargin;

        unsigned int groupBoxOutputHeight = groupBoxTopMargin +
                                            radioButtonHeight +
                                            groupBoxBottomMargin;

        unsigned int groupBoxOutputWidth = groupBoxLeftMargin +
                                           labelTitleWidth +
                                           labelCenterSpace +
                                           labelValueWidth +
                                           groupBoxRightMargin;

        // +-----------------------------------------------+
        // | Dialog                                        |
        // +-----------------------------------------------+

        unsigned int dialogWidth = dialogLeftMargin +
                                   groupBoxWidth +
                                   dialogRightMargin;
        unsigned int dialogHeight = dialogTopMargin +
                                    groupBoxParametersHeight +
                                    dialogMiddleSpace +
                                    groupBoxInputHeight +
                                    dialogMiddleSpace +
                                    groupBoxOutputHeight +
                                    dialogMiddleSpace +
                                    pushButtonHeight +
                                    dialogBottomMargin;

    // +---------------------------------------------------+
    // | Auto-scale positions                              |
    // +---------------------------------------------------+

        // +-----------------------------------------------+
        // | GroupBoxes                                    |
        // +-----------------------------------------------+

        unsigned int groupBoxParametersTop = dialogTopMargin;
        unsigned int groupBoxParametersLeft = dialogLeftMargin;
        unsigned int groupBoxInputTop = dialogTopMargin +
                                        groupBoxParametersHeight +
                                        dialogMiddleSpace;
        unsigned int groupBoxInputLeft = dialogLeftMargin;
        unsigned int groupBoxOutputTop = dialogTopMargin +
                                         groupBoxParametersHeight +
                                         dialogMiddleSpace +
                                         groupBoxInputHeight +
                                         dialogMiddleSpace;
        unsigned int groupBoxOutputLeft = dialogLeftMargin;

        // +-----------------------------------------------+
        // | Objects                                       |
        // +-----------------------------------------------+

            // +-------------------------------------------+
            // | Labels                                    |
            // +-------------------------------------------+

            unsigned int labelSampleRateTitleTop = groupBoxTopMargin;
            unsigned int labelSampleRateTitleLeft = groupBoxLeftMargin;
            unsigned int labelSampleRateValueTop = groupBoxTopMargin;
            unsigned int labelSampleRateValueLeft = groupBoxLeftMargin +
                                                    labelSampleRateTitleWidth +
                                                    labelCenterSpace;

            unsigned int labelFrameSizeTitleTop = groupBoxTopMargin +
                                                  labelSampleRateTitleHeight +
                                                  labelMiddleSpace;
            unsigned int labelFrameSizeTitleLeft = groupBoxLeftMargin;
            unsigned int labelFrameSizeValueTop = groupBoxTopMargin +
                                                  labelSampleRateValueHeight +
                                                  labelMiddleSpace;
            unsigned int labelFrameSizeValueLeft = groupBoxLeftMargin +
                                                    labelFrameSizeValueWidth +
                                                    labelCenterSpace;

            unsigned int labelHopSizeTitleTop = groupBoxTopMargin +
                                                  labelSampleRateTitleHeight +
                                                  labelMiddleSpace +
                                                  labelFrameSizeTitleHeight +
                                                  labelMiddleSpace;
            unsigned int labelHopSizeTitleLeft = groupBoxLeftMargin;
            unsigned int labelHopSizeValueTop = groupBoxTopMargin +
                                                labelSampleRateValueHeight +
                                                labelMiddleSpace +
                                                  labelFrameSizeValueHeight +
                                                  labelMiddleSpace;
            unsigned int labelHopSizeValueLeft = groupBoxLeftMargin +
                                                    labelHopSizeTitleWidth +
                                                    labelCenterSpace;

            unsigned int labelNumberChannelsTitleTop = groupBoxTopMargin +
                                                       labelSampleRateTitleHeight +
                                                       labelMiddleSpace +
                                                       labelFrameSizeTitleHeight +
                                                       labelMiddleSpace +
                                                       labelHopSizeTitleHeight +
                                                       labelMiddleSpace;
            unsigned int labelNumberChannelsTitleLeft = groupBoxLeftMargin;
            unsigned int labelNumberChannelsValueTop = groupBoxTopMargin +
                                                       labelSampleRateValueHeight +
                                                       labelMiddleSpace +
                                                       labelFrameSizeValueHeight +
                                                       labelMiddleSpace +
                                                       labelHopSizeValueHeight +
                                                       labelMiddleSpace;
            unsigned int labelNumberChannelsValueLeft = groupBoxLeftMargin +
                                                        labelNumberChannelsTitleWidth +
                                                        labelCenterSpace;

            // +-------------------------------------------+
            // | Radio buttons                             |
            // +-------------------------------------------+

            unsigned int radioButtonSoundCardTop = groupBoxTopMargin;
            unsigned int radioButtonSoundCardLeft = groupBoxLeftMargin;
            unsigned int radioButtonInputFileTop = groupBoxTopMargin +
                                                   radioButtonSoundCardHeight +
                                                   radioButtonBottomSpace +
                                                   listSoundCardHeight +
                                                   listBottomSpace;
            unsigned int radioButtonInputFileLeft = groupBoxLeftMargin;

            // +-------------------------------------------+
            // | Check Box                                 |
            // +-------------------------------------------+

            unsigned int checkBoxOutputFileTop = groupBoxTopMargin;
            unsigned int checkBoxOutputFileLeft = groupBoxLeftMargin;

            // +-------------------------------------------+
            // | List                                      |
            // +-------------------------------------------+

            unsigned int listSoundCardTop = groupBoxTopMargin +
                                            radioButtonSoundCardHeight +
                                            radioButtonBottomSpace;
            unsigned int listSoundCardLeft = groupBoxLeftMargin +
                                             listIndentation;

            // +-------------------------------------------+
            // | Push Buttons                              |
            // +-------------------------------------------+

            unsigned int pushButtonOkTop = dialogTopMargin +
                                           groupBoxParametersHeight +
                                           dialogMiddleSpace +
                                           groupBoxInputHeight +
                                           dialogMiddleSpace +
                                           groupBoxOutputHeight +
                                           dialogMiddleSpace;
            unsigned int pushButtonOkLeft = dialogWidth -
                                            dialogRightMargin -
                                            pushButtonCancelWidth -
                                            pushButtonSpace -
                                            pushButtonOkWidth;
            unsigned int pushButtonCancelTop = dialogTopMargin +
                                               groupBoxParametersHeight +
                                               dialogMiddleSpace +
                                               groupBoxInputHeight +
                                               dialogMiddleSpace +
                                               groupBoxOutputHeight +
                                               dialogMiddleSpace;
            unsigned int pushButtonCancelLeft = dialogWidth -
                                                dialogRightMargin -
                                                pushButtonCancelWidth;

    // +---------------------------------------------------+
    // | Create objects                                    |
    // +---------------------------------------------------+

        // +-----------------------------------------------+
        // | Dialog                                        |
        // +-----------------------------------------------+

        this->dialog = new QDialog();

        this->dialog->setFixedHeight(dialogHeight);
        this->dialog->setFixedWidth(dialogWidth);

        // +-----------------------------------------------+
        // | Group Boxes                                   |
        // +-----------------------------------------------+

        this->groupBoxParameters = new QGroupBox(dialog);

        this->groupBoxParameters->setGeometry(groupBoxParametersLeft,
                                              groupBoxParametersTop,
                                              groupBoxParametersWidth,
                                              groupBoxParametersHeight);

        this->groupBoxParameters->setFont(QFont("Tahoma", 8, QFont::Bold, false));
        this->groupBoxParameters->setTitle("Parameters");

        this->groupBoxInput = new QGroupBox(dialog);

        this->groupBoxInput->setGeometry(groupBoxInputLeft,
                                         groupBoxInputTop,
                                         groupBoxInputWidth,
                                         groupBoxInputHeight);

        this->groupBoxInput->setFont(QFont("Tahoma", 8, QFont::Bold, false));
        this->groupBoxInput->setTitle("Input");

        this->groupBoxOutput = new QGroupBox(dialog);

        this->groupBoxOutput->setGeometry(groupBoxOutputLeft,
                                          groupBoxOutputTop,
                                          groupBoxOutputWidth,
                                          groupBoxOutputHeight);

        this->groupBoxOutput->setFont(QFont("Tahoma", 8, QFont::Bold, false));
        this->groupBoxOutput->setTitle("Output");

        // +-----------------------------------------------+
        // | Objects                                       |
        // +-----------------------------------------------+

            // +-------------------------------------------+
            // | Labels                                    |
            // +-------------------------------------------+

            this->labelSampleRateTitle = new QLabel(groupBoxParameters);

            this->labelSampleRateTitle->setGeometry(labelSampleRateTitleLeft,
                                                    labelSampleRateTitleTop,
                                                    labelSampleRateTitleWidth,
                                                    labelSampleRateTitleHeight);

            this->labelSampleRateTitle->setText("Sample rate:");
            this->labelSampleRateTitle->setFont(QFont("Tahoma", 8, QFont::Normal, false));
            this->labelSampleRateTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            this->labelSampleRateValue = new QLabel(groupBoxParameters);

            this->labelSampleRateValue->setGeometry(labelSampleRateValueLeft,
                                                    labelSampleRateValueTop,
                                                    labelSampleRateValueWidth,
                                                    labelSampleRateValueHeight);

            this->labelSampleRateValue->setText(QString::number(this->getSampleRate()));
            this->labelSampleRateValue->setFont(QFont("Tahoma", 8, QFont::Normal, false));
            this->labelSampleRateValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);


            this->labelFrameSizeTitle = new QLabel(groupBoxParameters);

            this->labelFrameSizeTitle->setGeometry(labelFrameSizeTitleLeft,
                                                   labelFrameSizeTitleTop,
                                                   labelFrameSizeTitleWidth,
                                                   labelFrameSizeTitleHeight);

            this->labelFrameSizeTitle->setText("Frame size:");
            this->labelFrameSizeTitle->setFont(QFont("Tahoma", 8, QFont::Normal, false));
            this->labelFrameSizeTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            this->labelFrameSizeValue = new QLabel(groupBoxParameters);

            this->labelFrameSizeValue->setGeometry(labelFrameSizeValueLeft,
                                                   labelFrameSizeValueTop,
                                                   labelFrameSizeValueWidth,
                                                   labelFrameSizeValueHeight);

            this->labelFrameSizeValue->setText(QString::number(this->getFrameSize()));
            this->labelFrameSizeValue->setFont(QFont("Tahoma", 8, QFont::Normal, false));
            this->labelFrameSizeValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);


            this->labelHopSizeTitle = new QLabel(groupBoxParameters);

            this->labelHopSizeTitle->setGeometry(labelHopSizeTitleLeft,
                                                 labelHopSizeTitleTop,
                                                 labelHopSizeTitleWidth,
                                                 labelHopSizeTitleHeight);

            this->labelHopSizeTitle->setText("Hop size:");
            this->labelHopSizeTitle->setFont(QFont("Tahoma", 8, QFont::Normal, false));
            this->labelHopSizeTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            this->labelHopSizeValue = new QLabel(groupBoxParameters);

            this->labelHopSizeValue->setGeometry(labelHopSizeValueLeft,
                                                 labelHopSizeValueTop,
                                                 labelHopSizeValueWidth,
                                                 labelHopSizeValueHeight);

            this->labelHopSizeValue->setText(QString::number(this->getHopSize()));
            this->labelHopSizeValue->setFont(QFont("Tahoma", 8, QFont::Normal, false));
            this->labelHopSizeValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);


            this->labelNumberChannelsTitle = new QLabel(groupBoxParameters);

            this->labelNumberChannelsTitle->setGeometry(labelNumberChannelsTitleLeft,
                                                        labelNumberChannelsTitleTop,
                                                        labelNumberChannelsTitleWidth,
                                                        labelNumberChannelsTitleHeight);

            this->labelNumberChannelsTitle->setText("Number of channels:");
            this->labelNumberChannelsTitle->setFont(QFont("Tahoma", 8, QFont::Normal, false));
            this->labelNumberChannelsTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            this->labelNumberChannelsValue = new QLabel(groupBoxParameters);

            this->labelNumberChannelsValue->setGeometry(labelNumberChannelsValueLeft,
                                                        labelNumberChannelsValueTop,
                                                        labelNumberChannelsValueWidth,
                                                        labelNumberChannelsValueHeight);

            this->labelNumberChannelsValue->setText(QString::number(this->getNumberChannels()));
            this->labelNumberChannelsValue->setFont(QFont("Tahoma", 8, QFont::Normal, false));
            this->labelNumberChannelsValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            // +-------------------------------------------+
            // | Radio Buttons                             |
            // +-------------------------------------------+

            this->radioButtonSoundCard = new QRadioButton(groupBoxInput);

            this->radioButtonSoundCard->setGeometry(radioButtonSoundCardLeft,
                                                    radioButtonSoundCardTop,
                                                    radioButtonSoundCardWidth,
                                                    radioButtonSoundCardHeight);

            this->radioButtonSoundCard->setText("Use an audio device:");
            this->radioButtonSoundCard->setFont(QFont("Tahoma", 8, QFont::Normal, false));

            this->radioButtonInputFile = new QRadioButton(groupBoxInput);

            this->radioButtonInputFile->setGeometry(radioButtonInputFileLeft,
                                                    radioButtonInputFileTop,
                                                    radioButtonInputFileWidth,
                                                    radioButtonInputFileHeight);

            this->radioButtonInputFile->setText("Use a raw file");
            this->radioButtonInputFile->setFont(QFont("Tahoma", 8, QFont::Normal, false));

            // +-------------------------------------------+
            // | List                                      |
            // +-------------------------------------------+

            this->listSoundCard = new QListWidget(groupBoxInput);

            this->listSoundCard->setGeometry(listSoundCardLeft,
                                             listSoundCardTop,
                                             listSoundCardWidth,
                                             listSoundCardHeight);
            this->listSoundCard->setFont(QFont("Tahoma", 8, QFont::Normal, false));

            // +-------------------------------------------+
            // | CheckBox                                  |
            // +-------------------------------------------+

            this->checkBox = new QCheckBox(groupBoxOutput);

            this->checkBox->setGeometry(checkBoxOutputFileLeft,
                                        checkBoxOutputFileTop,
                                        checkBoxOutputFileWidth,
                                        checkBoxOutputFileHeight);

            this->checkBox->setText("Save the raw audio in a file");
            this->checkBox->setFont(QFont("Tahoma", 8, QFont::Normal, false));

            // +-------------------------------------------+
            // | Push Buttons                              |
            // +-------------------------------------------+

            this->pushButtonOk = new QPushButton(dialog);

            this->pushButtonOk->setGeometry(pushButtonOkLeft,
                                            pushButtonOkTop,
                                            pushButtonOkWidth,
                                            pushButtonOkHeight);

            this->pushButtonOk->setText("OK");

            this->pushButtonCancel = new QPushButton(dialog);

            this->pushButtonCancel->setGeometry(pushButtonCancelLeft,
                                                pushButtonCancelTop,
                                                pushButtonCancelWidth,
                                                pushButtonCancelHeight);

            this->pushButtonCancel->setText("Cancel");

    // +---------------------------------------------------+
    // | Populate the list of sound cards                  |
    // +---------------------------------------------------+

    QVector<QString> strSoundCards = this->getListSoundcards();

    for (int indexSoundCard = 0; indexSoundCard < strSoundCards.size(); indexSoundCard++)
    {
        this->listSoundCard->addItem(strSoundCards.at(indexSoundCard));
    }

    // +---------------------------------------------------+
    // | Default selections                                |
    // +---------------------------------------------------+

    if (this->listSoundCard->count() > 0)
    {
        this->listSoundCard->setCurrentRow(0);
    }

    this->radioButtonSoundCard->setChecked(true);
    this->radioButtonInputFile->setChecked(false);
    this->checkBox->setChecked(false);

    // +---------------------------------------------------+
    // | Connect signals to slots for user interaction     |
    // +---------------------------------------------------+

    QObject::connect(this->pushButtonOk, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
    QObject::connect(this->pushButtonCancel, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
    QObject::connect(this->radioButtonSoundCard, SIGNAL(clicked()), this, SLOT(soundCardRadioButtonClicked()));
    QObject::connect(this->radioButtonInputFile, SIGNAL(clicked()), this, SLOT(fileInputRadioButtonClicked()));
    QObject::connect(this->listSoundCard, SIGNAL(itemSelectionChanged()), this, SLOT(listItemSelectionChanged()));
    QObject::connect(this->listSoundCard, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listDoubleClicked(QListWidgetItem*)));

    // +---------------------------------------------------+
    // | Show dialog                                       |
    // +---------------------------------------------------+

    dialog->exec();  

    // +---------------------------------------------------+
    // | Delete objects                                    |
    // +---------------------------------------------------+

        // +-----------------------------------------------+
        // | Objects                                       |
        // +-----------------------------------------------+

            // +-------------------------------------------+
            // | Labels                                    |
            // +-------------------------------------------+

            delete this->labelSampleRateTitle;
            delete this->labelSampleRateValue;
            delete this->labelFrameSizeTitle;
            delete this->labelFrameSizeValue;
            delete this->labelHopSizeTitle;
            delete this->labelHopSizeValue;
            delete this->labelNumberChannelsTitle;
            delete this->labelNumberChannelsValue;

            // +-------------------------------------------+
            // | Radio Buttons                             |
            // +-------------------------------------------+

            delete this->radioButtonSoundCard;
            delete this->radioButtonInputFile;

            // +-------------------------------------------+
            // | CheckBox                                  |
            // +-------------------------------------------+

            delete this->checkBox;

            // +-------------------------------------------+
            // | List                                      |
            // +-------------------------------------------+

            delete this->listSoundCard;

            // +-------------------------------------------+
            // | Push Buttons                              |
            // +-------------------------------------------+

            delete this->pushButtonOk;
            delete this->pushButtonCancel;

        // +-----------------------------------------------+
        // | Group Boxes                                   |
        // +-----------------------------------------------+

        delete this->groupBoxParameters;
        delete this->groupBoxInput;
        delete this->groupBoxOutput;

        // +-----------------------------------------------+
        // | Dialog                                        |
        // +-----------------------------------------------+

        delete this->dialog;

    // +---------------------------------------------------+
    // | Update, quit or ignore                            |
    // +---------------------------------------------------+

    // If a selection is required, then the application will
    // exit if there is no valid selection

    if (this->projectionIsValid == false)
    {

        if (_validSelectionRequired == true)
        {

            QCoreApplication::quit();

        }

    }
    else
    {

        // First stop and close the stream
        this->endOfStream();
        this->closeStreamIfOpened();

        // Load the new parameters
        this->fromSoundCard = this->projFromSoundCard;
        this->soundcardIndex = this->projSoundcardIndex;
        this->fromFile = this->projFromFile;
        this->inputFileName = this->projInputFileName;
        this->toFile = this->projToFile;
        this->outputFileName = this->projOutputFileName;

        // Open a new stream
        this->openStream();

    }

}

/***********************************************************
* Event receivers                                          *
***********************************************************/

// +-------------------------------------------------------+
// | Add an event receiver                                 |
// +-------------------------------------------------------+

void InputOutputManager::pushEventReceiver(QObject* _receiver)
{

    this->streamEventReceiver.push_back(_receiver);

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | General information regarding data format             |
// +-------------------------------------------------------+

const unsigned int InputOutputManager::getHopSize()
{
    unsigned int tmp;

    this->mutexHopSize.lock();
    tmp = this->hopSize;
    this->mutexHopSize.unlock();

    return tmp;
}

const unsigned int InputOutputManager::getFrameSize()
{
    unsigned int tmp;

    this->mutexFrameSize.lock();
    tmp = this->frameSize;
    this->mutexFrameSize.unlock();

    return tmp;
}

const unsigned int InputOutputManager::getSampleRate()
{
    unsigned int tmp;

    this->mutexSampleRate.lock();
    tmp = this->sampleRate;
    this->mutexSampleRate.unlock();

    return tmp;
}

const unsigned int InputOutputManager::getNumberChannels()
{
    unsigned int tmp;

    this->mutexNumberChannels.lock();
    tmp = this->numberChannels;
    this->mutexNumberChannels.unlock();

    return tmp;
}


/***********************************************************
* Internal buffers management                              *
***********************************************************/

// +-------------------------------------------------------+
// | Create a new buffer                                   |
// +-------------------------------------------------------+

float** InputOutputManager::createBuffer()
{

    float** newBuffer;

    newBuffer = new float*[this->getNumberChannels()];

    for (int indexChannel = 0; indexChannel < this->getNumberChannels(); indexChannel++)
    {
        newBuffer[indexChannel] = new float[this->getHopSize()];
    }

    return newBuffer;

}

// +-------------------------------------------------------+
// | Delete an existing buffer                             |
// +-------------------------------------------------------+

void InputOutputManager::deleteBuffer(float** _buffer)
{

    for (int indexChannel = 0; indexChannel < this->getNumberChannels(); indexChannel++)
    {

        delete _buffer[indexChannel];

    }

    delete _buffer;

}

// +-------------------------------------------------------+
// | Initialize the memory for the buffers                 |
// +-------------------------------------------------------+

void InputOutputManager::initBuffers()
{

    // Clear the vectors
    this->unusedBuffersStack.clear();
    this->filledBuffersStack.clear();

    // Allocate memory for the buffers
    for (int indexBuffer = 0; indexBuffer < BUFFERS_RESERVE; indexBuffer++)
    {
        this->unusedBuffersStack.push_front(createBuffer());
    }

}

// +-------------------------------------------------------+
// | Clear the memory for the buffers                      |
// +-------------------------------------------------------+

void InputOutputManager::deleteBuffers()
{

    // Empty the unused buffers stack
    while (this->unusedBuffersStack.size() > 0)
    {

        deleteBuffer(this->unusedBuffersStack.front());
        this->unusedBuffersStack.pop_front();

    }

    // Empty the filled buffers stack
    while (this->filledBuffersStack.size() > 0)
    {

        deleteBuffer(this->filledBuffersStack.front());
        this->filledBuffersStack.pop_front();

    }

}

// +-------------------------------------------------------+
// | Get an available buffer to fill                       |
// +-------------------------------------------------------+

float** InputOutputManager::getUnusedBuffer()
{

    float** newBuffer;   

    // Protect this read operation with a mutex
    this->unusedBuffersStackIsUsed.lock();

    // Check if there is an available buffer
    if (this->unusedBuffersStack.size() > 0)
    {
        // If so then take one of the available buffers
        newBuffer = this->unusedBuffersStack.front();
        this->unusedBuffersStack.pop_front();
    }
    else
    {
        // Otherwise create a new buffer (takes more time)
        newBuffer = createBuffer();
    }

    this->unusedBuffersStackIsUsed.unlock();

    return newBuffer;

}

// +-------------------------------------------------------+
// | Push an unused buffer in the stack                    |
// +-------------------------------------------------------+

void InputOutputManager::pushUnusedBuffer(float** _unusedBuffer)
{

    // Protect this write operation with a mutex
    this->unusedBuffersStackIsUsed.lock();
    this->unusedBuffersStack.push_back(_unusedBuffer);
    this->unusedBuffersStackIsUsed.unlock();

}

// +-------------------------------------------------------+
// | Get an available buffer to process                    |
// +-------------------------------------------------------+

float** InputOutputManager::getFilledBuffer()
{

    float** filledBuffer;

    // Decrement the semaphore (or wait on it)
    this->waitingFilledBuffers.acquire();

    // Protect this read operation with a mutex
    this->filledBuffersStackIsUsed.lock();

    // Check if there is a filled buffer
    if (this->filledBuffersStack.size() > 0)
    {
        // If so return it
        filledBuffer = this->filledBuffersStack.front();
        this->filledBuffersStack.pop_front();

    }
    else
    {
        // Otherwise return NULL
        filledBuffer = NULL;
    }

    this->filledBuffersStackIsUsed.unlock();

    return filledBuffer;

}

// +-------------------------------------------------------+
// | Push a filled buffer in the stack                     |
// +-------------------------------------------------------+

void InputOutputManager::pushFilledBuffer(float** _filledBuffer)
{

    // Protect this write operation with a mutex
    this->filledBuffersStackIsUsed.lock();
    this->filledBuffersStack.push_back(_filledBuffer);
    this->filledBuffersStackIsUsed.unlock();

    // Increment the semaphore
    this->waitingFilledBuffers.release();

}

// +-------------------------------------------------------+
// | Move all filled buffers to unused buffers             |
// +-------------------------------------------------------+

void InputOutputManager::moveAllFilledToUnused()
{

    bool filledNotEmpty;
    float** filledBuffer;

    filledNotEmpty = true;

    while(filledNotEmpty)
    {

        // Protect this read operation with a mutex
        this->filledBuffersStackIsUsed.lock();

        // Check if there is a filled buffer
        if (this->filledBuffersStack.size() > 0)
        {
            // If so return it
            filledBuffer = this->filledBuffersStack.front();
            this->filledBuffersStack.pop_front();

            filledNotEmpty = true;

        }
        else
        {
            filledNotEmpty = false;
        }

        this->filledBuffersStackIsUsed.unlock();

        if (filledNotEmpty == true)
        {
            if (filledBuffer != NULL)
            {
                this->unusedBuffersStackIsUsed.lock();
                this->unusedBuffersStack.push_back(filledBuffer);
                this->unusedBuffersStackIsUsed.unlock();
            }
        }

    }

    // Reset semaphore
    this->waitingFilledBuffers.acquire(this->waitingFilledBuffers.available());

}

/***********************************************************
* Sound card                                               *
***********************************************************/

// +-------------------------------------------------------+
// | Get the list of the sound cards                       |
// +-------------------------------------------------------+

QVector<QString> InputOutputManager::getListSoundcards()
{

    // Determine the number of devices available
    unsigned int numberDevices = this->rtaudio->getDeviceCount();

    // Create an empty vector
    QVector<QString> rtnStrings;

    // Scan through devices for various capabilities
    RtAudio::DeviceInfo info;

    for (unsigned int indexDevice = 0; indexDevice < numberDevices; indexDevice++ )
    {

      info = this->rtaudio->getDeviceInfo(indexDevice);

      if ( info.probed == true )
      {

          if (info.inputChannels >=  this->getNumberChannels())
          {
              qDebug("Adding compatible card : %s",info.name.c_str());
              rtnStrings.push_back(QString(info.name.c_str()));
          }
          else
          {
              qDebug() << "Rejecting device (not enough input channels) : " << QString(info.name.c_str()) + " nbInputChannels : " + QString::number(info.inputChannels);
          }
      }

    }

    return rtnStrings;

}

/***********************************************************
* Stream                                                   *
***********************************************************/

// +-------------------------------------------------------+
// | Open the stream                                       |
// +-------------------------------------------------------+

void InputOutputManager::openStream()
{

    // +---------------------------------------------------+
    // | Stream configuration                              |
    // +---------------------------------------------------+

        // +-----------------------------------------------+
        // | Sound card                                    |
        // +-----------------------------------------------+

        if (this->fromSoundCard == true)
        {

            // Parameters for the stream
            RtAudio::StreamParameters audioParameters;


            //DL Index modified to fit compatible sound cards...
            audioParameters.deviceId = this->soundcardIndex;


            audioParameters.firstChannel = 0;
            audioParameters.nChannels = this->getNumberChannels();

            // Options for the stream
            RtAudio::StreamOptions audioOptions;

            // Sample rate and size
            int tmpSampleRate = this->getSampleRate();
            unsigned int tmpBufferSize = (unsigned int) this->getHopSize();

            // Open the stream
            try
            {
                this->rtaudio->openStream(NULL, &audioParameters, RTAUDIO_SINT16, tmpSampleRate,&tmpBufferSize, &rtAudioCallBack, this, &audioOptions);
            }
            catch ( RtError& e )
            {
                qDebug("RtError : %s",e.what());
            }

        }

        // +-----------------------------------------------+
        // | Input file                                    |
        // +-----------------------------------------------+

        if (this->fromFile == true)
        {

            // Parameters for the stream
            QString filePath = this->inputFileName;

            // Open the stream
            this->inputFile = fopen(filePath.toStdString().c_str(), "rb");

            // Flag it
            this->inputFileOpened = true;

            // Set the timer setting
            float timeOneFrame;
            timeOneFrame = ((float) this->getHopSize()) / ((float) this->getSampleRate());
            this->inputFileTimer.setInterval(timeOneFrame * 1000);

            // Create the buffer to receive samples
            this->inputFileBuffer = new signed short[this->getHopSize() * this->getNumberChannels()];

        }

        // +-----------------------------------------------+
        // | Output file                                   |
        // +-----------------------------------------------+

        if (this->toFile == true)
        {

            // Parameters for the stream
            QString filePath = this->outputFileName;

            // Open the stream
            this->outputFile = fopen(filePath.toStdString().c_str(), "wb");

            // Flag it
            this->outputFileOpened = true;

        }

}

// +-------------------------------------------------------+
// | Open the stream if closed                             |
// +-------------------------------------------------------+

void InputOutputManager::openStreamIfClosed()
{

    if (this->fromSoundCard == true)
    {
        if (this->rtaudio->isStreamOpen() == false)
        {
            openStream();
        }
    }

    if (this->fromFile == true)
    {
        if (this->inputFileOpened == false)
        {
            openStream();
        }
    }

}


// +-------------------------------------------------------+
// | Close the stream if already opened                    |
// +-------------------------------------------------------+

void InputOutputManager::closeStreamIfOpened()
{

    // +---------------------------------------------------+
    // | Stream termination                                |
    // +---------------------------------------------------+

        // +-----------------------------------------------+
        // | Sound card                                    |
        // +-----------------------------------------------+

        if (this->fromSoundCard == true)
        {
            // Check if the stream was opened in the first place
            if (this->rtaudio->isStreamOpen() == true)
            {
                this->rtaudio->closeStream();
            }
        }

        // +-----------------------------------------------+
        // | Input file                                    |
        // +-----------------------------------------------+

        if (this->fromFile == true)
        {
            // Check if the stream was opened in the first place
            if (this->inputFileOpened == true)
            {
                fclose(this->inputFile);
                delete this->inputFileBuffer;
                this->inputFileOpened = false;
            }
        }

        // +-----------------------------------------------+
        // | Output file                                   |
        // +-----------------------------------------------+

        if (this->toFile == true)
        {

            // Check if the stream was opened in the first place
            if (this->outputFileOpened == true)
            {
                fclose(this->outputFile);
                this->outputFileOpened = false;
            }

        }

}

// +-------------------------------------------------------+
// | Start the stream                                      |
// +-------------------------------------------------------+

void InputOutputManager::startStream()
{

    // +---------------------------------------------------+
    // | Start acquisition                                 |
    // +---------------------------------------------------+

        // +-----------------------------------------------+
        // | Sound card                                    |
        // +-----------------------------------------------+

        if (this->fromSoundCard == true)
        {
            qDebug("Start stream from sound card");


            if (rtaudio->isStreamOpen())
            {
                this->rtaudio->startStream();
                this->streamIsRunning = true;
            }
            else
            {
                qDebug("Stream is not opened, cannot start.");

            }
        }

        // +-----------------------------------------------+
        // | Input file                                    |
        // +-----------------------------------------------+

        if (this->fromFile == true)
        {

            // Reset file pointer
            if (this->inputFileOpened == true)
            {
                fclose(this->inputFile);

                QString filePath = this->inputFileName;

                this->inputFile = fopen(filePath.toStdString().c_str(), "rb");
            }

            this->inputFileTimer.start();
            this->streamIsRunning = true;

        }

        // +-----------------------------------------------+
        // | Output file                                   |
        // +-----------------------------------------------+

        if (this->toFile == true)
        {

            // Reset file pointer
            if (this->outputFileOpened == true)
            {
                fclose(this->outputFile);

                QString filePath = this->outputFileName;

                this->outputFile = fopen(filePath.toStdString().c_str(), "wb");
            }

        }

}

// +-------------------------------------------------------+
// | Stop the stream                                       |
// +-------------------------------------------------------+

void InputOutputManager::stopStream()
{

    if (this->streamIsRunning == true)
    {

        // +-----------------------------------------------+
        // | Stop acquisition                              |
        // +-----------------------------------------------+

            // +-------------------------------------------+
            // | Sound card                                |
            // +-------------------------------------------+

            if (this->fromSoundCard == true)
            {
                this->rtaudio->stopStream();
                this->streamIsRunning = false;
            }

            // +-------------------------------------------+
            // | Input file                                |
            // +-------------------------------------------+

            if (this->fromFile == true)
            {
                this->inputFileTimer.stop();
                this->streamIsRunning = false;
            }

    }

}

// +-------------------------------------------------------+
// | Handle the end of the stream                          |
// +-------------------------------------------------------+

void InputOutputManager::endOfStream()
{

    this->stopThread();

    // Post an event
    for (int indexReceiver = 0; indexReceiver < this->streamEventReceiver.size(); indexReceiver++)
    {
        QCoreApplication::postEvent(this->streamEventReceiver.at(indexReceiver), new EndOfStreamEvent());
    }

}


/***********************************************************
* User interface                                           *
***********************************************************/

// +-------------------------------------------------------+
// | Ok push button is pressed                             |
// +-------------------------------------------------------+

void InputOutputManager::okPushButtonClicked()
{

    bool inputIsValid;
    bool outputIsValid;

    inputIsValid = false;
    outputIsValid = false;

    // +---------------------------------------------------+
    // | Check if a sound card is selected                 |
    // +---------------------------------------------------+

    if (this->radioButtonSoundCard->isChecked() == true)
    {
        if (this->listSoundCard->currentRow() != -1)
        {
            this->projFromSoundCard = true;
            this->projFromFile = false;

            //this->projSoundcardIndex = this->listSoundCard->currentRow();
            //DL - Get the adequate RtAudio index device

            for (unsigned int i = 0; i < rtaudio->getDeviceCount(); i++)
            {
                if (this->listSoundCard->item(this->listSoundCard->currentRow())->text() == QString(rtaudio->getDeviceInfo(i).name.c_str()) )
                {
                    this->projSoundcardIndex = i;
                    inputIsValid = true;
                    break;
                }
            }    
        }
    }

    // +---------------------------------------------------+
    // | Check if a file is selected                       |
    // +---------------------------------------------------+

    if (this->radioButtonInputFile->isChecked() == true)
    {

        QString fileOpenPath = QFileDialog::getOpenFileName(this->dialog, "Open raw audio file...", "", "Raw audio file (*.raw);;All files (*.*)");

        if (fileOpenPath != "")
        {
            this->projFromSoundCard = false;
            this->projFromFile = true;
            this->projInputFileName = fileOpenPath;

            inputIsValid = true;
        }

    }

    // +---------------------------------------------------+
    // | Select an output file if needed                   |
    // +---------------------------------------------------+

    if (inputIsValid == true)
    {

        if (this->checkBox->isChecked() == true)
        {

            QString fileSavePath = QFileDialog::getSaveFileName(this->dialog, "Save to a raw audio file...", "", "Raw audio file (*.raw);;All files (*.*)");

            if (fileSavePath != "")
            {
                this->projToFile = true;
                this->projOutputFileName = fileSavePath;

                outputIsValid = true;

            }

        }
        else
        {
            this->projToFile = false;

            outputIsValid = true;

        }

    }

    // +---------------------------------------------------+
    // | If input and output are good, then proceed        |
    // +---------------------------------------------------+

    if ((inputIsValid == true) && (outputIsValid == true))
    {
        this->projectionIsValid = true;
        this->dialog->close();
    }
    else
    {
        this->projectionIsValid = false;
    }

}

// +-------------------------------------------------------+
// | Cancel push button is pressed                         |
// +-------------------------------------------------------+

void InputOutputManager::cancelPushButtonClicked()
{

    this->projectionIsValid = false;
    this->dialog->close();

}

// +-------------------------------------------------------+
// | Sound card radio button is clicked                    |
// +-------------------------------------------------------+

void InputOutputManager::soundCardRadioButtonClicked()
{
    this->radioButtonInputFile->setChecked(false);
    this->radioButtonSoundCard->setChecked(true);
    this->listSoundCard->setEnabled(true);
    this->checkBox->setEnabled(true);
}

// +-------------------------------------------------------+
// | File input radio button is clicked                    |
// +-------------------------------------------------------+

void InputOutputManager::fileInputRadioButtonClicked()
{
    this->radioButtonInputFile->setChecked(true);
    this->radioButtonSoundCard->setChecked(false);
    this->listSoundCard->setEnabled(false);
    this->checkBox->setEnabled(false);
    this->checkBox->setChecked(false);
}

// +-------------------------------------------------------+
// | Item selection changed in the list                    |
// +-------------------------------------------------------+

void InputOutputManager::listItemSelectionChanged()
{

    if (this->listSoundCard->currentRow() != -1)
    {
        this->pushButtonOk->setEnabled(true);
    }
    else
    {
        this->pushButtonOk->setEnabled(false);
    }

}


// +-------------------------------------------------------+
// | List is double-clicked                                |
// +-------------------------------------------------------+

void InputOutputManager::listDoubleClicked(QListWidgetItem* _item)
{

    // Same as clicking ok
    okPushButtonClicked();

}


/***********************************************************
* File callback function                                   *
***********************************************************/

void InputOutputManager::fileCallBack()
{

    signed short inputShort;

    unsigned int indexBuffer;

    bool endHasBeenReached;

    indexBuffer = 0;
    endHasBeenReached = false;

    // Load the samples for a complete hop size
    for (int indexSample = 0; indexSample < this->getHopSize(); indexSample++)
    {
        for (int indexChannel = 0; indexChannel < this->getNumberChannels(); indexChannel++)
        {
            if (feof(this->inputFile)  == 0)
            {
                fread(&inputShort, sizeof(short), 1, this->inputFile);
            }
            else
            {
                inputShort = 0;
                endHasBeenReached = true;
            }
            this->inputFileBuffer[indexBuffer] = inputShort;
            indexBuffer++;
        }
    }

    // Then call the general callback if end has not been reached
    if (endHasBeenReached == false)
    {
        this->streamCallBack(this->inputFileBuffer);
    }
    else
    {
        this->endOfStream();
    }

}

/***********************************************************
* RTAudio callback function                                *
***********************************************************/

int rtAudioCallBack(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void* inputOutputManager)
{
    
    if (nBufferFrames != INPUT_HOPSIZE)
    {
        qDebug("Received  nBufferFrames %i outputBuffer=%p, inputBuffer=%p streamTime=%f",nBufferFrames,outputBuffer,inputBuffer,streamTime);
    }
    else
    {
        // Call the general callback
        InputOutputManager* inputOutputManagerPtr = reinterpret_cast<InputOutputManager*>(inputOutputManager);
        inputOutputManagerPtr->streamCallBack((signed short*) inputBuffer);
    }
    return 0;

}

/***********************************************************
* Constructor                                              *
***********************************************************/

EndOfStreamEvent::EndOfStreamEvent(): QEvent(QEvent::User)
{

}

EndOfStreamEvent::EndOfStreamEvent(const EndOfStreamEvent &e): QEvent(QEvent::User)
{
    (*this) = e;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

EndOfStreamEvent::~EndOfStreamEvent()
{

}

/***********************************************************
* Operators                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

EndOfStreamEvent& EndOfStreamEvent::operator=(const EndOfStreamEvent& _endOfStreamEvent)
{
	(*this) = _endOfStreamEvent;
	return *this;
}

