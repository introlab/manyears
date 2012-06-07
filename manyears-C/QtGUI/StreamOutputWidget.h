#ifndef _STREAM_OUTPUT_WIDGET_
#define _STREAM_OUTPUT_WIDGET_

#include <QWidget>
#include <QTextEdit>
#include <QTimer>
#include <QMutex>
#include <QAudioOutput>
#include "ui_StreamOutputWidget.h"


class StreamOutputWidget : public QWidget
{
    Q_OBJECT

public:

    StreamOutputWidget(QWidget *parent=NULL);
    ~StreamOutputWidget();
    QVector<float> popFrame();
    void pushFrame(const QVector<float> &frame);

protected slots:

    void startButtonClicked();
    void stopButtonClicked();
    void clearButtonClicked();
    void refreshTimeout();
    void audioStateChanged(QAudio::State state );
    void notify();


protected:
    virtual bool event(QEvent* event);
    void scanOutputDevice();
    Ui::StreamOutputWidget m_ui;
    QList<QVector<float> > m_frames;
    QMutex m_mutex;
    QTimer *m_timer;
    QAudioOutput *m_audioOutput;
    QIODevice *m_IODevice;
};



#endif
