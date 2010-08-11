#ifndef COMPENSATOR_H
#define COMPENSATOR_H

#include <math.h>

#define COMP_BUFFERSIZE 20

#define COMP_SPREAD1    0.1
#define COMP_SPREAD2    0.1

struct objCompensator
{

    float maximaAngle[COMP_BUFFERSIZE];
    float maximaWeight[COMP_BUFFERSIZE];
    int pointer;

    float gain;

};

void compensatorUpdate(struct objCompensator* myCompensator, float x, float y, float z, float E0);

void compensatorInit(struct objCompensator* myCompensator);

#endif
