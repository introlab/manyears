#ifndef SOURCE_H
#define SOURCE_H

class Source
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    Source(int _sourceID, int _colorID, float _x, float _y, float _z);
    Source();

    /***********************************************************
    * Public fields                                            *
    ***********************************************************/

    int sourceID;
    int colorID;
    bool active;
    float x;
    float y;
    float z;

};

#endif // SOURCE_H
