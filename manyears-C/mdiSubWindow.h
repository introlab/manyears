#include <QMdiSubWindow>
#include "sourceBuffer.h"

class MdiSubWindow: public QMdiSubWindow
{

    Q_OBJECT

public:

    MdiSubWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);

    ~MdiSubWindow();

protected:

    virtual bool event(QEvent *event);
    virtual void closeEvent ( QCloseEvent * event );

signals:

    void windowIsHidden();

};
