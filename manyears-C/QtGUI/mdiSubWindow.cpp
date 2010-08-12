#include "mdiSubWindow.h"
#include <QCloseEvent>

MdiSubWindow::MdiSubWindow(QWidget * parent, Qt::WindowFlags flags): QMdiSubWindow(parent, flags)
{

}

MdiSubWindow::~MdiSubWindow()
{

}

bool MdiSubWindow::event(QEvent *event)
{

    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | Window needs to be shown                      |
        // +-----------------------------------------------+

        ShowEvent *showEvent = dynamic_cast<ShowEvent*>(event);

        if (showEvent != NULL)
        {

            this->show();

        }

        return true;

    }

    return QWidget::event(event);

}

void MdiSubWindow::closeEvent ( QCloseEvent * event )
{

    hide();
    event->ignore();

    emit this->windowIsHidden();

}
