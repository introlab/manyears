/* Copyright (C) 2002-2005 Jean-Marc Valin

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
//#include "FFTWrap.h"
#include "misc.h"
#include "conj_window.h"
#include "SourceInfo.h"

#include "fftw3.h"
#include "sphere.h"
#include "Matrix.h"

#define fftw_malloc fftwf_malloc

using namespace FD;

class LocalizeBeam;

static float A[7][3] = {
   {-0.0033429,   -0.0022298,  0.0102052},
   {-0.0034587,  0.0083475,   -0.0038984},
   {-0.0061969,  0.0048726,  0.0068401},
   {0.0094233,   -0.0027946,   -0.0031785},
   {0.0062322,   -0.0048827,  0.0068463},
   {0.0059732,  0.0056614,   -0.0071567},
   {0.0033075,  0.0022399,  0.0035056}
};

float reg_deltas[7][28] = {
   {2, 1, 1, 1, 1, 1, 1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {1, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {1, 1, 2, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,-1,-1,-1,-1, 0, 0, 0, 0, 0, 0},
   {1, 1, 1, 2, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,-1,-1,-1, 0, 0, 0},
   {1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0,-1,-1, 0},
   {1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,-1},
   {1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1}
};

#if 1
#include <xmmintrin.h>
inline float approx_sqrt_1(float x)
{
   _mm_store_ss(&x, _mm_rsqrt_ss(_mm_set_ss(x)));
   return x;
}
inline float approx_sqrt(float x)
{
   _mm_store_ss(&x, _mm_sqrt_ss(_mm_set_ss(x)));
   return x;
}
#else
#define approx_sqrt_1(x) (1./sqrt(x))
#define approx_sqrt(x) (sqrt(x))
#endif

DECLARE_NODE(LocalizeBeam)
/*Node
 *
 * @name LocalizeBeam
 * @category ManyEars
 * @description Beamformer-based localization
 *
 * @input_name INPUT
 * @input_type Vector<float>
 * @input_description Multi-channel audio
 *
 * @input_name MIC_POS
 * @input_type Matrix<float>
 * @input_description Microphone position
 *
 * @output_name OUTPUT
 * @output_type Vector<ObjectRef>
 * @output_description Output vector (after gain)
 *
 * @output_name FRAMES
 * @output_type Vector<ObjectRef>
 * @output_description Channels in frequency domain
 *
 * @output_name NOISE
 * @output_type Vector<ObjectRef>
 * @output_description Noise spectrum for each channel
 *
 * @output_name ATTENTION
 * @output_type float
 * @output_description Where to pay attention
 *
 * @parameter_name NB_CHANNELS
 * @parameter_type int
 * @parameter_description Number of channels
 *
 * @parameter_name LENGTH
 * @parameter_type int
 * @parameter_description Size of the frames (per channel)
 *
 * @parameter_name SPEED_OF_SOUND
 * @parameter_type float
 * @parameter_description Speed of sound (m/s)
 *
 * @parameter_name SAMPLING_RATE
 * @parameter_type int
 * @parameter_description Sampling Rate (Hz)
 *
 * @parameter_name REVERB_LEVEL
 * @parameter_type float
 * @parameter_value 0.2
 * @parameter_description Level of instantaneous reverberation attenuation (between 0 and 1)
 *
 * @parameter_name REVERB_DECAY
 * @parameter_type float
 * @parameter_value 0.4
 * @parameter_description Reverberation decay (per frame)
 *
END*/

class PosInfo {
public:
   float t,p;
   int delta[28];
};

class LocalizeBeam : public BufferedNode {
   
   int inputID;
   int micInputID;
   int outputID;
   int framesID;
   int noiseID;
   int attentionID;
   int nb_channels;
   int length;
   int pslength;
   std::vector<std::vector <std::complex<float> > > x;
   std::vector<std::vector <std::complex<float> > > w;
   std::vector<std::vector <complex<float> > > autocor[3];
   std::vector<std::vector <float> > autocor2[3];
   std::vector<std::vector <float> > pow_average;
   std::vector<std::vector <float> > Stmp;
   std::vector<std::vector <float> > Smin;
   std::vector<std::vector <float> > noise;
   std::vector<std::vector <float> > reverb;
   std::vector<std::vector <float> > last_ps;
   std::vector<double> nfloor;
   std::vector<float> window;
   int N;
   float heading;
   float elevation;
   bool isSource;
   float adapt;
   FD::ObjectRef srcList;
   int samplingRate;
   float speedOfSound;
   float speedOfSound_1;

   float reverb_level;
   float reverb_decay;
   float reverb_coef;
   float reverb_coef2;
   float reverb_decay2;
   
   std::vector<PosInfo> dir;
   int count_up2;
   int valid_count;
   fftwf_plan plan_forward;
   fftwf_plan plan_backward;
   float *real_val;
   fftwf_complex *complex_val;
   bool isInit;
   FD::ObjectRef attention;
   float mic_pos2[8][3];

public:
   LocalizeBeam(string nodeName, ParameterSet params)
   : FD::BufferedNode(nodeName, params)
   {
      inputID = addInput("INPUT");
      micInputID = addInput("MIC_POS");
      outputID = addOutput("OUTPUT");
      framesID = addOutput("FRAMES");
      noiseID = addOutput("NOISE");
      attentionID = addOutput("ATTENTION");
      nb_channels = dereference_cast<int> (parameters.get("NB_CHANNELS"));
      length = dereference_cast<int> (parameters.get("LENGTH"));
      speedOfSound = dereference_cast<float> (parameters.get("SPEED_OF_SOUND"));
      samplingRate = dereference_cast<int> (parameters.get("SAMPLING_RATE"));

      valid_count=0;
      if (parameters.exist("REVERB_LEVEL"))
         reverb_level = dereference_cast<float> (parameters.get("REVERB_LEVEL"));
      else
         reverb_level = .2;
      if (parameters.exist("REVERB_DECAY"))
         reverb_decay = dereference_cast<float> (parameters.get("REVERB_DECAY"));
      else
         reverb_decay = .4;
      reverb_coef = (1-reverb_decay)*reverb_level;
      reverb_decay2 = reverb_decay/(1+reverb_coef);
      reverb_coef2 = reverb_coef/(1+reverb_coef);
      
      speedOfSound_1 = samplingRate/(100.*speedOfSound);
      //cerr << "sound " << speedOfSound_1 << endl;
      pslength = length/2+1;
      x.resize(nb_channels);
      w.resize(nb_channels);
      nfloor.resize(pslength);
      for (int i=0;i<3;i++)
      {
         autocor[i].resize(nb_channels*(nb_channels-1)/2);
         autocor2[i].resize(nb_channels*(nb_channels-1)/2);
      }
     
      isInit = false;
      window.resize(length);

      real_val = (float*)fftw_malloc(length*sizeof(float));
      complex_val = (fftwf_complex*)fftw_malloc(pslength*sizeof(fftwf_complex));

      plan_forward = fftwf_plan_dft_r2c_1d(length, real_val, complex_val, FFTW_PATIENT);
      plan_backward = fftwf_plan_dft_c2r_1d(length, complex_val, real_val, FFTW_PATIENT);

      /*for (int i=0;i<length;i++)
         window[i]=1;
      for (int i=0;i<length>>2;i++)
      window[i]=window[length-i-1]=.5-.5*cos(4*M_PI*i/length);*/
      conj_window(&window[0],length);
      //cout << window << endl;
      //exit(0);
      isSource=false;
      N=8;
      inOrder=1;
      int tmp=0;

      for (int i=0;i<nb_channels;i++)
      {
         x[i].resize(pslength);
         w[i].resize(pslength);
         for (int j=i+1;j<nb_channels;j++)
         {
            for (int k=0;k<3;k++)
            {
               autocor[k][tmp].resize(pslength,0);
               autocor2[k][tmp].resize(length,0);
            }
            tmp++;
         }
      }

      pow_average.resize(nb_channels);
      Stmp.resize(nb_channels);
      Smin.resize(nb_channels);
      noise.resize(nb_channels);
      reverb.resize(nb_channels);
      last_ps.resize(nb_channels);
      for (int i=0;i<nb_channels;i++)
      {
         pow_average[i].resize(pslength,0);
         Stmp[i].resize(pslength,0);
         Smin[i].resize(pslength,0);
         noise[i].resize(pslength,0);
         reverb[i].resize(pslength,0);
         last_ps[i].resize(pslength,0);
      }

      //cerr << "dir.size() = " << dir.size() << endl;
      //exit(0);
      count_up2=0;
   }


   void find_sources(int m, int count, int nb_peaks, int nframes)
   {
      std::vector<std::vector<float> > &xcorr = autocor2[m];
      //sourceOut[m]=&srcList;
      //srcList = nilObject;
      FD::Vector<FD::ObjectRef> &sources=object_cast<FD::Vector<FD::ObjectRef> > (srcList);
      float last_strength=0;
      //SourceInfo src;
      for (int peak=0;peak<nb_peaks;peak++)
      {
         float max_sum=-10;
         int max_ind=0;
         int Nmax = 30;
         float max_sum2[Nmax];
         int max_ind2[Nmax];
         bool corrected = false;
         float cor[28][256];
         float cor3[28][256];
         for (int i=0;i<28;i++)
         {
            float cor2[256];
            for (int j=0;j<128;j++)
               cor[i][j]=xcorr[i][length+j-128];
            for (int j=0;j<128;j++)
               cor[i][j+128]=xcorr[i][j];
            for (int j=1;j<256-1;j++)
               cor2[j]=max(max(cor[i][j],cor[i][j-1]),cor[i][j+1]);
            cor2[0]=cor[i][0];
            cor2[255]=cor[i][255];
#if 0
            for (int j=3;j<256-3;j++)
               cor3[i][j]=cor2[j];
            for (int j=1;j<256-1;j++)
               cor[i][j]=max(max(cor2[j],cor2[j-1]),cor2[j+1]);
#else
            for (int j=3;j<256-3;j++)
               cor3[i][j]=max(max(cor[i][j],.9f*cor[i][j-1]),.9f*cor[i][j+1]);
            for (int j=3;j<256-3;j++)
               cor[i][j]=max(max(cor2[j],cor2[j-3]),cor2[j+3]);
#endif
         }

         float accum1=0;
         /*if (peak==0)
         cerr << "loop " ;*/
         for (int i=0;i<Nmax;i++)
         {
            max_sum2[i] = -10;
            max_ind2[i] = 0;
         }
         for (int t=0;t<dir.size();t++)
         {
               int *ind=&(dir[t].delta[0]);
               float sum=0;

               for (int i=0;i<28;i++)
               {
                  sum += cor[i][128+ind[i]];
               }
               if (sum>max_sum)
               {
                  max_sum=sum;
                  max_ind=t;
               }
               if (peak==0)
               {
                  if (sum>max_sum2[0])
                  {
                     int i;
                     for (i=0;i<Nmax-1;i++)
                        if (sum<=max_sum2[i+1])
                           break;
                  //cerr << i << endl;
                     for (int j=0;j<i;j++)
                     {
                        max_sum2[j] = max_sum2[j+1];
                        max_ind2[j] = max_ind2[j+1];
                     }
                     max_sum2[i] = sum;
                     max_ind2[i] = t;
                  }
               }
         }
         /*for (int i=0;i<20;i++)
         cerr << max_sum2[i] << " ";*/
         /*if (peak==0)
         cerr << endl;*/
         if (peak==0 && max_ind2[Nmax-1] != max_ind)
            cerr << "oops!! " << max_sum << " " << max_sum2[Nmax-1] << " " << max_ind << " " << max_ind2[Nmax-1] << endl;
         if (peak==0)
         {
            for (int i=Nmax-2;i>=0;i--)
            {
               if (max_sum2[i] < (.85)*max_sum || max_sum2[i] < 200*nframes)
                  break;
               float at1 = atan2(sphere[max_ind].x[1], sphere[max_ind].x[0]);
               float at2 = atan2(sphere[max_ind2[i]].x[1], sphere[max_ind2[i]].x[0]);
               at1 = fabs(at1-at2);
               if (at1>180)
                  at1 = 360-at1;
               if (at1<.08 && fabs(asin(sphere[max_ind2[i]].x[2]))+.15 < fabs(asin(sphere[max_ind2[Nmax-1]].x[2])) && fabs(asin(sphere[max_ind2[i]].x[2]))+.35 < fabs(asin(sphere[max_ind].x[2])) && fabs(asin(sphere[max_ind2[i]].x[2])-asin(sphere[max_ind2[Nmax-1]].x[2])) > .2)
               {
/*                  if (max_sum>80*4)
                  {
                     cerr << "reverb swap" << endl;
                     cerr << sphere[max_ind2[i]].x[0] << " " << sphere[max_ind2[i]].x[1] << " " << sphere[max_ind2[i]].x[2] << endl;
                     cerr << sphere[max_ind].x[0] << " " << sphere[max_ind].x[1] << " " << sphere[max_ind].x[2] << endl;
                  }*/
                  max_ind2[Nmax-1] = max_ind2[i];
                  corrected = true;
               }
            }
            max_ind = max_ind2[Nmax-1];
         }
         max_sum /= nframes;
         /*if (m==0 && peak==0)
            cerr << "first search: " << max_sum << endl;*/
#if 0 /*SWITCH*/
         int deltas[28];
         int *ind=&(dir[max_ind].delta[0]);
         float sum_peaks=0;
         int nb_peaks=0;
         float sigma_sum=0;
         for (int i=0;i<28;i++)
         {
            float maxd=-10000;
            for (int kk=-6;kk<=6;kk++)
            {
               int ii=ind[i]+kk;
               if (ii<0)
                  ii += length;
               if (xcorr[i][ii]>maxd)
               {
                  maxd=xcorr[i][ii];
                  deltas[i]=ind[i]+kk;
               }
            }
            sum_peaks+=maxd;
            for (int kk=-6;kk<=6;kk++)
            {
               int ii=deltas[i]+kk;
               if (ii<0)
                  ii += length;
               xcorr[i][ii]=0;
            }
            if (peak==0 && m==1)
               cout << deltas[i] << " ";
         }
         if (peak==0 && m==1)
            cout << endl;
         
         SourceInfo &src = *new SourceInfo;
         //float delta[7];
         for (int i=0;i<7;i++)
         {
            src.delta[i]=0;
            for (int j=0;j<28;j++)
               src.delta[i]+= reg_deltas[i][j]*deltas[j];
            src.delta[i]*=.125;
            //cout << delta[i] << " ";
         }
         //cout << endl;
         //float pos[3];
         for (int i=0;i<3;i++)
         {
            src.x[i]=0;
            for (int j=0;j<7;j++)
               src.x[i]+= A[j][i]*src.delta[j];
         }
         float norm_1 = 1/sqrt(sqr(src.x[0])+sqr(src.x[1])+sqr(src.x[2]));
         src.x[0]*=norm_1;
         src.x[1]*=norm_1;
         src.x[2]*=norm_1;
#else
         const Point &p = sphere[max_ind];
         float distance = 0;
         float v0[3], vtheta[3], vphi[3];
         v0[0] = p.x[0];
         v0[1] = p.x[1];
         v0[2] = p.x[2];
         float r = sqrt(v0[0]*v0[0] + v0[1]*v0[1]);
         float ratio;
         if (r!=0)
            ratio = -v0[2]/r;
         else
            ratio = sqrt(.5);
         vphi[0] = ratio*v0[0];
         vphi[1] = ratio*v0[1];
         vphi[2] = r;
         vtheta[0] = v0[1]*vphi[2] - v0[2]*vphi[1];
         vtheta[1] = v0[2]*vphi[0] - v0[0]*vphi[2];
         vtheta[2] = v0[0]*vphi[1] - v0[1]*vphi[0];
         float sum_peaks = -1000000;
         int flush_ids[28];
         SourceInfo &src = *new SourceInfo;
         /*cerr << v0[0] << " " << v0[1] << " " << v0[2] << endl;
         cerr << vphi[0] << " " << vphi[1] << " " << vphi[2] << endl;
         cerr << vtheta[0] << " " << vtheta[1] << " " << vtheta[2] << endl;
         cerr << endl;*/
         float stepSize;
         int Nsteps;
         if (peak==0)
         {
            Nsteps = 4;
            stepSize = .02;
         } else {
            Nsteps = 2;
            stepSize = .035;
         }
         for (int dtheta = -Nsteps;dtheta<=Nsteps;dtheta++)
         {
            for (int dphi = -Nsteps;dphi<=Nsteps;dphi++)
            {
               float pos[3];
               pos[0] = v0[0] + stepSize * (dtheta*vtheta[0] + dphi*vphi[0]);
               pos[1] = v0[1] + stepSize * (dtheta*vtheta[1] + dphi*vphi[1]);
               pos[2] = v0[2] + stepSize * (dtheta*vtheta[2] + dphi*vphi[2]);
               for (float dist = 50;dist<=500;dist*=1.5)
               {
                  float sum=0;
                  float fine_delta[nb_channels];
                  for (int i=0;i<nb_channels;i++)
                  {
                     fine_delta[i]=speedOfSound_1*sqrt(sqr(dist*pos[0]-mic_pos2[i][0]) +
                                                       sqr(dist*pos[1]-mic_pos2[i][1]) +
                                                       sqr(dist*pos[2]-mic_pos2[i][2]) );
                  }
                  int pair_id=0;
                  for (int i=0;i<nb_channels;i++)
                  {
                     for (int j=i+1;j<nb_channels;j++)
                     {
                        /*if (dist>=150 && dist < 200 && dphi == 0 && dtheta==0)
                           cerr << (dir[max_ind].delta[pair_id]) << " " << fine_delta[i]-fine_delta[j] << "    ";*/
                        sum += cor3[pair_id][128+int(floor(.5+fine_delta[i]-fine_delta[j]))];
                        pair_id++;
                     }
                  }
                  //cerr << endl;
                  if (sum>sum_peaks)
                  {
                     sum_peaks = sum;
                     
                     float norm_1 = 1.0f/sqrt(sqr(pos[0])+sqr(pos[1])+sqr(pos[2]));
                     src.x[0]=pos[0]*norm_1;
                     src.x[1]=pos[1]*norm_1;
                     src.x[2]=pos[2]*norm_1;
                     distance = dist;
                     int pair_id=0;
                     for (int i=0;i<nb_channels;i++)
                     {
                        for (int j=i+1;j<nb_channels;j++)
                        {
                           flush_ids[pair_id] = int(floor(.5+fine_delta[i]-fine_delta[j]));
                           pair_id++;
                        }
                     }
                  }
               }
            }
         }
         
         //Flush correlations corresponding to the sources
         for (int i=0;i<28;i++)
         {
            //cerr << (dir[max_ind].delta[i]) << " " << flush_ids[i] << "    ";
            for (int kk=-4;kk<=4;kk++)
            {
               int ii=flush_ids[i]+kk;
               if (ii<0)
                  ii += length;
               xcorr[i][ii]=0;
            }
         }
#endif
         /*if (m==0 && peak==0)
            cerr << sum_peaks << " " << src.x[0] << " " << src.x[1] << " " << src.x[2] << " " << distance << endl;*/

#if 1
         sum_peaks /= nframes;
         /*if (sum_peaks>mean_peak)
            src.strength = 1-exp(-.01*(1+m)*(1+peak)*(sum_peaks-mean_peak));
         else
            src.strength = min(.3,1-exp(-.003*(1+peak)*(sum_peaks-.8*mean_peak)));*/
         //src.strength = max(min(.4*peak,1-0*exp(-.003*(sum_peaks-.6*mean_peak))),   1-exp(-.01*(1+m)*(-1+peak)*(sum_peaks-mean_peak)));
         /*if (src.strength>.98)
            src.strength = .98;*/
         /*if (src.strength < .1 && last_strength>.15)
            src.strength = .1;*/
         if (m==0)
         {
            if (peak==0)
            {
               //src.strength = 1-exp(-.01*(sum_peaks-60));
#if 1
		//INCREASE FOR LESS SENSIBILITY 150=lab 220 = AAAI
               float xn = sum_peaks/200.;
               /*if (corrected)
                  xn *= .7;*/
               //xn *= sqrt(1-src.x[2]*src.x[2]);
               /*if (fabs(src.x[2])>.5)
                  xn = 0;*/
               
               if (xn<1)
                  src.strength = .5f*xn*xn;
               else
                  src.strength = 1 - .5f/(xn*xn);
#else
               /*float xn = sum_peaks/80.;
               if (xn < 1)
                  src.strength = .333*xn*xn;
               else
                  src.strength = 1 - .667/xn;*/
               float xn = sum_peaks/80.;
               if (xn < 1)
                  src.strength = .428*xn*xn;
               else
                  src.strength = 1 - .572/pow(xn,1.5f);
#endif
               /*if (xn > 5) 
                  src.strength = 1;*/
                 /*if (xn < 1)
                    src.strength = 0;*/
            } else {
               //src.strength = min(.8f*last_strength, .2);
               if (peak==1)
                  src.strength = min(.4f*last_strength, .3f);
               else if (peak==2)
                  src.strength = min(.6f*last_strength, .16f);
               else if (peak==3)
                  src.strength = min(.3f*last_strength, .03f);
               else
                  src.strength = .01;
            }
            /*if (peak==1 && last_strength>.05)
               src.strength = .15;
            else if (last_strength>.05)
               src.strength = .1;
            else
            src.strength = 0;*/
            /*if (peak==0)
            cout << "peak " << sum_peaks << endl;*/
            
            //***********
            //cout << "source " << count << " " << max(0.0f,src.strength) << " " << src.x[0] << " " << src.x[1] << " " << src.x[2] << endl;
            
            /*if (peak==0)
               cout << 0 << " " << sum_peaks << endl;*/
            /*if (src.strength > .9)
               cerr << "instant " << src.x[0] << " " << src.x[1] << " " << src.x[2] << endl;*/
            /*if (src.strength > .9)
               cerr << count << " " << 180/M_PI*atan2(src.x[1],src.x[0]) << " " << -180/M_PI*asin(src.x[2]) << endl;*/
         } else if (m==1)
         {
            if (peak==0)
               src.strength = 1-exp(-.01*(sum_peaks-60));
            else if (peak==1 && last_strength>.05)
               src.strength = .2;
            else if (last_strength>.05)
               src.strength = .1;
            else
               src.strength = 0;
            /*if (peak==0)
               cout << 1 << " " << sum_peaks << endl;*/
         } else
            src.strength = 0;
            
         if (src.strength<0)
            src.strength = 0;
         last_strength = src.strength;
         if (fabs(src.x[2])>.9)
            src.strength = 0;
         /*if (src.x[2] > 0)
            src.strength = 0;*/
#else
         /* KLUDGE: prevent low sources from being detected */
         if (src.x[2]>.85)
            src.strength = 0;
         else
         if (m==0)
         {
            //src.strength = .90-.85*exp(-(.000125*(sum_peaks-6000)));
            if (peak==0)
               src.strength = 1-exp(-(.00025*(sum_peaks-4000)));
            else 
               src.strength = 1-exp(-(.000166*(sum_peaks-6000)));
            if (src.strength>.98)
               src.strength = .98;
            if (src.strength < .15)
               src.strength = .15;
         } else if (m==1)
         {
            //src.strength = .95-.85*exp(-(.000025*(sum_peaks-22000)));
            if (peak==0)
               src.strength = 1-exp(-(.00005*(sum_peaks-20000)));
            else
               src.strength = 1-exp(-(.0000333*(sum_peaks-30000)));
            if (src.strength>.98)
               src.strength = .98;
            //src.strength = .75*(1-exp(-(sum_peaks-22000)/5000));
            if (src.strength < .15)
               src.strength = .15;
            /*src.strength += .35*exp(-(1.0)*peak);
            if (src.strength>.99)
            src.strength = .99;*/
            /*if (src.strength>.3)
              cout << count << " " << src.x[0] << " " << src.x[1] << " " << src.x[2] << " " << sum_peaks << endl;*/
         } else {
            //src.strength = .99-.89*exp(-(.000008*(sum_peaks-125000)));
            src.strength = .6*exp(-(1.4-.4*m)*peak);
         }
#endif
         //if (src.strength>.6 && (m==0 || m==1))
         //cout << count << " " << src.x[0] << " " << src.x[1] << " " << src.x[2] << " " << sum_peaks << endl;
         src.interval = m;
         //src.strength = sum_peaks;
         src.eval_count = count;
         sources.push_back(FD::ObjectRef(&src));

         if (peak == 0 && attention->isNil())
         {
            if ((m==0 && sum_peaks> 15000) || (m==1 && sum_peaks > 35000))
            {
               //cout << src.x[0] << " " << src.x[1] << " " << src.x[2] << endl;
               Vector<float> *vec = Vector<float>::alloc(2);
               (*vec)[0] = -180/M_PI*atan2(src.x[1],src.x[0])+180;
               (*vec)[1] = -180/M_PI*asin(src.x[2])*10;
               attention=vec;
            }
         }

         if (m==3 && peak == 0)
         {
            cout << sum_peaks << " " << 1 << " ";// << nb_peaks << " " << 2.5*sigma_sum << " ";
            cout << -180/M_PI*atan2(src.x[1],src.x[0]) << " " << -180/M_PI*asin(src.x[2]) << endl;
            //cout << pos[0] << " " << pos[1] << " " << pos[2] << endl;
         }
      }



   }

   void update_corr(int m)
   {
      DYN_VEC(float, length, tmp2);
      int pair_id=0;
      for (int i=0;i<nb_channels;i++)
      {
         /*cout << i << ": ";*/
         for (int j=i+1;j<nb_channels;j++)
         {
            //FFTWrap.irfft(&autocor[m][pair_id][0], tmp2, length);
            //for (int k=0;k<length;k++)
            //   autocor2[m][pair_id][k]=tmp2[k];

            for(int k=0;k<pslength;k++)
            {
               complex_val[k][0] = autocor[m][pair_id][k].real();
               complex_val[k][1] = autocor[m][pair_id][k].imag();
            }
            fftwf_execute(plan_backward);
            for (int k=0;k<length;k++)
               autocor2[m][pair_id][k]=real_val[k];

            pair_id++;
         }
      }

   }

   void calculate(int output_id, int count, Buffer &out)
   {
      if (!isInit)
      {
         isInit = true;
         FD::ObjectRef micValue = getInput(micInputID, count);

         const FD::Matrix<float> &mat = object_cast<FD::Matrix<float> > (micValue);
         float pos_mean[3] = {0,0,0};
         for (int i=0;i<8;i++)
         {
            pos_mean[0] += mat[i][0];
            pos_mean[1] += mat[i][1];
            pos_mean[2] += mat[i][2];
         }
         pos_mean[0] *= .125;
         pos_mean[1] *= .125;
         pos_mean[2] *= .125;
         for (int i=0;i<8;i++)
         {
            mic_pos2[i][0]=mat[i][0]-pos_mean[0];
            mic_pos2[i][1]=mat[i][1]-pos_mean[1];
            mic_pos2[i][2]=mat[i][2]-pos_mean[2];
            //cerr << mic_pos2[i][0] << " " << mic_pos2[i][1] << " " << mic_pos2[i][2] <<endl;
         }
        
         int id=0;
         float dist = 150;

         dir.resize(sphere.size());
         for (int k=0;k<sphere.size();k++)
         {
            const Point &p = sphere[k];
            float pos[3];
            pos[0] = dist*p.x[0];
            pos[1] = dist*p.x[1];
            pos[2] = dist*p.x[2];
            float r[8];
            for (int i=0;i<nb_channels;i++)
            {
               r[i]=speedOfSound_1*sqrt(sqr(pos[0]-mic_pos2[i][0])+sqr(pos[1]-mic_pos2[i][1])+sqr(pos[2]-mic_pos2[i][2]));
               //cout << r[i] << " ";
            }
            //cout << endl;
            id=0;
            for (int i=0;i<nb_channels;i++)
            {
               for (int j=i+1;j<nb_channels;j++)
               {
                  dir[k].delta[id]=int(floor(.5+r[i]-r[j]));
                  id++;
               }
            }
         }

      }


      FD::Vector<FD::ObjectRef> &fft_frames = *FD::Vector<FD::ObjectRef>::alloc(nb_channels);
      (*(outputs[framesID].buffer))[count]=&fft_frames;
      FD::Vector<FD::ObjectRef> &noise_frames = *FD::Vector<FD::ObjectRef>::alloc(nb_channels);
      (*(outputs[noiseID].buffer))[count]=&noise_frames;

      FD::ObjectRef inputValue = getInput(inputID, count);

      srcList = new FD::Vector<FD::ObjectRef>;

      const FD::Vector<float> &in = object_cast<FD::Vector<float> > (inputValue);
      int length2 = in.size()/nb_channels;
      if (length2!=length)
         throw new FD::NodeException(this, "length mismatch", __FILE__, __LINE__);
      DYN_VEC(float, length, tmp);
      DYN_VEC(float, length, tmp2);
      for (int j=0;j<pslength;j++)
         tmp2[j]=0;

      attention = nilObject;
      {
         float e1=0, e2=0;
         for (int i=0;i<(length<<2);i++)
         {
            e1 += in[i]*in[i];
            e2 += in[i+(length<<2)]*in[i+(length<<2)];
         }
         if (e1 < 10 || e2 < 10)
         {
            cerr << "LocalizeBeam: got some zeros" << endl;
            for (int i=0;i<8;i++)
            {
               Vector<complex<float> > &fr = *Vector<complex<float> >::alloc(pslength);
               for (int j=0;j<pslength;j++)
                  fr[j] = 1;
               fft_frames[i] = &fr;
               FD::Vector<float> &noise_fr = *FD::Vector<float>::alloc(pslength);
               for (int j=0;j<pslength;j++)
                  noise_fr[j] = noise[i][j];
               noise_frames[i] = &noise_fr;
            }
            (*(outputs[outputID].buffer))[count]=srcList;
            (*(outputs[attentionID].buffer))[count]=attention;
            return;
         }
      }

      //Perform FFT's
      for (int i=0;i<nb_channels;i++)
      {
         int ind=0;

         for (int j=0,ind=i; j<length; j++,ind += nb_channels)
            real_val[j]=in[ind];
         for (int j=0;j<length;j++)
         {
            real_val[j]*=window[j];
         }
         fftwf_execute(plan_forward);
         for (int j=1;j<pslength;j++)
         {
            x[i][j] = complex<float> (complex_val[j][0], complex_val[j][1]);
            float mag=1.f+norm(x[i][j]);
            //float mag = 1.f + sqr(complex_val[j][0]) + sqr(complex_val[j][1]);
            tmp2[j] += mag;
            pow_average[i][j] = .7*pow_average[i][j] + .3*mag;
            //mag = 1/sqrt(mag);
            mag = approx_sqrt_1(mag);
            w[i][j]=x[i][j]*mag;
            //cout << "(" << w[i][j] << ", " << w[i][length-j] << ") ";
         }
         x[i][0]=w[i][0]=0;
         pow_average[i][0] = 0;
         //x[i][pslength-1]=w[i][pslength-1]=0;
         //cout << endl;

         Vector<complex<float> > &fr = *Vector<complex<float> >::alloc(pslength);
         for (int j=0;j<pslength;j++)
            fr[j] = x[i][j];
         fft_frames[i] = &fr;
      }
#if 1
      for (int i=0;i<nb_channels;i++)
      {
         if (valid_count%150==0)
         {
            for (int j=1;j<pslength-1;j++)
            {
               float Sf = .25f*pow_average[i][j-1]+.5f*pow_average[i][j]+.25f*pow_average[i][j+1];
               Smin[i][j] = min(Stmp[i][j], Sf);
               Stmp[i][j] = Sf;
            }
         } else {
            for (int j=1;j<pslength-1;j++)
            {
               float Sf = .25f*pow_average[i][j-1]+.5f*pow_average[i][j]+.25f*pow_average[i][j+1];
               Smin[i][j] = min(Smin[i][j], Sf);
               Stmp[i][j] = min(Stmp[i][j], Sf);      
            }
         }  
      }


      adapt = 1.0/(count+1);
      if (adapt<.05)
         adapt=.05;
      for (int i=0;i<nb_channels;i++)
      {
         for (int j=1;j<pslength-1;j++)
         {
            float Sf = .25f*pow_average[i][j-1]+.5f*pow_average[i][j]+.25f*pow_average[i][j+1];
            float mag = norm(x[i][j]);
            //FIXME: should probably be noise[i][j] > Sf instead
            if (Sf < 3*Smin[i][j] || count<100 || noise[i][j] > mag)
               noise[i][j] = (1-adapt)*noise[i][j] + adapt*mag;
            //reverb[i][j] = reverb_decay2*reverb[i][j] + reverb_coef2*max(0.f, mag - noise[i][j] - reverb[i][j]);
            float post = mag - noise[i][j] - reverb[i][j];
            if (post < 0)
               post = 0;
            //reverb[i][j] = reverb_decay2*reverb[i][j] + reverb_coef2*mag;
            //reverb[i][j] = reverb_decay2*reverb[i][j];
            //reverb[i][j] = sqrt(reverb_decay2)*reverb[i][j] + .5*reverb_coef2*mag;
            float gamma = .15;
            /*if (mag/(1+noise[i][j]+reverb[i][j]) > 2)
               gamma = .2;
            if (mag/(1+noise[i][j]+reverb[i][j]) > 4)
            gamma = .4;*/
            float prior = ((1.f-gamma)*last_ps[i][j] + gamma*post)/(1.f+noise[i][j]+reverb[i][j]);
            //reverb[i][j] += reverb_coef2*mag;
            float gain = prior/(1.f+prior);
            last_ps[i][j] = mag*gain*gain;
            //reverb[i][j] = sqrt(reverb_decay2)*reverb[i][j] + .5*reverb_coef2*mag;
            reverb[i][j] = reverb_decay*reverb[i][j] + reverb_coef*last_ps[i][j];
            //reverb[i][j] = reverb_decay*reverb[i][j] + reverb_decay*reverb_level*last_ps[i][j];
            /*if (prior>1)
            gain *= pow(prior,.15f);*/
            /*if (prior>1)
               gain *= pow(prior,.45f);
            else
               gain *= pow(prior,.15f);*/
            //gain *= pow(prior,.15f);
            /*if (gain>2)
            {
               cerr << j << " " << prior << endl;
               gain = 2;
         }*/
            /*if (i >= 3)
            {
               gain = 0;
            } else {
               gain *= 3.06;
            }*/
            //gain *= .75;
            w[i][j] *= gain;
         }
         FD::Vector<float> &noise_fr = *FD::Vector<float>::alloc(pslength);
         for (int j=0;j<pslength;j++)
            noise_fr[j] = noise[i][j];
         noise_frames[i] = &noise_fr;

      }
#endif 
      /*for (int j=0;j<pslength;j++)
         cout << norm(x[0][j]) << " ";
      for (int j=0;j<pslength;j++)
         cout << norm(w[0][j]) << " ";
      cout << endl;
      */
      /*
      for (int j=1;j<pslength;j++)
      {
         float fact = (tmp2[j]-.4*8*nfloor[j]*nfloor[j])/(.1+tmp2[j]);
         if (fact<0)
            fact=0;
         fact=1;
         if (tmp2[j]>8*nfloor[j]*nfloor[j])
            fact *= pow(tmp2[j]/(8.*nfloor[j]*nfloor[j]),.1);
         //fact *= sqrt(fact);
         //fact += .2;
         //fact=1;
         //cout << fact << " ";
         for (int i=0;i<nb_channels;i++)
         {
            w[i][j] *= fact;
         }
      }
      */





#if 1


      int N1=4, N2=20, N3=100;
      int up1=0, up2=0, up3=0;
      if ((count+1)%N1==0)
         up1=1;
      if ((count+1)%N2==0)
         up2=1;
      if ((count+1)%N3==0)
         up3=1;
      up3=up2=0;
      //Correlations
      int pair_id=0;
      for (int i=0;i<nb_channels;i++)
      {
         /*cout << i << ": ";*/
         for (int j=i+1;j<nb_channels;j++)
         {
            //cerr << "pair " << i << " " << j << " " << pair_id << endl;
#if 0
            tmp[0] = 0;
            tmp[(length>>1)] = 0;
            for (int k=1;k<(length>>1);k++)
            {
               tmp[k] = w[i][k]*w[j][k] +  w[i][length-k]*w[j][length-k];
               tmp[length-k] = w[i][length-k]*w[j][k] -  w[i][k]*w[j][length-k];
            }
#endif

            //Set to 1 to cut high frequencies
#if 0
            for (int k=1*(length>>3);k<7*(length>>3);k++)
               tmp[k]=0;
#endif

#if 0
            for (int k=0;k<pslength;k++)
               autocor[0][pair_id][k] += w[i][k]*conj(w[j][k]);
#else       //Faster version
            for (int k=0;k<pslength;k++)
               autocor[0][pair_id][k] += complex<float> (real(w[i][k])*real(w[j][k])+imag(w[i][k])*imag(w[j][k]) , imag(w[i][k])*real(w[j][k])-real(w[i][k])*imag(w[j][k]));
#endif
            pair_id++;
         }
         /*cout << endl;*/
      }

      if (up1)
      {
         update_corr(0);
         find_sources(0, count, 4, N1);
         for (int pair_id=0;pair_id<28;pair_id++)
            for (int k=0;k<pslength;k++)
               autocor[0][pair_id][k] = 0;
      }
#if 0
      if (up2)
      {
         update_corr(1);
         find_sources(1, count, 4, N2);
         for (int pair_id=0;pair_id<28;pair_id++)
            for (int k=0;k<pslength;k++)
               autocor[1][pair_id][k] = 0;
      }
      if (up3)
      {
         update_corr(2);
         find_sources(2, count, 6, N3);
#if 0
         for (int pair_id=0;pair_id<28;pair_id++)
            for (int k=0;k<pslength;k++)
            autocor[2][pair_id][k] = 0;
#endif
      }

#endif
#endif
      (*(outputs[outputID].buffer))[count]=srcList;
      (*(outputs[attentionID].buffer))[count]=attention;
      valid_count++;
   }

   IN_ORDER_NODE_SPEEDUP(LocalizeBeam)
      
};
