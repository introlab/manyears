/* Copyright (C) 2004-2005 Jean-Marc Valin

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


#include "particles.h"
#include <math.h>
#include <iostream>
#include <cstdlib>

using namespace std;

#define DELTA_T 0.04
#define TRANS_PROB .05f
#define SHORT_APRIORI .5f
#define GLOB_APRIORI .1f
#define EXIST_THRESH .98

#if 0
#define REF_PROB 4e-4f
#define REF_NEW_PROB 1.5e-5f
#define NEW_SOURCE_THRESH .3f
#else
#define REF_PROB 4.e-3f
#define REF_NEW_PROB 9e-5f
#define NEW_SOURCE_THRESH .3f
#endif
#define PROB_UNOBSERVED .2

/*inline float ran()
{
   return float(rand())/RAND_MAX;
}*/

inline float ran()
{
   const unsigned int jflone = 0x3f800000;
   const unsigned int jflmsk = 0x007fffff;
   static int idum = 10000;
   union {int i; float f;} ran;
   idum = 1664525*idum + 1013904223;
   ran.i = jflone | (jflmsk & idum);
   ran.f -= 1;
   return ran.f;
}

float eexp(float x)
{
   if (x<-60)
      return 8.75e-27;
   x *= 1.4427;
   union {unsigned int i; float f;} result;
   float xi = floor(x);
   int exponent = 128+(int)xi;
   x -= xi;
   result.f = 1.f + x*(1.f+.5f*x*(1.f+.333333f*x*(1.f+.25f*x)));
   result.i = (result.i&0x007fffff) | (exponent << 23);
   return result.f;
}

#define FLOGLOOKUP2SIZE 256
#define FLOGLOOKUP2SHIFT 15
float logtable2[FLOGLOOKUP2SIZE];
union FloatManip {
   float f;
   unsigned int i;
};
inline void build_flog_table()
{
   static bool init=false;
   if (!init)
   {
      FloatManip m;

      for (int i=0;i<FLOGLOOKUP2SIZE;i++)
      {
         m.i = (i<<FLOGLOOKUP2SHIFT) | 0x3f800000;
         logtable2[i]=log(m.f);
      }
      init=true;
   }
}
inline float flog(const float in)
{
   build_flog_table();
   FloatManip m;
   m.f = in;
      //Extract the mantissa and perform lookup for log(mantissa)
   float tb = logtable2[(m.i & 0x007fffff)>>FLOGLOOKUP2SHIFT];
      //Extract the exponent
   int id = (m.i>>23)-127;
      //log(mantissa*2^exponent) = exponent*log(2) + log(mantissa)
   return id*M_LN2 + tb;
}


inline float randn()
{
   static bool avail = false;
   static float cached=0.f;
   if (avail)
   {
      avail = false;
      return cached;
   } else {
      float U1, U2, S, x;
      do {
         U1 = float(rand())/float(RAND_MAX);
         U2 = float(rand())/float(RAND_MAX);
         U1 = 2*U1-1;
         U2 = 2*U2-1;
         S = U1*U1 + U2*U2;
      } while (S >= 1 || S == 0.0f);
      float tmp = sqrt(-2 * flog(S) / S);
      //float tmp = 1-S;
      cached = tmp * U1;
      avail = true;
      return tmp * U2;
   }
}


inline int find_range(float x, const vector<float> &cumul)
{
   int low=0;
   int high=cumul.size()-1;
   int cur = high>>1;
   while (1)
   {
      if (cumul[cur] > x)
      {
         if (cur == 0 || cumul[cur-1] <= x)
         {
            break;
         } else {
            high = cur-1;
            cur = (low+cur) >> 1;
         }
      } else {
         low = cur;
         cur = (high+cur+1) >> 1;
         if (cur==high)
            break;
      }
   }
   return cur;
}


Particle::Particle(float pos[3])
{
   //cerr << "called\n";
   x[0] = pos[0];
   x[1] = pos[1];
   x[2] = pos[2];
   v[0] = v[1] = v[2] = 0;
   state = 0;
   for (int i=0;i<DELAY;i++)
   {
      old_x[i][0] = x[0];
      old_x[i][1] = x[1];
      old_x[i][2] = x[2];
   }
   /*for (int i=0;i<DELAY;i++)
   {
      old_v[i][0] = v[0];
      old_v[i][1] = v[1];
      old_v[i][2] = v[2];
}*/
   old_index=0;
}

void Particle::predict()
{
   float betaX;
   float vX;
   const float deltaT = DELTA_T;
   float aX, bX;
   if (ran()<.03)
   {
      float x = ran();
      if (x<.2)
         state = Stopped;
      else if (x<.7)
         state = ConstVelocity;
      else
         state = Excitation;
   }
   //cerr << aX << " " << bX << endl;
   //state = Excitation;
   if (state==Excitation)
   {
      betaX = .5f;
      vX = .2;
      aX = .96;
      bX = 0.056000;
   } else if (state == ConstVelocity)
   {
      betaX = .05f;
      vX = .2;
      aX = .992;
      bX = .0505;
   } else {
      betaX = 2.f;
      vX = .04;
      aX = .923;
      bX = .015392;
   }
   aX = exp(-betaX*deltaT);
   bX = vX * sqrt(1-aX*aX);
   
   //aX = 1.f;
   
   v[0] = aX*v[0] + bX*randn();
   v[1] = aX*v[1] + bX*randn();
   v[2] = aX*v[2] + bX*randn();
   /*if (ran()<.1)
   v[0]=v[1]=v[2]=0;*/
   x[0] += deltaT*v[0];
   x[1] += deltaT*v[1];
   x[2] += deltaT*v[2];
   //float norm_1 = 1./sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
   float norm_1 = 1 - .5*((x[0]*x[0] + x[1]*x[1] + x[2]*x[2])-1);
   x[0] *= norm_1; x[1] *= norm_1; x[2] *= norm_1; 
   float cos_prod = x[0]*v[0] + x[1]*v[1] + x[2]*v[2];
   
   v[0] -= x[0]*cos_prod;
   v[1] -= x[1]*cos_prod;
   v[2] -= x[2]*cos_prod;
   
   old_x[old_index][0] = x[0];
   old_x[old_index][1] = x[1];
   old_x[old_index][2] = x[2];
   /*old_v[old_index][0] = v[0];
   old_v[old_index][1] = v[1];
   old_v[old_index][2] = v[2];*/
   old_index++;
   if (old_index >= DELAY)
      old_index = 0;
}

float Particle::prob(float pos[3], float std_1)
{
   float dist = (pos[0]-x[0])*(pos[0]-x[0]) + (pos[1]-x[1])*(pos[1]-x[1]) + (pos[2]-x[2])*(pos[2]-x[2]);
   dist = -std_1*std_1*dist;
   if (dist > -70)
      //return 1e-30+exp(.4*dist);
      //return 1e-30+.7f*exp(2*dist)+.3f*exp(.4*dist);
      //return 1e-30+.7f*exp(2*dist)+.29f*exp(.4*dist)+.01f*exp(.1*dist);
      return 1e-30+.8f*exp(4*dist)+.18f*exp(.4*dist)+.02f*exp(.1*dist);
      //return 1e-30+exp(-sqrt(-2*dist));
   else
      return 1e-30;
}

void Particle::updateWeight(float pos[3], float std_1)
{
   weight = prob(pos, std_1);
}





ParticleFilter::ParticleFilter(float pos[3], float std, int N, float initial_prob, int count)
   : particles(N, pos)
   , weight(initial_prob)
   , srcExists(false)
   , age(0)
   , age2(0)
   , aligned(0)
{
   for (int i=0;i<particles.size();i++)
   {
      particles[i].weight = 1./N;
      for (int j=0;j<3;j++)
         particles[i].x[j] += std*randn();
      float x = ran();
      if (x<.2)
         particles[i].state = Particle::Stopped;
      else if (x<.5)
         particles[i].state = Particle::ConstVelocity;
      else
         particles[i].state = Particle::Excitation;
      if (particles[i].state != Particle::Stopped)
      {
         particles[i].v[0] = .2*randn();
         particles[i].v[1] = .2*randn();
         particles[i].v[2] = .2*randn();
      }
      //particles[i].state = Particle::Stopped;
   }
   prob_exist = prob_exist2 = weight;
   short_term_weight = 1;
   minVar = std*std;
   minVarCount = count;
   lastSeenCount = count;
   startCount = count;
   for (int j=0;j<3;j++)
   {
      lastSeenPos[j] = pos[j];
      minVarPos[j] = pos[j];
   }
}

ParticleFilter::~ParticleFilter()
{
   if (exists())
   {
      cerr << "source " << ID << " " << startCount << " " << minVarCount << " " << lastSeenCount << " " << minVarPos[0] << " " << minVarPos[1] << " " << minVarPos[2] << endl;
      //cout << "source " << ID << " " << minVarCount << " " << 180/M_PI*atan2(minVarPos[1],minVarPos[0]) << " " << -180/M_PI*asin(minVarPos[2]) << endl;
   }
}

void ParticleFilter::predict()
{
   for (int i=0;i<particles.size();i++)
      particles[i].predict();
}

float ParticleFilter::prob(float pos[3], float std)
{
   float weightSum = 0;
   float std_1 = 1./std;
   for (int i=0;i<particles.size();i++)
   {
      weightSum += particles[i].prob(pos,std_1);
   }
   return weight * weightSum / particles.size();
}

void ParticleFilter::update(float pos[][3], float probs[], int N, float std[], int count)
{
   float sum_prob = 0;
   float std_1[N];
   for (int i=0;i<N;i++)
      std_1[i] = 1./std[i];
   vector<float> cumul_weight(particles.size(), 0.f);
   
   for (int i=0;i<N;i++)
      sum_prob += probs[i];
   
   {
      float p0, p1;
      float tmp;
      const float prob_floor = .03, apriori = GLOB_APRIORI;
      tmp = sum_prob;
      if (tmp < prob_floor)
         tmp = prob_floor;
      if (tmp > .99)
         tmp = .99;
      p1 = prob_exist * tmp/apriori;
      p0 = (1-prob_exist) * (1-tmp)/(1-apriori);
      //prob_exist = p1 / (p0+p1);
      if (prob_exist > prob_exist2) 
         prob_exist2 = prob_exist;
      //prob_exist = sum_prob + (1-sum_prob)*(prob_exist/(2.-prob_exist));
      prob_exist = sum_prob + (1-sum_prob)*(PROB_UNOBSERVED*prob_exist/(1-(1-PROB_UNOBSERVED)*prob_exist));
      /*if (weight > prob_exist)
      prob_exist = weight;*/
      if (prob_exist2 > EXIST_THRESH)
         srcExists = true;
      if (srcExists)
         prob_exist=1.;
      const float trans_prob = TRANS_PROB;
      const float apriori_short = SHORT_APRIORI;
      p0 = short_term_weight * trans_prob + (1-short_term_weight) * (1-trans_prob);
      p1 = short_term_weight * (1-trans_prob) + (1-short_term_weight) * trans_prob;
      tmp = .15+.85*sum_prob;
      p0 *= (1-tmp)/(1-apriori_short);
      p1 *= tmp/apriori_short;
      short_term_weight = p1/(p0+p1);
      weight = prob_exist * (short_term_weight * (1-trans_prob) + (1-short_term_weight) * trans_prob);
      //weight = prob_exist;
      /*if (aligned < 5)
      weight *= .5;*/
      //cerr << ID << " " << short_term_weight << " " << prob_exist << " " << weight << endl;
   }
   if (sum_prob>.1)
   //if (short_term_weight > .5)
   {
      aligned++;
      age=0;
   } else {
      age++;
   }
   if (sum_prob>.4)
   //if (short_term_weight > .5)
   {
      age2=0;
   } else {
      age2++;
   }

   //cerr << ID << " " << sum_prob << endl;
   for (int j=0;j<N;j++)
   {
      //Only update if it's worth it
      if (probs[j] > .00001)
         for (int i=0;i<particles.size();i++)
            cumul_weight[i] += probs[j]*particles[i].prob(pos[j], std_1[j]);
   }
   /*for (int i=0;i<particles.size();i++)
   particles[i].setWeight(cumul_weight[i]);*/
   float weightSum=1e-30;
   for (int i=0;i<particles.size();i++)
   {
      weightSum += cumul_weight[i];
   }
   float weightSum_1 = 1./weightSum;
   float prob_bias = (1.-sum_prob)/particles.size();
   for (int i=0;i<particles.size();i++)
   {
      cumul_weight[i] = weightSum_1*cumul_weight[i]*sum_prob + prob_bias;
   }

   for (int i=0;i<particles.size();i++)
      particles[i].weight *= cumul_weight[i];
   
   weightSum=1e-30;
   for (int i=0;i<particles.size();i++)
   {
      weightSum += particles[i].weight;
   }
   weightSum_1 = 1./weightSum;
   for (int i=0;i<particles.size();i++)
      particles[i].weight *= weightSum_1;

   updateStats(sum_prob>.3, count);
}

void ParticleFilter::resample()
{
   //FIXME: Make that cleaner
   /*if (age)
   return;*/
   float effectiveN = 0;
   for (int i=0;i<particles.size();i++)
      effectiveN += particles[i].weight * particles[i].weight;
   effectiveN = 1.f/(effectiveN*particles.size());
   //cerr << "effective " << effectiveN << endl;
   if (effectiveN > .6)
      return;
   vector<Particle> tmp = particles;
   vector<float> cumul(tmp.size());
   //FIXME: Can I really enable that (will the weights work OK?)
#if 1
   cumul[0] = tmp[0].weight;
   for (int i=1;i<particles.size();i++)
   {
      cumul[i] = cumul[i-1]+particles[i].weight;
   }
#else
   cumul[0] = pow(tmp[0].weight,.5f); 
   for (int i=1;i<particles.size();i++)
   {
      cumul[i] = cumul[i-1]+pow(particles[i].weight,.5f);
   }
   float norm = 1.f/cumul[particles.size()-1];
   for (int i=0;i<particles.size();i++)
      cumul[i] *= norm;
#endif 
   
   float size_1 = 1.f/tmp.size();
   float sum_weights = 0;
   
   for (int i=0;i<particles.size();i++)
   {
      float r = ran();
      int j;
      j=find_range(r,cumul);
      particles[i] = tmp[j];
      particles[i].weight = 1;//pow(particles[i].weight,.5f);
      sum_weights += particles[i].weight;
   }
   sum_weights = 1.f/sum_weights;
   
   for (int i=0;i<particles.size();i++)
   {
      particles[i].weight *= sum_weights;
   }
}

void ParticleFilter::print()
{
   for (int i=0;i<particles.size();i++)
      cout << particles[i].x[0] << " " << particles[i].x[1] << " " << particles[i].x[2] << " " << particles[i].v[0] << " " << particles[i].v[1] << " " << particles[i].v[2] << " " << particles[i].weight << endl;
}


void ParticleFilter::getStats(float mean[3], float std[3])
{
   for (int j=0;j<3;j++)
   {
      mean[j] = delayedPos[j];
      std[j] = 0;
   }
   return;
   float sum1[3]={0,0,0}, sum2[3]={0,0,0}, count=0;
   int const_speed=0;
   for (int i=0;i<particles.size();i++)
   {
      for (int j=0;j<3;j++)
      {
         //sum1[j] += particles[i].weight*particles[i].x[j];
         //sum2[j] += particles[i].weight*particles[i].x[j]*particles[i].x[j];
         float tmp = particles[i].old_x[particles[i].old_index][j];
         //float tmp = particles[i].old_v[particles[i].old_index][j];
         //float tmp = particles[i].v[j];
         sum1[j] += particles[i].weight*tmp;
         sum2[j] += particles[i].weight*tmp*tmp;
      }
      count += particles[i].weight;
      /*if (particles[i].state == 2)
      const_speed++;*/
   }
   //cerr << ID << " " << const_speed << endl;
   
   for (int j=0;j<3;j++)
   {
      mean[j] = sum1[j]/count;
      float tmp = (sum2[j]-sum1[j]*sum1[j]/count)/count;
      if (tmp > 0)
         std[j] = sqrt((sum2[j]-sum1[j]*sum1[j]/count)/count);
      else
         std[j] = 0;
   }
   //cerr << "weight " << weight << endl;
}


void ParticleFilter::updateStats(bool aligned, int count)
{
   float sum1[3]={0,0,0}, sum2[3]={0,0,0}, acc=0;
   float sumx1[3]={0,0,0}, sumx2[3]={0,0,0};
   int const_speed=0;
   for (int i=0;i<particles.size();i++)
   {
      for (int j=0;j<3;j++)
      {
         //sum1[j] += particles[i].weight*particles[i].x[j];
         //sum2[j] += particles[i].weight*particles[i].x[j]*particles[i].x[j];
         float tmp = particles[i].old_x[particles[i].old_index][j];
         float tmp2 = particles[i].x[j];
         //float tmp = particles[i].old_v[particles[i].old_index][j];
         //float tmp = particles[i].v[j];
         sum1[j] += particles[i].weight*tmp;
         sum2[j] += particles[i].weight*tmp*tmp;
         sumx1[j] += particles[i].weight*tmp2;
         sumx2[j] += particles[i].weight*tmp2*tmp2;
      }
      acc += particles[i].weight;
      /*if (particles[i].state == 2)
      const_speed++;*/
   }
   //cerr << ID << " " << const_speed << endl;
   
   float std1=0, std2=0;
   for (int j=0;j<3;j++)
   {
      delayedPos[j] = sum1[j]/acc;
      currPos[j] = sumx1[j]/acc;
      if (aligned)
      {
         lastSeenPos[j] = currPos[j];
         lastSeenCount = count;
      }
      float tmp = (sum2[j]-sum1[j]*sum1[j]/acc)/acc;
      float tmp2 = (sumx2[j]-sumx1[j]*sumx1[j]/acc)/acc;
      if (tmp > 0)
         std1 += ((sum2[j]-sum1[j]*sum1[j]/acc)/acc);
      if (tmp2 > 0)
         std2 += ((sumx2[j]-sumx1[j]*sumx1[j]/acc)/acc);
   }
   if (std2 < minVar)
   {
      minVar = std2;
      minVarCount = count;
      for (int j=0;j<3;j++)
         minVarPos[j] = currPos[j];
   }
   //cerr << "weight " << weight << endl;
}



ParticleMixture::ParticleMixture(int N, float std)
   : nextID(0)
   , nbParticles(N)
   , estimStd(std)
{
}

ParticleMixture::~ParticleMixture()
{
   for (int i=0;i<filters.size();i++)
      delete filters[i];
   filters.resize(0);
}

void ParticleMixture::resample()
{
   for (int i=0;i<filters.size();i++)
      filters[i]->resample();
}
   
void ParticleMixture::predict()
{
   for (int i=0;i<filters.size();i++)
      filters[i]->predict();
}

#define NEW_ALGO
void ParticleMixture::update(float pos[][3], float probs[], int N, float std[], int count)
{
   float inst_prob[filters.size()][N];
   for (int i=0;i<filters.size();i++)
   {
      for (int j=0;j<N;j++)
      {
         inst_prob[i][j] = filters[i]->prob(pos[j], std[j]);
         //cerr << inst_prob[i][j] << " ";
      }
   }
   //cerr << endl;
   float component_probs[filters.size()][N];
   float old_src_prob[N];
   float new_src_prob[N];
   const float ref_new_src_prob = REF_NEW_PROB;
   const float ref_prob = REF_PROB;
   for (int i=0;i<N;i++)
   {
      float src_prob = 0;
      float sum_prob = 1e-10;
      for (int j=0;j<filters.size();j++)
      {
         sum_prob += inst_prob[j][i];
         if (inst_prob[j][i] > src_prob)
            src_prob = inst_prob[j][i];
      }
      //src_prob = sum_prob;
      float post_prob = probs[i]/(probs[i]+ref_prob*(1-probs[i])/(src_prob+ref_new_src_prob));
      //cerr << post_prob << " ";
      old_src_prob[i] = post_prob * src_prob / (src_prob + ref_new_src_prob);
      new_src_prob[i] = post_prob * ref_new_src_prob / (src_prob + ref_new_src_prob);
      //cerr << old_src_prob[i] << " " << new_src_prob[i] << " ";
      for (int j=0;j<filters.size();j++)
      {
         //cerr << inst_prob[j][i] << " " << sum_prob << " ";
         component_probs[j][i] = old_src_prob[i] * inst_prob[j][i]/sum_prob;
         //cerr << component_probs[j][i] << " ";
      }
      //cerr << endl;
   }
   //cerr << endl;
   //cerr << "old:" << endl;
   //Update each particle filter using the probability of each source being part of that component
   for (int i=0;i<filters.size();i++)
   {
      float null_prob = 1;  //probability that no source corresponds to this component
      for (int j=0;j<N;j++)
      {
         null_prob *= 1-component_probs[i][j];
      }
      
      float sum_prob = 1e-10;
      for (int j=0;j<N;j++)
      {
         component_probs[i][j] = component_probs[i][j] / (1-component_probs[i][j]);
         sum_prob += component_probs[i][j];
      }
      for (int j=0;j<N;j++)
      {
         component_probs[i][j] = (1-null_prob) * component_probs[i][j]/sum_prob;
         //cerr << component_probs[i][j] << " ";
      }
      //cerr << endl;
      //Update component
      //cerr << (1-null_prob) << endl;
#ifndef NEW_ALGO
      filters[i]->update(pos, component_probs[i], N, std, count);
#endif
   }
   
   {
      float Pqj[N][filters.size()+2];
      int assignment[N];
      int level=0;
      int combinations=0;
      for (int i=0;i<N;i++)
         for (int j=0;j<filters.size()+2;j++)
            Pqj[i][j] = 0;
      for (int i=0;i<N;i++)
         assignment[i] = -3;
      //cerr << level << " " << assignment[0] << endl;
      while (1)
      {
         bool conflict = false;
         int size = filters.size();
         assignment[level]++;
         if (assignment[level] >= size)
         {
            if (level==0)
            {
               break;
            }
            assignment[level] = -3;
            level--;
            continue;
         }
         if (assignment[level] >= 0)
         {
            for (int i=0;i<level;i++)
            {
               if (assignment[level] == assignment[i] && assignment[i]>=0)
               {
                  conflict = true;
                  break;
               }
            }
         }
         if (!conflict)
         {
            if (level==N-1)
            {
               //We're at the leaf, update the table
               float prob = 1;
               combinations++;
               //cerr << "found one!" << endl;
               for (int i=0;i<N;i++)
               {
                  if (assignment[i]==-2)
                     prob *= (1-probs[i])*REF_PROB;
                  else if (assignment[i]==-1)
                     prob *= probs[i]*REF_NEW_PROB;
                  else  
                  {
                     prob *= probs[i]*inst_prob[assignment[i]][i];
                  }
               }
               for (int i=0;i<N;i++)
               {
                  if (assignment[i]==-2)
                     Pqj[i][filters.size()] += prob;
                  else if (assignment[i]==-1)
                     Pqj[i][filters.size()+1] += prob;
                  else
                     Pqj[i][assignment[i]] += prob;
               }
            } else {
               //Go one level up
               level++;
               //cerr << "+";
            }
         }
      }
      for (int i=0;i<N;i++)
      {
         float tot_prob = 0;
         for (int j=0;j<filters.size()+2;j++)
            tot_prob += Pqj[i][j];
         tot_prob = 1./tot_prob;
         for (int j=0;j<filters.size()+2;j++)
         {
            Pqj[i][j] *= tot_prob;
            if (j<filters.size())
               component_probs[j][i] = Pqj[i][j];
         }
      }
#ifdef NEW_ALGO
      for (int i=0;i<filters.size();i++)
      {
         filters[i]->update(pos, component_probs[i], N, std, count);
      }
      for (int i=0;i<N;i++)
         new_src_prob[i] = Pqj[i][filters.size()+1];
#endif
      //cerr << "new: combinations = " << combinations << endl;
      /*for (int i=0;i<filters.size();i++)
      {
         for (int j=0;j<N;j++)
            cerr << Pqj[j][i] << " ";
         cerr << endl;
   }*/
      
   }
   //cerr << endl;
   //Create new sources if needed
   for (int i=0;i<N;i++)
   {
      if (new_src_prob[i] > NEW_SOURCE_THRESH)
      {
         filters.push_back(new ParticleFilter (pos[i], estimStd, nbParticles, new_src_prob[i], count));
         filters[filters.size()-1]->ID = nextID++;
         //cerr << "adding " << new_src_prob[i] << endl;
        //Create new source
      }
   }
   
   //Remove unneeded mixtures
   for (int i=0;i<filters.size();i++)
   {
      bool remove = false;
      if (/*filters[i]->prob_exist < .01 || */(filters[i]->aligned == 0 && filters[i]->age > 4) || (filters[i]->aligned == 1 && filters[i]->age > 6) || (filters[i]->aligned < 4 && filters[i]->age > 8) || (filters[i]->aligned < 8 && filters[i]->age > 12) || (filters[i]->age2 > 30))
      //if (filters[i]->age + filters[i]->aligned > 15)
      {
         /*if (filters[i]->ID==4)
         continue;*/
         //cerr << filters[i]->age << endl;
         delete filters[i];
         for (int j=i;j<filters.size()-1;j++)
            filters[j] = filters[j+1];
         filters.resize(filters.size()-1);
         i--;
      }
   }
}
