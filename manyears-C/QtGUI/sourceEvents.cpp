#include "sourceEvents.h"

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Algorithm Core Sources Objects                        x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential Source                                      %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

PotentialSource::PotentialSource(float _X, float _Y, float _Z, float _E, int _index)
{

    this->setX(_X);
    this->setY(_Y);
    this->setZ(_Z);
    this->setE(_E);
    this->setIndex(_index);

}

PotentialSource::PotentialSource(const PotentialSource& _potentialSource)
{

    this->setX(_potentialSource.getX());
    this->setY(_potentialSource.getY());
    this->setZ(_potentialSource.getZ());
    this->setE(_potentialSource.getE());
    this->setIndex(_potentialSource.getIndex());

}

PotentialSource::PotentialSource()
{

}

/***********************************************************
* Destructor                                               *
***********************************************************/

PotentialSource::~PotentialSource()
{

}

/***********************************************************
* Operators                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

PotentialSource& PotentialSource::operator=(const PotentialSource& _potentialSource)
{

    this->setX(_potentialSource.getX());
    this->setY(_potentialSource.getY());
    this->setZ(_potentialSource.getZ());
    this->setE(_potentialSource.getE());
    this->setIndex(_potentialSource.getIndex());

    return (*this);

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | X                                                     |
// +-------------------------------------------------------+

float PotentialSource::getX() const
{
    return this->X;
}

// +-------------------------------------------------------+
// | Y                                                     |
// +-------------------------------------------------------+

float PotentialSource::getY() const
{
    return this->Y;
}

// +-------------------------------------------------------+
// | Z                                                     |
// +-------------------------------------------------------+

float PotentialSource::getZ() const
{
    return this->Z;
}

// +-------------------------------------------------------+
// | E                                                     |
// +-------------------------------------------------------+

float PotentialSource::getE() const
{
    return this->E;
}

// +-------------------------------------------------------+
// | Index                                                 |
// +-------------------------------------------------------+

int PotentialSource::getIndex() const
{
    return this->index;
}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | X                                                     |
// +-------------------------------------------------------+

void PotentialSource::setX(float _X)
{
    this->X = _X;
}

// +-------------------------------------------------------+
// | Y                                                     |
// +-------------------------------------------------------+

void PotentialSource::setY(float _Y)
{
    this->Y = _Y;
}

// +-------------------------------------------------------+
// | Z                                                     |
// +-------------------------------------------------------+

void PotentialSource::setZ(float _Z)
{
    this->Z = _Z;
}

// +-------------------------------------------------------+
// | E                                                     |
// +-------------------------------------------------------+

void PotentialSource::setE(float _E)
{
    this->E = _E;
}

// +-------------------------------------------------------+
// | Index                                                 |
// +-------------------------------------------------------+

void PotentialSource::setIndex(int _index)
{
    this->index = _index;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Tracked Source                                            %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


/***********************************************************
* Constructor                                              *
***********************************************************/

TrackedSource::TrackedSource(float _X, float _Y, float _Z, int _ID)
{

    this->setX(_X);
    this->setY(_Y);
    this->setZ(_Z);
    this->setID(_ID);

}

TrackedSource::TrackedSource(const TrackedSource& _trackedSource)
{

    this->setX(_trackedSource.getX());
    this->setY(_trackedSource.getY());
    this->setZ(_trackedSource.getZ());
    this->setID(_trackedSource.getID());

}

TrackedSource::TrackedSource()
{

}

/***********************************************************
* Destructor                                               *
***********************************************************/

TrackedSource::~TrackedSource()
{

}

/***********************************************************
* Operators                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

TrackedSource& TrackedSource::operator=(const TrackedSource& _trackedSource)
{

    this->setX(_trackedSource.getX());
    this->setY(_trackedSource.getY());
    this->setZ(_trackedSource.getZ());
    this->setID(_trackedSource.getID());

    return (*this);

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | X                                                     |
// +-------------------------------------------------------+

float TrackedSource::getX() const
{
    return this->X;
}

// +-------------------------------------------------------+
// | Y                                                     |
// +-------------------------------------------------------+

float TrackedSource::getY() const
{
    return this->Y;
}

// +-------------------------------------------------------+
// | Z                                                     |
// +-------------------------------------------------------+

float TrackedSource::getZ() const
{
    return this->Z;
}

// +-------------------------------------------------------+
// | ID                                                    |
// +-------------------------------------------------------+

int TrackedSource::getID() const
{
    return this->ID;
}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | X                                                     |
// +-------------------------------------------------------+

void TrackedSource::setX(float _X)
{
    this->X = _X;
}

// +-------------------------------------------------------+
// | Y                                                     |
// +-------------------------------------------------------+

void TrackedSource::setY(float _Y)
{
    this->Y = _Y;
}

// +-------------------------------------------------------+
// | Z                                                     |
// +-------------------------------------------------------+

void TrackedSource::setZ(float _Z)
{
    this->Z = _Z;
}

// +-------------------------------------------------------+
// | ID                                                    |
// +-------------------------------------------------------+

void TrackedSource::setID(int _ID)
{
    this->ID = _ID;
}

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Algorithm Core Sources Events                         x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential source                                      %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

PotentialSourceEvent::PotentialSourceEvent(QList<PotentialSource> _list) : QEvent(QEvent::User)
{

    this->list = _list;

}

PotentialSourceEvent::PotentialSourceEvent() : QEvent(QEvent::User)
{

}

PotentialSourceEvent::PotentialSourceEvent(const PotentialSourceEvent &e): QEvent(QEvent::User)
{

    (*this) = e;

}

/***********************************************************
* Destructor                                               *
***********************************************************/

PotentialSourceEvent::~PotentialSourceEvent()
{

}


/***********************************************************
* Operators                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

PotentialSourceEvent& PotentialSourceEvent::operator=(const PotentialSourceEvent& _potentialSourceEvent)
{

    this->list.clear();

    for (int indexSource = 0; indexSource < _potentialSourceEvent.getListSize(); indexSource++)
    {

        this->list.push_back(_potentialSourceEvent.getPotentialSource(indexSource));

    }

    return *this;

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | PotentialSource                                       |
// +-------------------------------------------------------+

PotentialSource PotentialSourceEvent::getPotentialSource(const int indexSource) const
{

    return (this->list[indexSource]);

}

// +-------------------------------------------------------+
// | List                                                  |
// +-------------------------------------------------------+

QList<PotentialSource> PotentialSourceEvent::getList() const
{

    return (this->list);

}


// +-------------------------------------------------------+
// | Size                                                  |
// +-------------------------------------------------------+

int PotentialSourceEvent::getListSize() const
{

    return (this->list.size());

}

/***********************************************************
* Mutators                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | PotentialSource                                       |
// +-------------------------------------------------------+

void PotentialSourceEvent::pushSource(PotentialSource _source)
{

    this->list.push_back(_source);

}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Tracked source                                        %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

TrackedSourceEvent::TrackedSourceEvent(QList<TrackedSource> _list) : QEvent(QEvent::User)
{

    this->list = _list;

}

TrackedSourceEvent::TrackedSourceEvent() : QEvent(QEvent::User)
{

}

TrackedSourceEvent::TrackedSourceEvent(const TrackedSourceEvent &e): QEvent(QEvent::User)
{

    (*this) = e;

}

/***********************************************************
* Destructor                                               *
***********************************************************/

TrackedSourceEvent::~TrackedSourceEvent()
{

}


/***********************************************************
* Operators                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

TrackedSourceEvent& TrackedSourceEvent::operator=(const TrackedSourceEvent& _trackedSourceEvent)
{

    this->list.clear();

    for (int indexSource = 0; indexSource < _trackedSourceEvent.getListSize(); indexSource++)
    {

        this->list.push_back(_trackedSourceEvent.getTrackedSource(indexSource));

    }

    return *this;

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | PotentialSource                                       |
// +-------------------------------------------------------+

TrackedSource TrackedSourceEvent::getTrackedSource(const int indexSource) const
{

    return (this->list[indexSource]);

}

// +-------------------------------------------------------+
// | Size                                                  |
// +-------------------------------------------------------+

int TrackedSourceEvent::getListSize() const
{

    return (this->list.size());

}

/***********************************************************
* Mutators                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | PotentialSource                                       |
// +-------------------------------------------------------+

void TrackedSourceEvent::pushSource(TrackedSource _source)
{

    this->list.push_back(_source);

}




SeparatedSourceEvent::SeparatedSourceEvent(const QVector<short> &data)
    : QEvent(QEvent::User), m_data(data)
{

}


SeparatedSourceEvent::SeparatedSourceEvent()
    : QEvent(QEvent::User)
{
    //Empty data
}

SeparatedSourceEvent::SeparatedSourceEvent(const SeparatedSourceEvent &e)
    : QEvent(QEvent::User), m_data(e.m_data)
{

}

SeparatedSourceEvent::~SeparatedSourceEvent()
{
    //Nothing to do
}



SeparatedSourceEvent& SeparatedSourceEvent::operator=(const SeparatedSourceEvent& _separatedSourceEvent)
{
    m_data = _separatedSourceEvent.m_data;
}


