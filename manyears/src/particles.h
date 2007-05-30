/* Copyright (C) 2004 Jean-Marc Valin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/


#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>
#include <map>
//#include "SourceInfo.h"

#define DELAY 1

class Particle {
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
   Particle() {}
   
   Particle(float pos[3]);
   
   void predict();
   
   float prob(float pos[3], float std_1);
   
   void updateWeight(float pos[3], float std_1);
   
   void setWeight(float w) {weight = w;}
   
   void accumWeight(float pos[3], float std_1, float w) {weight += w*prob(pos, std_1);}
   
   friend class ParticleFilter;
};

class ParticleFilter {
   public:
   std::vector<Particle> particles;
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
   ParticleFilter(float pos[3], float std, int N, float initial_prob, int count);
   ~ParticleFilter();
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
   friend class ParticleMixture;
};

class ParticleMixture {
   int nbParticles;
   float estimStd;
public:
   std::vector<ParticleFilter*> filters;
   int nextID;
public:
   ParticleMixture(int N, float std);
   ~ParticleMixture();
   void resample();
   void predict();
   void update(float pos[][3], float probs[], int N, float std[], int count);
};

#endif
