// Copyright (C) 2002-2005 Jean-Marc Valin/Universite de Sherbrooke
// Copyright (C) 2006 CSIRO (some modifications from original PhD work)
// All rights reserved

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
//#include "FFTWrap.h"
#include "misc.h"
#include "conj_window.h"
#include "SourceInfo.h"
#include <complex>

#include "fftw3.h"
//#include "sphere.h"
#include "Matrix.h"

#define fftw_malloc fftwf_malloc

using namespace FD;
using namespace std;

#define NGRID1 41
#define NDIST1 5
#define NGRID2 201
#define NDIST2 25

class Localize3D;

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

DECLARE_NODE(Localize3D);
/*Node
 *
 * @name Localize3D
 * @category ManyEars:3D
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
   //float t,p;
   int delta[28];
};

inline void fold(int i, int j, int N, float pos[3])
{
   float x, y;
   float offset = .5*(N-1);
   float offset_1 = 1/offset;
   x = (float(i)-offset) * offset_1;
   y = (float(j)-offset) * offset_1;
   float mm = sqrt(max(x*x,y*y));
   float r2 = x*x + y*y;
   float r_1;
   if (r2 == 0)
      r_1 = 0;
   else
      r_1 = 1/sqrt(r2);
   x *= r_1;
   y *= r_1;
   pos[0] = x*sin(.5*M_PI*mm);
   pos[1] = y*sin(.5*M_PI*mm);
   pos[2] = -cos(.5*M_PI*mm);
   //cerr << i << " " << j << " " << mm << " " << pos[0] << " " << pos[1] << " " << pos[2] << endl;
}

#define UC unsigned char
//#define UC int

class Localize3D : public BufferedNode {
   
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
   
   //std::vector<PosInfo> dir;
   int count_up2;
   int valid_count;
   fftwf_plan plan_forward;
   fftwf_plan plan_backward;
   float *real_val;
   fftwf_complex *complex_val;
   bool isInit;
   FD::ObjectRef attention;
   float mic_pos2[8][3];

   UC grid[NGRID1][NGRID1][NDIST1][28];
   UC grid2[NGRID2][NGRID2][NDIST2][28];
   
public:
   Localize3D(string nodeName, ParameterSet params)
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
         int max_ind[3]={0,0,0};
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
            for (int j=3;j<256-3;j++)
               cor3[i][j]=max(max(cor[i][j],.9f*cor[i][j-1]),.9f*cor[i][j+1]);
         }

         for (int i=0;i<NGRID1;i++)
            for (int j=0;j<NGRID1;j++)
               for (int k=0;k<NDIST1;k++)
               {
                  UC *ind=grid[i][j][k];
                  float sum=0;

                  for (int m=0;m<28;m++)
                  {
                     sum += cor3[m][ind[m]];
                  }
                  if (sum>max_sum)
                  {
                     max_sum=sum;
                     max_ind[0] = i;
                     max_ind[1] = j;
                     max_ind[2] = k;
                  }
               }
               
         if (1)
         {
            int imin = max_ind[0] * (NGRID2-1) / (NGRID1-1) - 8;
            int jmin = max_ind[1] * (NGRID2-1) / (NGRID1-1) - 8;
            if (imin < 0)
               imin = 0;
            if (jmin < 0)
               jmin = 0;
            if (imin > NGRID2-18)
               imin = NGRID2-18;
            if (jmin > NGRID2-18)
               jmin = NGRID2-18;
            max_sum=-10;
            for (int i=imin;i<imin+17;i++)
               for (int j=jmin;j<jmin+17;j++)
                  for (int k=0;k<NDIST2;k++)
                  {
                     UC *ind=grid2[i][j][k];
                     float sum=0;
                     
                     for (int m=0;m<28;m++)
                     {
                        sum += cor[m][ind[m]];
                     }
                     if (sum>max_sum)
                     {
                        max_sum=sum;
                        max_ind[0] = i;
                        max_ind[1] = j;
                        max_ind[2] = k;
                     }
                  }
            
         }         
               
         max_sum /= nframes;
         
         int flush_ids[28];
         for (int i=0;i<28;i++)
            flush_ids[i] = -128+(int)grid[max_ind[0]][max_ind[1]][max_ind[2]][i];
         //Flush correlations corresponding to the sources
         for (int i=0;i<28;i++)
         {
            //cerr << flush_ids[i] << " ";
            //cerr << (dir[max_ind].delta[i]) << " " << flush_ids[i] << "    ";
            for (int kk=-4;kk<=4;kk++)
            {
               int ii=flush_ids[i]+kk;
               if (ii<0)
                  ii += length;
               xcorr[i][ii]=0;
            }
         }
         //cerr << endl;
         /*if (m==0 && peak==0)
            cerr << sum_peaks << " " << src.x[0] << " " << src.x[1] << " " << src.x[2] << " " << distance << endl;*/
         /*cerr << "(" << max_ind[0] << ", " << max_ind[1] << ", " << max_ind[2] << ") ";
         if (peak==3)
         cerr << endl;*/

         SourceInfo &src = *new SourceInfo;
         float pos[3];
#if 0
         fold(max_ind[0],max_ind[1],NGRID1, pos);
         float xdist = float(max_ind[2])/(NDIST1-1);
#else
         fold(max_ind[0],max_ind[1],NGRID2, pos);
         float xdist = float(max_ind[2])/(NDIST2-1);
#endif
         float dist = .3+xdist + 1.7*xdist*xdist;
         for (int mi=0;mi<3;mi++)
            pos[mi] *= dist;
			
         float norm_1 = 1.0f/sqrt(sqr(pos[0])+sqr(pos[1])+sqr(pos[2]));
         /*if (peak == 0 && max_sum>300)
            cout << "pos " << pos[0] << " " << pos[1] << " " << pos[2] << endl;*/
         src.x[0]=pos[0]*norm_1;
         src.x[1]=pos[1]*norm_1;
         src.x[2]=pos[2]*norm_1;
         
         src.x[0]=pos[0];
         src.x[1]=pos[1];
         src.x[2]=pos[2];
         if (peak==0)
         {
            float xn = max_sum/210.;
            if (xn<1)
               src.strength = .5f*xn*xn;
            else
               src.strength = 1 - .5f/(xn*xn);
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
         last_strength = src.strength;
         src.interval = m;
         //src.strength = sum_peaks;
         src.eval_count = count;
         sources.push_back(FD::ObjectRef(&src));

         if (m==3 && peak == 0)
         {
            //cout << "src " << max_sum << " " << 1 << " ";// << nb_peaks << " " << 2.5*sigma_sum << " ";
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
         for (int i=0;i<NGRID1;i++)
            for (int j=0;j<NGRID1;j++)
            {
               float npos[3];
               fold(i,j,NGRID1, npos);
               //cout << "npos " << npos[0] << " " << npos[1] << " " << npos[2] << endl;
               for (int k=0;k<NDIST1;k++)
               {
                  float pos[3];
                  float xdist = float(k)/(NDIST1-1);
                  float dist = 100*(.3+xdist + 1.7*xdist*xdist);
                  for (int m=0;m<3;m++)
                     pos[m] = npos[m]*dist;
                  float r[8];
                  for (int m=0;m<nb_channels;m++)
                  {
                     r[m]=speedOfSound_1*sqrt(sqr(pos[0]-mic_pos2[m][0])+sqr(pos[1]-mic_pos2[m][1])+sqr(pos[2]-mic_pos2[m][2]));
                  }
                  id=0;
                  for (int m=0;m<nb_channels;m++)
                  {
                     for (int n=m+1;n<nb_channels;n++)
                     {
                        grid[i][j][k][id]=(UC)(floor(128.5+r[m]-r[n]));
                        id++;
                     }
                  }
               }
            }


         id=0;
         for (int i=0;i<NGRID2;i++)
            for (int j=0;j<NGRID2;j++)
            {
               float npos[3];
               fold(i,j,NGRID2, npos);
               //cout << "npos " << npos[0] << " " << npos[1] << " " << npos[2] << endl;
               for (int k=0;k<NDIST2;k++)
               {
                  float pos[3];
                  float xdist = float(k)/(NDIST2-1);
                  float dist = 100*(.3+xdist + 1.7*xdist*xdist);
                  for (int m=0;m<3;m++)
                     pos[m] = npos[m]*dist;
                  float r[8];
                  for (int m=0;m<nb_channels;m++)
                  {
                     r[m]=speedOfSound_1*sqrt(sqr(pos[0]-mic_pos2[m][0])+sqr(pos[1]-mic_pos2[m][1])+sqr(pos[2]-mic_pos2[m][2]));
                  }
                  id=0;
                  for (int m=0;m<nb_channels;m++)
                  {
                     for (int n=m+1;n<nb_channels;n++)
                     {
                        grid2[i][j][k][id]=(UC)(floor(128.5+r[m]-r[n]));
                        id++;
                     }
                  }
               }
            }
            //exit(0);
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
            //cerr << "got some zeros" << endl;
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
            w[i][j] *= gain;
         }
         FD::Vector<float> &noise_fr = *FD::Vector<float>::alloc(pslength);
         for (int j=0;j<pslength;j++)
            noise_fr[j] = noise[i][j];
         noise_frames[i] = &noise_fr;

      }
#endif 

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
         find_sources(0, count, 2, N1);
         for (int pair_id=0;pair_id<28;pair_id++)
            for (int k=0;k<pslength;k++)
               autocor[0][pair_id][k] = 0;
      }
      (*(outputs[outputID].buffer))[count]=srcList;
      (*(outputs[attentionID].buffer))[count]=attention;
      valid_count++;
   }

   IN_ORDER_NODE_SPEEDUP(Localize3D);
      
};
