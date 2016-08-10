#include "soundInputConfig.h"

#define LOADFROMFILE        "Load from a raw audio file..."

SoundInputConfig::SoundInputConfig(int _frameSize, int _hopSize, int _samplingRate, int _microphoneNumber)
{

    int dialogWidth, dialogHeight;
    int buttonLeft, buttonTop, buttonWidth, buttonHeight;
    int groupBox1Left, groupBox1Top, groupBox1Width, groupBox1Height;
    int groupBox2Left, groupBox2Top, groupBox2Width, groupBox2Height;
    int listWidgetLeft, listWidgetTop, listWidgetWidth, listWidgetHeight;
    int label1Left, label1Top, label1Width, label1Height;
    int label2Left, label2Top, label2Width, label2Height;
    int label3Left, label3Top, label3Width, label3Height;
    int label4Left, label4Top, label4Width, label4Height;
    int label5Left, label5Top, label5Width, label5Height;
    int label6Left, label6Top, label6Width, label6Height;
    int label7Left, label7Top, label7Width, label7Height;
    int label8Left, label8Top, label8Width, label8Height;

    int labelsTitleWidth, labelsTitleHeight;
    int labelsValueWidth, labelsValueHeight;

    int verticalMargin, horizontalMargin;
    int inBoxVerticalMargin, inBoxHorizontalMargin;

    QString label1Title;
    QString label2Title;
    QString label3Title;
    QString label4Title;
    QString label5Title;
    QString label6Title;
    QString label7Title;
    QString label8Title;

    QString groupBox1Title;
    QString groupBox2Title;

    // +-------------------------------------------------------+
    // | Save parameters                                       |
    // +-------------------------------------------------------+

    this->frameSize = _frameSize;
    this->hopSize = _hopSize;
    this->samplingRate = _samplingRate;
    this->microphoneNumber = _microphoneNumber;

    // +-------------------------------------------------------+
    // | Create audio object                                   |
    // +-------------------------------------------------------+

    this->audioInput = new AudioManager();

    // +-------------------------------------------------------+
    // | Margins                                               |
    // +-------------------------------------------------------+

    // Set the margins
    verticalMargin = 10;
    horizontalMargin = 10;
    inBoxVerticalMargin = 30;
    inBoxHorizontalMargin = 20;

    // +-------------------------------------------------------+
    // | Sizes                                                 |
    // +-------------------------------------------------------+

    // Size of the labels
    labelsTitleWidth = 150;
    labelsTitleHeight = 15;
    labelsValueWidth = 100;
    labelsValueHeight = 15;

    label1Width = labelsTitleWidth;
    label1Height = labelsTitleHeight;
    label2Width = labelsValueWidth;
    label2Height = labelsValueHeight;
    label3Width = labelsTitleWidth;
    label3Height = labelsTitleHeight;
    label4Width = labelsValueWidth;
    label4Height = labelsValueHeight;
    label5Width = labelsTitleWidth;
    label5Height = labelsTitleHeight;
    label6Width = labelsValueWidth;
    label6Height = labelsValueHeight;
    label7Width = labelsTitleWidth;
    label7Height = labelsTitleHeight;
    label8Width = labelsValueWidth;
    label8Height = labelsValueHeight;

    // Size of the list for sound cards
    listWidgetWidth = 390;
    listWidgetHeight = 160;

    // Size of the group box for sound cards
    groupBox2Width = inBoxHorizontalMargin + listWidgetWidth ;
    groupBox2Height = inBoxVerticalMargin + listWidgetHeight + verticalMargin;

    // Size of the group box for parameters
    groupBox1Width = groupBox2Width;
    groupBox1Height = inBoxVerticalMargin + label1Height + verticalMargin + label3Height + verticalMargin + label5Height + verticalMargin + label7Height;

    // Size of the button
    buttonWidth = groupBox2Width;
    buttonHeight = 30;

    // Size of dialog
    dialogWidth = horizontalMargin + groupBox1Width + horizontalMargin;
    dialogHeight = verticalMargin + groupBox1Height + verticalMargin + groupBox2Height + verticalMargin + buttonHeight + verticalMargin;

    // +-------------------------------------------------------+
    // | Positions                                             |
    // +-------------------------------------------------------+

    // Position of the group box for parameters
    groupBox1Left = horizontalMargin;
    groupBox1Top = verticalMargin;

    // Position of the group box for sound cards
    groupBox2Left = horizontalMargin;
    groupBox2Top = groupBox1Top + groupBox1Height + verticalMargin;

    // Position of the labels
    label1Left = inBoxHorizontalMargin;
    label2Left = label1Left + label1Width + horizontalMargin;
    label3Left = inBoxHorizontalMargin;
    label4Left = label3Left + label3Width + horizontalMargin;
    label5Left = inBoxHorizontalMargin;
    label6Left = label5Left + label5Width + horizontalMargin;
    label7Left = inBoxHorizontalMargin;
    label8Left = label7Left + label7Width + horizontalMargin;

    label1Top = inBoxVerticalMargin;
    label2Top = inBoxVerticalMargin;
    label3Top = label1Top + label1Height + verticalMargin;
    label4Top = label2Top + label2Height + verticalMargin;
    label5Top = label3Top + label3Height + verticalMargin;
    label6Top = label4Top + label4Height + verticalMargin;
    label7Top = label5Top + label5Height + verticalMargin;
    label8Top = label6Top + label6Height + verticalMargin;

    // Position of the list for sound cards
    listWidgetLeft = horizontalMargin;
    listWidgetTop = inBoxVerticalMargin;

    // Positin of the button
    buttonLeft = horizontalMargin;
    buttonTop = groupBox2Top + groupBox2Height + verticalMargin;

    // +-------------------------------------------------------+
    // | Titles                                                |
    // +-------------------------------------------------------+

    label1Title = "Samples per frame";
    label2Title = QString::number(this->frameSize);
    label3Title = "Samples per hop";
    label4Title = QString::number(this->hopSize);
    label5Title = "Sampling rate";
    label6Title = QString::number(this->samplingRate);
    label7Title = "Number of microphones";
    label8Title = QString::number(this->microphoneNumber);

    groupBox1Title = "Fixed parameters";
    groupBox2Title = "Select a recording device";


    // +-------------------------------------------------------+
    // | Instantiation                                         |
    // +-------------------------------------------------------+

    this->dialogWindow = new QDialog();
    this->dialogWindow->setFixedSize(dialogWidth,dialogHeight);

    this->groupBox1 = new QGroupBox(this->dialogWindow);
    this->groupBox1->setGeometry(QRect(groupBox1Left, groupBox1Top, groupBox1Width, groupBox1Height));
    this->groupBox1->setTitle(groupBox1Title);
    this->groupBox1->setFont(QFont("MS Shell Dlg 2",8,QFont::Bold,false));

    this->groupBox2 = new QGroupBox(this->dialogWindow);
    this->groupBox2->setGeometry(QRect(groupBox2Left, groupBox2Top, groupBox2Width, groupBox2Height));
    this->groupBox2->setTitle(groupBox2Title);
    this->groupBox2->setFont(QFont("MS Shell Dlg 2",8,QFont::Bold,false));

    this->buttonBox = new QDialogButtonBox(this->dialogWindow);
    this->buttonBox->setGeometry(QRect(buttonLeft, buttonTop, buttonWidth, buttonHeight));
    this->buttonBox->setOrientation(Qt::Horizontal);
    this->buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    this->frameSizeTitle = new QLabel(this->dialogWindow);
    this->frameSizeTitle->setGeometry(QRect(label1Left, label1Top, label1Width, label1Height));
    this->frameSizeTitle->setText(label1Title);
    this->frameSizeValue = new QLabel(this->dialogWindow);
    this->frameSizeValue->setGeometry(QRect(label2Left, label2Top, label2Width, label2Height));
    this->frameSizeValue->setText(label2Title);
    this->hopSizeTitle = new QLabel(this->dialogWindow);
    this->hopSizeTitle->setGeometry(QRect(label3Left, label3Top, label3Width, label3Height));
    this->hopSizeTitle->setText(label3Title);
    this->hopSizeValue = new QLabel(this->dialogWindow);
    this->hopSizeValue->setGeometry(QRect(label4Left, label4Top, label4Width, label4Height));
    this->hopSizeValue->setText(label4Title);
    this->samplingRateTitle = new QLabel(this->dialogWindow);
    this->samplingRateTitle->setGeometry(QRect(label5Left, label5Top, label5Width, label5Height));
    this->samplingRateTitle->setText(label5Title);
    this->samplingRateValue = new QLabel(this->dialogWindow);
    this->samplingRateValue->setGeometry(QRect(label6Left, label6Top, label6Width, label6Height));
    this->samplingRateValue->setText(label6Title);
    this->microphoneTitle = new QLabel(this->dialogWindow);
    this->microphoneTitle->setGeometry(QRect(label7Left, label7Top, label7Width, label7Height));
    this->microphoneTitle->setText(label7Title);
    this->microphoneValue = new QLabel(this->dialogWindow);
    this->microphoneValue->setGeometry(QRect(label8Left, label8Top, label8Width, label8Height));
    this->microphoneValue->setText(label8Title);


    this->listWidget = new QListWidget(this->groupBox2);
    this->listWidget->setGeometry(QRect(listWidgetLeft, listWidgetTop, listWidgetWidth, listWidgetHeight));

    // +-------------------------------------------------------+
    // | Populate list view with available recording device    |
    // +-------------------------------------------------------+

    this->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    for (unsigned int indexDevice = 0; indexDevice < this->audioInput->getAudioDevicesCount(); indexDevice++)
    {
        this->listWidget->addItem(this->audioInput->getAudioDeviceName(indexDevice));
        this->listWidget->setItemSelected(this->listWidget->item(indexDevice),false);
    }

    this->listWidget->addItem(LOADFROMFILE);

    this->itemChanged();

    this->deviceIndexTmp = -1;
    this->deviceIndex = -1;

    // +-------------------------------------------------------+
    // | Connect signals and slots                             |
    // +-------------------------------------------------------+

    QObject::connect(this->listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(itemChanged()));
    QObject::connect(this->buttonBox,SIGNAL(accepted()),this,SLOT(buttonAccepted()));
    QObject::connect(this->buttonBox,SIGNAL(rejected()),this,SLOT(buttonRejected()));

}

SoundInputConfig::~SoundInputConfig()
{

    delete this->audioInput;
    delete this->frameSizeTitle;
    delete this->frameSizeValue;
    delete this->hopSizeTitle;
    delete this->hopSizeValue;
    delete this->samplingRateTitle;
    delete this->samplingRateValue;
    delete this->microphoneTitle;
    delete this->microphoneValue;
    delete this->listWidget;
    delete this->buttonBox;
    delete this->groupBox1;
    delete this->groupBox2;
    delete this->dialogWindow;

}

bool SoundInputConfig::event(QEvent *event)
{

    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | Window needs to be shown                      |
        // +-----------------------------------------------+

        ShowEvent *showEvent = dynamic_cast<ShowEvent*>(event);

        if (showEvent != NULL)
        {

            this->dialogWindow->exec();
            this->itemChanged();

        }

        return true;

    }

    return QObject::event(event);

}

void SoundInputConfig::showDialog()
{
    this->dialogWindow->exec();
    this->itemChanged();
}

void SoundInputConfig::closeDialog()
{
    this->dialogWindow->hide();
}

void SoundInputConfig::itemChanged()
{

    if (this->listWidget->selectedItems().count()>0)
    {

        if (this->listWidget->selectedItems().at(0)->text() == LOADFROMFILE)
        {
            this->deviceIndexTmp = -2;
        }
        else
        {
            this->deviceIndexTmp = this->audioInput->getAudioDeviceIndex(this->listWidget->selectedItems().at(0)->text());
        }
    }
    else
    {
        this->deviceIndexTmp = -1;
    }


    if (this->deviceIndexTmp != -1)
    {
        this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }

}

void SoundInputConfig::buttonAccepted()
{
    this->deviceIndex = this->deviceIndexTmp;

    if (this->deviceIndex == -2)
    {

        QString filePath = QFileDialog::getOpenFileName(this->dialogWindow, "Open raw audio file...", "", "Raw audio file (*.raw);;All files (*.*)");

        if (filePath != "")
        {
            this->fileName = filePath;
            this->typeVisualization = SoundInputConfig::VIEW_ALL;
            this->closeDialog();
        }

    }

    else
    {
        this->closeDialog();
    }

}

void SoundInputConfig::buttonRejected()
{
    this->deviceIndex = -1;
    this->closeDialog();
}

int SoundInputConfig::getDeviceIndex()
{
    return (this->deviceIndex);
}

// +-------------------------------------------------------+
// | Get the name of the file selected                     |
// +-------------------------------------------------------+

QString SoundInputConfig::getFileName() const
{
    return (this->fileName);
}

// +-------------------------------------------------------+
// | Get the type of visualisation of the file selected    |
// +-------------------------------------------------------+

int SoundInputConfig::getTypeVisualization() const
{
    return (this->typeVisualization);
}
