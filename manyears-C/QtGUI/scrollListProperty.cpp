#include "scrollListProperty.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

ScrollListProperty::ScrollListProperty(int _width, int _scrollBarWidth, int _height)
{
    // Save parameters
    this->width = _width;
    this->scrollBarWidth = _scrollBarWidth;
    this->height = _height;

    this->setGeometry(0,0,this->width,this->height);

    // Create widgets
    this->grid = new QGridLayout(this);
    this->grid->setMargin(0);
    this->grid->setSpacing(0);
    this->list = new ListProperty(this->width - this->scrollBarWidth,this->height);
    this->scrollBar = new QScrollBar(Qt::Vertical, this);

    // Adjuste the scroll bar
    this->adjustRange();

    // Add widgets
    this->grid->addWidget(this->list,0,0);
    this->grid->addWidget(this->scrollBar,0,1);

    QObject::connect(this->scrollBar, SIGNAL(valueChanged(int)), this->list, SLOT(scrollBarValueChanged(int)));
    QObject::connect(this->list, SIGNAL(isResized()), this, SLOT(resizeOccured()));

}

/***********************************************************
* Destructor                                               *
***********************************************************/

ScrollListProperty::~ScrollListProperty()
{

    delete this->grid;
    delete this->list;
    delete this->scrollBar;

}

/***********************************************************
* Slots                                                    *
***********************************************************/

// +-------------------------------------------------------+
// | Resize occured                                        |
// +-------------------------------------------------------+

void ScrollListProperty::resizeOccured()
{
    this->adjustRange();
}

/***********************************************************
* Events                                                   *
***********************************************************/

// +-------------------------------------------------------+
// | Resize occurs                                         |
// +-------------------------------------------------------+

void ScrollListProperty::resizeEvent(QResizeEvent *event)
{

    QSize newSize;

    newSize = event->size();

    // Save parameters
    this->width = newSize.width();
    this->height = newSize.height();

}

/***********************************************************
* Access the list                                          *
***********************************************************/

ListProperty* ScrollListProperty::getListProperty()
{
    return (this->list);
}

/***********************************************************
* Adjust range of the scroll bar                           *
***********************************************************/
void ScrollListProperty::adjustRange()
{

    if (this->list->getDisplayHeight() < this->height)
    {
        this->list->freezeScrollBarUpdate();
        this->scrollBar->setEnabled(false);
        this->scrollBar->setValue(-1);
        this->scrollBar->setMinimum(0);
        this->scrollBar->setMaximum(-1);
        this->list->resetDisplayTop();

    }
    else
    {
        this->list->unfreezeScrollBarUpdate();
        this->scrollBar->setEnabled(true);
        this->scrollBar->setMinimum(0);
        this->scrollBar->setMaximum(this->list->getDisplayHeight() - this->height);
        if ((this->scrollBar->value() < this->scrollBar->minimum()) || (this->scrollBar->value() > this->scrollBar->maximum()))
        {
            this->scrollBar->setValue(this->scrollBar->minimum());
        }
        this->scrollBar->setPageStep(this->list->getHeightElement());
    }

    this->update();

}
