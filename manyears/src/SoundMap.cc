/* Copyright (C) 2003-2004 Jean-Marc Valin

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

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include "SourceInfo.h"
#include "sphere.h"

class SoundMap;

DECLARE_NODE(SoundMap)
/*Node
 *
 * @name SoundMap
 * @category ManyEars
 * @description Applies a gain to a vector
 *
 * @input_name INPUT
 * @input_type Vector<ObjectRef>
 * @input_description Potential sources
 *
 * @output_name OUTPUT
 * @output_type Vector<float>
 * @output_description Map of all sounds
 *
END*/

#define PROB_FLOOR .005f
#define PRIOR      .02f
#define PRIOR_1    50.0f

static const float prob_floor[3] = {.006,  .006,  .0015};
static const float prior[3]      = {.014,   .014,    .003};
static const float tt[2][3] = {{.008, .04, .02}, {.00008, .0004, .0002}};

static const float prior_1[3] = {1/prior[0], 1/prior[1], 1/prior[2]};

/*#define PROB_FLOOR .0005f
#define PRIOR      .001f
#define PRIOR_1    1000.0f
*/

class SoundMap : public BufferedNode {

   int inputID;
   int outputID;
   //float gain;

   //float hmm_prob[3][45][90];
   //float cur_prob[45][90];
   vector<float> hmm_prob[3];
   vector<float> old_hmm_prob[3];
   vector<float> cur_prob;
   vector<float> tot_prob;
public:
   SoundMap(string nodeName, ParameterSet params)
   : BufferedNode(nodeName, params)
   {
      inputID = addInput("INPUT");
      outputID = addOutput("OUTPUT");
      //gain = dereference_cast<float> (parameters.get("GAIN"));

      for (int m=0;m<3;m++)
      {
         old_hmm_prob[m].resize(sphere.size(), 0);
         hmm_prob[m].resize(sphere.size(), 0);
      }
      cur_prob.resize(sphere.size(), 0);
      tot_prob.resize(sphere.size(), 0);
      inOrder = true;
   }

   void calculate(int output_id, int count, Buffer &out)
   {
      RCPtr<Vector<ObjectRef> > inputValue = getInput(inputID, count);

      const Vector<ObjectRef> &in = *inputValue;

      for (int m=0;m<3;m++)
      {
         int count_updates=0;
         for (int i=0;i<cur_prob.size();i++)
               cur_prob[i]=prob_floor[m];
         //cerr << in.size() << endl;
         for (int i=0;i<in.size();i++)
         {
#if 1
            SourceInfo &src = object_cast<SourceInfo>(in[i]);
#else
            SourceInfo &src_tmp = object_cast<SourceInfo>(in[i]);
            static ObjectRef last_src[5][3];
            ObjectRef tmp_ptr=in[i];
            if (m==0 && src_tmp.interval==0)
            {
               //cerr << "a " << count << " " << ((count+1)/4)%5 << " " << i << " " << tmp << endl;
               if (count>40)
               {
                  //cerr << "b " << ((count+1)/4)%5 << " " << i << " " << last_src[((count+1)/4)%5][i] << endl;
                  tmp_ptr = last_src[((count+1)/4)%5][i];
               }
               last_src[((count+1)/4)%5][i] = in[i];
            }
            SourceInfo &src = object_cast<SourceInfo>(tmp_ptr);
#endif
            if (src.interval==m)
            {
               Point p;
               p.set(src.x[0], src.x[1], src.x[2]);

               int id = sphere(p);
               if (cur_prob[id]<src.strength)
                  cur_prob[id] = src.strength;
#if 0
               Triangle *tri = sphere.searchTriangle(p);
               if (cur_prob[tri->point_id[0]]<.1*src.strength)
                  cur_prob[tri->point_id[0]] = .1*src.strength;
               if (cur_prob[tri->point_id[1]]<.1*src.strength)
                  cur_prob[tri->point_id[1]] = .1*src.strength;
               if (cur_prob[tri->point_id[2]]<.1*src.strength)
                  cur_prob[tri->point_id[2]] = .1*src.strength;
#endif
               count_updates++;
            }
         }

         if (count_updates>0)
         {
#if 0
      if (m==0)
      {
         for (int j=0;j<360;j++)
         {
            float max_prob = 0;
            for (int i=0;i<180;i++)
            {
               float pr,p0,p1,tmp;
               float beta=.4;

               float theta = 360.0*j/360.0 - 180;
               float phi = 180.0*i/180.0 - 90;
               theta*= M_PI/180.0;
               phi*= M_PI/180.0;
               Point p;
               p.set(cos(-phi)*cos(-theta),cos(-phi)*sin(-theta),sin(-phi));
               int id = sphere(p);
               if (cur_prob[id] > max_prob)
                  max_prob = cur_prob[id];
               //cout << tot_prob[id] << " ";
            }
            //cout << endl;
            cout << max_prob << " ";
         }
         cout << endl;
      }
#endif

            
#if 1
            for (int i=0;i<sphere.size();i++)
            {
               float p0, p1;
               float old_prob = old_hmm_prob[m][i];
               if (cur_prob[i] > .2)
               for (int j=0;j<6;j++)
               {
                  if (sphere.neighbors[i][j]==-1)
                     break;
                  old_prob = max(old_prob, old_hmm_prob[m][sphere.neighbors[i][j]]);;
               }
               p1 = (1-tt[0][m]) * old_prob + tt[1][m] * (1-old_prob);

               p0 = 1-p1;
               p1 *= cur_prob[i]*prior_1[m];
               p0 = (1-cur_prob[i]);
               hmm_prob[m][i] = p1/(p0+p1);
               /*if (hmm_prob[m][i] < prob_floor[m])
                  hmm_prob[m][i] = prob_floor[m];*/
            }
#else
            for (int i=0;i<sphere.size();i++)
            {
               float p0, p1;
               p1 = (1-tt[0][m]-.03) * old_hmm_prob[m][i] +    tt[1][m]  * (1-old_hmm_prob[m][i]);
               for (int j=0;j<6;j++)
               {
                  if (sphere.neighbors[i][j]==-1)
                     break;
                  p1 += .005*old_hmm_prob[m][sphere.neighbors[i][j]];
               }

               p0 = 1-p1;
               p1 *= cur_prob[i]*prior_1[m];
               p0 = (1-cur_prob[i]);
               hmm_prob[m][i] = p1/(p0+p1);
               /*if (hmm_prob[m][i] < prob_floor[m])
                  hmm_prob[m][i] = prob_floor[m];*/
            }
#endif
            for (int i=0;i<sphere.size();i++)
            {
               old_hmm_prob[m][i] = hmm_prob[m][i];
            }
         }
      }

      if (in.size()>=2)
      {
         for (int i=0;i<sphere.size();i++)
         {
            float pr,p0,p1,tmp;
            float beta=.5;
#if 1
            pr=hmm_prob[0][i];
            /*
            if (pr<PROB_FLOOR)
               pr=PROB_FLOOR;
            tmp = hmm_prob[2][i];
            if (tmp<PROB_FLOOR)
               tmp=PROB_FLOOR;
            p0=pr*tmp;
            p1=(1-pr)*(1-tmp);
            pr = 1 / ( 1 + pow(p1/p0,.5f*(1+beta))*pow(PRIOR,beta) );
            */
            if (pr<PROB_FLOOR)
               pr=PROB_FLOOR;
            tmp = hmm_prob[1][i];
            if (tmp<PROB_FLOOR)
               tmp=PROB_FLOOR;
            p0=pr*tmp;
            p1=(1-pr)*(1-tmp);
            pr = 1 / ( 1 + pow(p1/p0,.5f*(1+beta))*pow(PRIOR,beta) );

            //pr = max(pr,max(hmm_prob[1][i], hmm_prob[0][i]));
            //pr = hmm_prob[1][i];
#else
            //pr = 1-pow((1-hmm_prob[0][i])*(1-hmm_prob[1][i])*(1-hmm_prob[2][i]),.4f);
            pr = hmm_prob[0][i];
            if (pr<hmm_prob[1][i])
               pr = hmm_prob[1][i];
            if (pr<hmm_prob[2][i])
               pr = hmm_prob[2][i];
            pr = hmm_prob[0][i];
#endif
            tot_prob[i] = pr;
         }
      }
#if 0
      if (in.size()>=2)
      {
         for (int i=0;i<40;i++)
         {
            for (int j=0;j<80;j++)
            {
               float pr,p0,p1,tmp;
               float beta=.4;

               float theta = 360.0*j/80.0 - 180;
               float phi = 180.0*i/40.0 - 90;
               theta*= M_PI/180.0;
               phi*= M_PI/180.0;
               Point p;
               p.set(cos(-phi)*cos(-theta),cos(-phi)*sin(-theta),sin(-phi));
               int id = sphere(p);

               cout << tot_prob[id] << " ";
            }
            cout << endl;
         }
      }
#endif
#if 0 //here
      if (in.size()>=2)
      {
         for (int j=0;j<80;j++)
         {
            float max_prob = 0;
            for (int i=0;i<40;i++)
            {
               float pr,p0,p1,tmp;
               float beta=.4;

               float theta = 360.0*j/80.0 - 180;
               float phi = 180.0*i/40.0 - 90;
               theta*= M_PI/180.0;
               phi*= M_PI/180.0;
               Point p;
               p.set(cos(-phi)*cos(-theta),cos(-phi)*sin(-theta),sin(-phi));
               int id = sphere(p);
#if 0
               if (hmm_prob[1][id] > max_prob)
                  max_prob = hmm_prob[1][id];
#else
               if (tot_prob[id] > max_prob)
                  max_prob = tot_prob[id];
#endif
               //cout << tot_prob[id] << " ";
            }
            //cout << endl;
            cout << max_prob << " ";
         }
         cout << endl;
      }
#endif
#if 0
      if (in.size()>=2)
      {
         for (int i=0;i<40;i++)
         {
            float max_prob = 0;
            for (int j=0;j<80;j++)
            {
               float pr,p0,p1,tmp;
               float beta=.4;

               float theta = 360.0*j/80.0 - 180;
               float phi = 180.0*i/40.0 - 90;
               theta*= M_PI/180.0;
               phi*= M_PI/180.0;
               Point p;
               p.set(cos(-phi)*cos(-theta),cos(-phi)*sin(-theta),sin(-phi));
               int id = sphere(p);
               if (tot_prob[id] > max_prob)
                  max_prob = tot_prob[id];
               //cout << tot_prob[id] << " ";
            }
            //cout << endl;
            cout << max_prob << " ";
         }
         cout << endl;
      }
#endif

#if 0
      for (int i=0;i<sphere.size();i++)
      {
         if (tot_prob[i]>.6)
         {
            const Point &p = sphere[i];
            cout << count << " " << p.x[0] << " " << p.x[1] << " " << p.x[2] << endl;
         }
      }
#endif
      Vector<float> &output = *Vector<float>::alloc(sphere.size());
      for (int i=0;i<sphere.size();i++)
      {
         output[i] = tot_prob[i];
      }
      out[count] = &output;
   }

   IN_ORDER_NODE_SPEEDUP(SoundMap)

};
