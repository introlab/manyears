// Copyright (C) 2002-2005 Jean-Marc Valin/Universite de Sherbrooke
// All rights reserved
#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>
#include <map>
//#include "SourceInfo.h"

#define DELAY 13

class Particle3D {
public:
   enum State {Stopped=0, ConstVelocity=1, Excitation=2};
private:
   float x[3];
   float v[3];
   //int id;
   float weight;
   int state;
   float old_x[DELAY][3];
   //float old_v[DELAY][3];
   int old_index;
public:
   Particle3D() {}
   
   Particle3D(float pos[3]);
   
   void predict();
   
   float prob(float pos[3], float std_1);
   
   void updateWeight(float pos[3], float std_1);
   
   void setWeight(float w) {weight = w;}
   
   void accumWeight(float pos[3], float std_1, float w) {weight += w*prob(pos, std_1);}
   
   friend class Particle3DFilter;
};

class Particle3DFilter {
   public:
   std::vector<Particle3D> particles;
   float weight;
   float prob_exist, prob_exist2;
   float short_term_weight;
   bool srcExists;
   //public:
   int ID;
   int age, age2, aligned;
   float currPos[3];
   float delayedPos[3];
   float lastSeenPos[3];
   int lastSeenCount;
   float minVarPos[3];
   float minVar;
   int minVarCount;
   int startCount;
public:
   Particle3DFilter(float pos[3], float std, int N, float initial_prob, int count);
   ~Particle3DFilter();
   void resample();
   void predict();
   float prob(float pos[3], float std);
   void update(float pos[][3], float probs[], int N, float std[], int count);
   void print();
   void updateStats(bool aligned, int count);
   void getStats(float mean[3], float std[3]);
   bool exists() {return srcExists;}
   int getID() {return ID;}
   //void update(std::vector<SourceInfo *> src);
   friend class Particle3DMixture;
};

class Particle3DMixture {
   int nbParticle3Ds;
   float estimStd;
public:
   std::vector<Particle3DFilter*> filters;
   int nextID;
public:
   Particle3DMixture(int N, float std);
   ~Particle3DMixture();
   void resample();
   void predict();
   void update(float pos[][3], float probs[], int N, float std[], int count);
};

#endif
