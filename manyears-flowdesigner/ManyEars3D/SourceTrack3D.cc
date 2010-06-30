// Copyright (C) 2002-2005 Jean-Marc Valin/Universite de Sherbrooke
// All rights reserved

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include "SourceInfo.h"
//#include "sphere.h"
#include "particles3D.h"



using namespace FD;

class SourceTrack3D;

DECLARE_NODE(SourceTrack3D)
/*Node
 *
 * @name SourceTrack3D
 * @category ManyEars:3D
 * @description Track a sound source using a particle filter
 *
 * @input_name INPUT
 * @input_type Vector<ObjectRef>
 * @input_description Potential sources
 *
 * @output_name OUTPUT
 * @output_type Vector<ObjectRef>
 * @output_description Tracked sources (and their position)
 *
END*/

class SourceTrack3D : public BufferedNode {

   int inputID;
   int outputID;
   int nonCausality;
   
   Particle3DMixture tracker;
   ObjectRef sources;

public:
   SourceTrack3D(std::string nodeName, ParameterSet params)
   : BufferedNode(nodeName, params)
   , tracker(1000,0.05)
   {
      inputID = addInput("INPUT");
      outputID = addOutput("OUTPUT");
      
      inOrder = true;
      sources = Vector<ObjectRef>::alloc(0);
      if (parameters.exist("LOOKAHEAD"))
         nonCausality = dereference_cast<int> (parameters.get("LOOKAHEAD"));
      else
         nonCausality = 80;
      inputsCache[inputID].lookAhead = nonCausality;
      inputsCache[inputID].lookBack = nonCausality;
   }

   void calculate(int output_id, int count, Buffer &out)
   {
      RCPtr<Vector<ObjectRef> > inputValue = getInput(inputID, count+nonCausality);

      //cerr << count << endl;
      const Vector<ObjectRef> &in = *inputValue;

      if (!in.size())
      {
         out[count] = sources;
         return;
      }
      int N = in.size();
      
      float beampos[N][3];
      float beamprob[N];
      float beamstd[N];
      //sources = Vector<ObjectRef>::alloc(0);
      Vector<ObjectRef> &src = *Vector<ObjectRef>::alloc(0);
      sources = &src;
      
      for (int i=0;i<in.size();i++)
      {
         SourceInfo &peak = object_cast<SourceInfo>(in[i]);
         for (int j=0;j<3;j++)
            beampos[i][j] = peak.x[j];
         beamprob[i] = peak.strength;
         /*if (beamprob[i] > .4)
            beamstd[i] = .02;
         else
         beamstd[i] = .04;*/
         //beamstd[i] = .03 + .03*(1-beamprob[i]);
         beamstd[i] = .05;
      }
      tracker.predict();
      tracker.resample();
      tracker.update(beampos, beamprob, N, beamstd, count);

      for (int i=0;i<tracker.filters.size();i++)
      {
         float mean[3], std[3];
         if (!tracker.filters[i]->exists())
            continue;
         tracker.filters[i]->getStats (mean, std);
         //std::cout << count << " " << tracker.filters[i]->getID() << " " << mean[0] << " " << mean[1] << " " << mean[2] << " " << std[0] << " " << std[1] << " " << std[2] << " " << tracker.filters[i]->age << " " << tracker.filters[i]->age2 << " " << tracker.filters[i]->weight << std::endl;
         SourceInfo &s = *new SourceInfo;
         s.x[0] = mean[0];
         s.x[1] = mean[1];
         s.x[2] = mean[2];
         s.source_id = tracker.filters[i]->getID();
         s.strength = tracker.filters[i]->weight;
         s.start_time = count;
         s.eval_count = count + 300;
         s.remaining = 1;
	 s.age = tracker.filters[i]->age2;
         //s.remaining = 75;
         src.push_back(ObjectRef(&s));

      } 
#if 1 /* Controlling camera */
      int i;
      for (i=0;i<tracker.filters.size();i++)
      {
         if (tracker.filters[i]->exists())
            break;
      }
      if (i < tracker.filters.size())
      {
         float mean[3], std[3];
         tracker.filters[i]->getStats (mean, std);
         /*mean[2] += .6;
         mean[1] += 1.1;
         if (mean[2]>0)
            mean[2] = 0;
         if (mean[2] < -1.5)
            mean[2] = -1.5;*/
	 float range = sqrt(mean[0]*mean[0] + mean[1]*mean[1] + mean[2]*mean[2]);
	 float elev = -180/3.1416*atan2(mean[2],sqrt(mean[0]*mean[0] + mean[1]*mean[1]));
         std::cerr << "mean[1] = " << mean[1] << std::endl;
         //if (tracker.filters[i]->age2 < 13 && mean[1] > .5)
         if (tracker.filters[i]->age2 < 13 && range > .35 && elev < 60 )
         std::cout << 180/M_PI*atan2(mean[0],mean[1]) << " " 
                   << -180/3.1416*atan2(mean[2],sqrt(mean[0]*mean[0] + mean[1]*mean[1])) << " "
                   << max(0.,-.15+.15*sqrt(mean[0]*mean[0] + mean[1]*mean[1] + mean[2]*mean[2]))
                   << std::endl;
	 std::cerr << "range = " << range << std::endl;
         float err = sqrt(std[0]*std[0] + std[1]*std[1] + std[2]*std[2]);
         //std::cerr << "std = " << err << endl;
      }
#endif

#if 0 //Code for controling the robot with CAN
      int i;
      for (i=0;i<tracker.filters.size();i++)
      {
         if (tracker.filters[i]->exists())
            break;
      }
      if (i < tracker.filters.size())
      {
         float mean[3], std[3];
         tracker.filters[i]->getStats (mean, std);
         int age = tracker.filters[i]->age2;
         
         //SourceInfo &s2 = object_cast<SourceInfo> (src[0]);
         //cout << s2.source_id << " " << count << " " << 180/M_PI*atan2(s2.x[1],s2.x[0]) << " " << -180/M_PI*asin(s2.x[2]) << endl;
         float delta = 180/M_PI*atan2(mean[1],mean[0]);
         cerr << "angle = " << delta << endl;
         float command;
         float speed=0;
         if (delta > 2)
         {
            if (delta < 10)
               command = .1;
            else if (delta < 30)
               command = .2;
            else if (delta < 70)
               command = .35;
            else
               command = .65;
         }
         else if (delta < -2)
         {
            if (delta > -10)
               command = -.1;
            else if (delta > -30)
               command = -.2;
            else if (delta > -70)
               command = -.35;
            else
               command = -.65;
         } else {
            command = 0;
         }
         
         command = max(-.99,min(.99,.015*delta));
         if (age > 5)
            command *= .7;
         if (age > 10)
            command *= .3;
         if (age > 15)
            command = 0;
         
         if (fabs(delta)<4)
            speed=.23;
         else if (fabs(delta)<9)
            speed=.20;
         else if (fabs(delta)<12)
            speed=.18;
         else if (fabs(delta)<15)
            speed=.15;
         else if (fabs(delta)<22)
            speed=.05;
         speed *= 1.5;
         if (age > 10)
            speed *=.5;
         if (age > 20)
            speed =0;
         
         cout << speed << " " << command << endl;
         //cout << speed << " " << max(-.99,min(.99,.012*delta)) << endl;
         //cerr << "age = " << age << endl;
         //cout << "0 " << max(-.3,min(.3,.005*180/M_PI*atan2(s2.x[1],s2.x[0]))) << endl;
      } else {
         cout << "0 0" << endl;
      }
#endif

      out[count] = sources;
   }

   IN_ORDER_NODE_SPEEDUP(SourceTrack3D)

};
