#ifndef SOUNDINPUTCONFIG_H
#define SOUNDINPUTCONFIG_H

#include <QObject>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QFileDialog>

#include "audioManager.h"
#include "sourceBuffer.h"

class SoundInputConfig: public QObject
{
    Q_OBJECT

public:

    static const int VIEW_ALL = 1;
    static const int VIEW_FRAME = 2;

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    SoundInputConfig(int _frameSize, int _hopSize, int _samplingRate, int _microphoneNumber);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    virtual ~SoundInputConfig();

    /***********************************************************
    * Dialog management                                        *
    ***********************************************************/

    void showDialog();

    void closeDialog();

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Get the index of the selected device                  |
    // +-------------------------------------------------------+

    int getDeviceIndex();

    // +-------------------------------------------------------+
    // | Get the name of the file selected                     |
    // +-------------------------------------------------------+

    QString getFileName() const;

    // +-------------------------------------------------------+
    // | Get the type of visualisation of the file selected    |
    // +-------------------------------------------------------+

    int getTypeVisualization() const;

protected:

    virtual bool event(QEvent *event);

private:

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Recording device parameters                           |
    // +-------------------------------------------------------+

    int frameSize;
    int hopSize;
    int samplingRate;
    int microphoneNumber;

    // +-------------------------------------------------------+
    // | Selected device                                       |
    // +-------------------------------------------------------+

    int deviceIndex;
    int deviceIndexTmp;
    QString fileName;
    int typeVisualization;

    // +-------------------------------------------------------+
    // | QWidgets used for the user interface                  |
    // +-------------------------------------------------------+

    QDialog *dialogWindow;
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox1;
    QGroupBox *groupBox2;
    QListWidget *listWidget;
    QLabel *frameSizeTitle;
    QLabel *frameSizeValue;
    QLabel *hopSizeTitle;
    QLabel *hopSizeValue;
    QLabel *samplingRateTitle;
    QLabel *samplingRateValue;
    QLabel *microphoneTitle;
    QLabel *microphoneValue;

    // +-------------------------------------------------------+
    // | Audio Manager                                         |
    // +-------------------------------------------------------+

    AudioManager *audioInput;

private slots:

    /***********************************************************
    * Private slots                                            *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Item selection changed                                |
    // +-------------------------------------------------------+

    void itemChanged();

    // +-------------------------------------------------------+
    // | Button OK has been pressed                            |
    // +-------------------------------------------------------+

    void buttonAccepted();

    // +-------------------------------------------------------+
    // | Button cancel has been pressed                        |
    // +-------------------------------------------------------+

    void buttonRejected();

};

#endif // SOUNDINPUTCONFIG_H
