/* Copyright (C) 2003-2005 Jean-Marc Valin

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
#include "fftw3.h"
#include "conj_window.h"
#include "solver.h"
#include "Matrix.h"
#include "Map.h"
#include "SourceInfo.h"

using namespace FD;

class SeparGSS;

#define fftw_malloc fftwf_malloc

DECLARE_NODE(SeparGSS)
/*Node
 *
 * @name SeparGSS
 * @category ManyEars
 * @description Fixed SeparGSS
 *
 * @input_name INPUT
 * @input_type Vector<ObjectRef>
 * @input_description Input frames in frequency domain
 *
 * @input_name NOISE
 * @input_type Vector<ObjectRef>
 * @input_description Background noise spectrum
 *
 * @input_name SOURCES
 * @input_type Vector<ObjectRef>
 * @input_description Source location
 *
 * @input_name MIC_POS
 * @input_type Matrix<float>
 * @input_description Microphone position
 *
 * @output_name OUTPUT
 * @output_type Map<int,ObjectRef>
 * @output_description Separated output
 *
 * @parameter_name LENGTH
 * @parameter_type int
 * @parameter_description Frame size
 *
 * @parameter_name SPEED_OF_SOUND
 * @parameter_type float
 * @parameter_description Speed of sound (m/s)
 *
 * @parameter_name SAMPLING_RATE
 * @parameter_type int
 * @parameter_description Sampling Rate (Hz)
 *
 * @parameter_name LOC_DELAY
 * @parameter_type int
 * @parameter_description Delay (in frames) introduced by localization
 *
 * @parameter_name REVERB_LEVEL
 * @parameter_type float
 * @parameter_description Amount of reverberation present
 * 
 * @parameter_name REVERB_DECAY
 * @parameter_type float
 * @parameter_description Reverberation decay (per frame)
 * 
 *
END*/


#define MAX_CHAN   8
#define MAX_SRC    30
#define MAX_PS   513

#define USE_REAL_SOURCE_POS
//#define FILTER_FOR_8KHZ

static const float zmin=.1, zmax=.316;
static const float zmin1=.1, zmax1=.316;
static const float zmin2=.1, zmax2=.316;

#if 0

float qcurve(float x, float z1, float z2)
{
   if (x < z1)
   {
      return 0;
   } else if (x > z2)
   {
      return  1;
   } else {
      return log(x/z1) / log(z2/z1);
   }
}

#else
float qcurve(float x, float z1, float z2)
{
   return 1.f/(1.f+.1f/(1e-15+x*x));
}

#endif

static const float g_window[29] = {0.000728, 0.002882, 0.006366, 0.011029, 0.016667, 0.023033, 0.029849, 0.036818, 0.043634, 0.050000, 0.055638, 0.060301, 0.063785, 0.065938, 0.066667, 0.065938, 0.063785, 0.060301, 0.055638, 0.050000, 0.043634, 0.036818, 0.029849, 0.023033, 0.016667, 0.011029, 0.006366, 0.002882, 0.000728};

static const float g_window2[21] = {0.001841, 0.007216, 0.015688, 0.026572, 0.038986, 0.051923, 0.064337, 0.075221, 0.083693, 0.089068, 0.090909, 0.089068, 0.083693, 0.075221, 0.064337, 0.051923, 0.038986, 0.026572, 0.015688, 0.007216, 0.001841};

#if 0
static float hypergeom_gain(float x)
{
   int ind;
   float integer, frac;
   static const float table[21] = {
      0.82157, 1.02017, 1.20461, 1.37534, 1.53363, 1.68092, 1.81865,
      1.94811, 2.07038, 2.18638, 2.29688, 2.40255, 2.50391, 2.60144,
      2.69551, 2.78647, 2.87458, 2.96015, 3.04333, 3.12431, 3.20326};

   if (x>9.5)
      return 1+.12/x;

   integer = floor(x);
   frac = x-integer;
   ind = (int)integer;

   return ((1-frac)*table[ind] + frac*table[ind+1])/sqrt(x+.0001);
}
#else

static float hypergeom_gain(float x)
{
   int ind;
   float integer, frac;
   static const float table[21] = {
      0.82157f, 1.02017f, 1.20461f, 1.37534f, 1.53363f, 1.68092f, 1.81865f,
      1.94811f, 2.07038f, 2.18638f, 2.29688f, 2.40255f, 2.50391f, 2.60144f,
      2.69551f, 2.78647f, 2.87458f, 2.96015f, 3.04333f, 3.12431f, 3.20326f};
      
   if (x>9.5)
      return 1+.1296/x;
   
   integer = floor(2*x);
   frac = 2*x-integer;
   ind = (int)integer;
   
   return ((1-frac)*table[ind] + frac*table[ind+1])/sqrt(x+.0001f);
}

static float hypergeom_gain2(float x)
{
   int ind;
   float integer, frac;
   static const float table[21] = {
      0.88623, 1.09501, 1.28192, 1.45142, 1.60682, 1.75066, 1.88488,
      2.01098, 2.13015, 2.24335, 2.35134, 2.45474, 2.55407, 2.64977,
      2.74218, 2.83163, 2.91837, 3.00263, 3.08461, 3.16449, 3.24241};

   if (x>9.5)
      return 1+0.253/x;
   
   integer = floor(2*x);
   frac = 2*x-integer;
   ind = (int)integer;
   
   return ((1-frac)*table[ind] + frac*table[ind+1])/sqrt(x+.0001f);
}

static float hypergeom_gain_log(float x)
{
   int ind;
   float integer, frac;
   static const float table[21] = {
      0.75008, 0.93640, 1.11688, 1.28852, 1.45013, 1.60184, 1.74422, 
      1.87812, 2.00448, 2.12417, 2.23799, 2.34656, 2.45053, 2.55031,
      2.64646, 2.73921, 2.82895, 2.91602, 3.00049, 3.08265, 3.16270};

      if (x>9.5)
         return 1;
   
      integer = floor(2*x);
      frac = 2*x-integer;
      ind = (int)integer;
   
      return ((1-frac)*table[ind] + frac*table[ind+1])/sqrt(x+.0001f);
}

#endif

class SeparGSS : public BufferedNode {
   int micInputID;
   int inputID;
   int noiseID;
   int sourcesID;
   int outputID;

   fftwf_plan plan_backward;
   float *real_val;
   fftwf_complex *complex_val;
   int length;
   int length_2;
   int pslength;
   int samplingRate;
   float speedOfSound;
   float speedOfSound_1;
   std::vector<float> window;
   //vector<float> mem;
   float mem[MAX_SRC][MAX_PS];

   int nb_src;
   float alpha;

   //GSS-related variables
   std::complex<float> Rxxk[MAX_PS][MAX_CHAN][MAX_CHAN];
   std::complex<float> Wk[MAX_PS][MAX_SRC][MAX_CHAN];

   //Post-processor-related variables
   std::complex<float> Y[MAX_PS][MAX_SRC];
   float src_est[MAX_PS][MAX_SRC];
   float src_last[MAX_PS][MAX_SRC];
   float src_echo[MAX_PS][MAX_SRC];
   float zeta[MAX_PS][MAX_SRC];
   float Smin[MAX_PS][MAX_SRC];
   float Stmp[MAX_PS][MAX_SRC];
   float post[MAX_PS][MAX_SRC];
   float prior[MAX_PS][MAX_SRC];
   float back_noise[MAX_PS][MAX_SRC];

   int src_count[MAX_SRC];
   float zpeak[MAX_SRC];
   float zlast[MAX_SRC];

   int src_ref[MAX_SRC];
   int src_init[MAX_SRC];

   bool isInit;
   float mic[8][3];

   std::vector<std::vector<float> > src_deltas;
   int separ_length;

   float reverb_level;
   float reverb_decay;
   int delay;
   
public:
   SeparGSS(std::string nodeName, ParameterSet params)
   : BufferedNode(nodeName, params)
   {
      //cerr << "SeparGSS constructor\n";
      inputID = addInput("INPUT");
      noiseID = addInput("NOISE");
      micInputID = addInput("MIC_POS");
      sourcesID = addInput("SOURCES");
      outputID = addOutput("OUTPUT");
      length = dereference_cast<int> (parameters.get("LENGTH"));
      speedOfSound = dereference_cast<float> (parameters.get("SPEED_OF_SOUND"));
      samplingRate = dereference_cast<int> (parameters.get("SAMPLING_RATE"));
      delay = dereference_cast<int> (parameters.get("LOC_DELAY"));

      reverb_level = dereference_cast<float> (parameters.get("REVERB_LEVEL"));
      reverb_decay = dereference_cast<float> (parameters.get("REVERB_DECAY"));
      reverb_level *= 1-reverb_decay;

      speedOfSound_1 = samplingRate/(100.*speedOfSound);
      pslength = length/2 + 1;
      length_2 = length/2;
      isInit = false;

      window.resize(length);
      conj_window(&window[0],length);

      for (int i=0;i<MAX_SRC;i++)
         for (int k=0;k<MAX_PS;k++)
            mem[i][k] = 0;

      inOrder = true;
      /*inputsCache[inputID].lookBack = delay;
      inputsCache[noiseID].lookBack = delay;*/
      inputsCache[inputID].lookBack = max(80,delay);
      inputsCache[noiseID].lookBack = max(80,delay);

#ifdef FILTER_FOR_8KHZ
      separ_length = 85;
#else
      separ_length = 169;
#endif
      nb_src = 0;

      real_val = (float*)fftw_malloc(length*sizeof(float));
      complex_val = (fftwf_complex*)fftw_malloc(pslength*sizeof(fftwf_complex));

      plan_backward = fftwf_plan_dft_c2r_1d(length, complex_val, real_val, FFTW_PATIENT);

      for (int k=0;k<MAX_PS;k++)
         for (int i=0;i<MAX_CHAN;i++)
            for (int j=0;j<MAX_CHAN;j++)
               Rxxk[k][i][j] = 0.1;

      //cerr << "SeparGSS constructor2\n";
      float delt[3][8] = {
         {-13.4036, -40.9243, 41.0992, 5.6753, -10.5627, -36.6207, 44.7855, 9.9512},
         {11.4288, -20.1366, -21.1408, -47.5487, 50.1822, 18.7733, 17.3695, -8.9276},
         {41.2451, 8.9306, 33.9998, 2.2823, -1.3331, -34.9172, -8.9643, -41.2433}};
      src_deltas.resize(MAX_SRC);
      for (int i=0;i<MAX_SRC;i++)
      {
         src_deltas[i].resize(8);
         for (int j=0;j<8;j++)
            src_deltas[i][j] = 0;
      }

      alpha=.01f;

      /*addSource(0);
      addSource(1);
      addSource(2);*/
      //cerr << "SeparGSS constructor end\n";

   }

   int addSource(int ref)
   {
      int id = nb_src;
      nb_src++;
      src_ref[id] = ref;

      for (int k=0;k<MAX_PS;k++)
         mem[id][k]=src_echo[k][id]=src_est[k][id]=src_last[k][id]=zeta[k][id]=Smin[k][id]=Stmp[k][id]=post[k][id]=prior[k][id]=back_noise[k][id] = 0;
      /* Set back_noise to the average of other present sources */
      /*float fact=1;
      if (id>1)
         fact = 1.0f/id;
      for (int k=0;k<MAX_PS;k++)
         for (int i=0;i<id;i++)
            back_noise[k][id] += fact*back_noise[k][i];*/
      src_init[id] = 0;
      zpeak[id] = 0;
      zlast[id] = 0;
      src_count[id] = 0;
      return id;
      //Do a better init
   }

   void removeSource(int ref)
   {
      int id = rmap(ref);
      for (int i=id;i<nb_src-1;i++)
      {
         for (int k=0;k<MAX_PS;k++)
         {
            src_est[k][i]=src_est[k][i+1];
            src_last[k][i]=src_last[k][i+1];
            src_echo[k][i]=src_echo[k][i+1];
            zeta[k][i]=zeta[k][i+1];
            Smin[k][i]=Smin[k][i+1];
            Stmp[k][i]=Stmp[k][i+1];
            post[k][i]=post[k][i+1];
            prior[k][i]=prior[k][i+1];
            back_noise[k][i] = back_noise[k][i+1];
            for (int j=0;j<MAX_CHAN;j++)
               Wk[k][i][j] = Wk[k][i+1][j];
         }
         for (int k=0;k<MAX_CHAN;k++)
            src_deltas[i][k] = src_deltas[i+1][k];
         zpeak[i] = zpeak[i+1];
         zlast[i] = zlast[i+1];
         src_count[i] = src_count[i+1];
         src_init[i] = src_init[i+1];
         src_ref[i] = src_ref[i+1];
      }
      nb_src--;
   }

   int rmap(int id)
   {
      for (int i=0;i<nb_src;i++)
         if (id==src_ref[i])
            return i;
      //cerr << "reverse mapping failed\n";
      return -1;
   }

   int rmapOrAdd(int id)
   {
      for (int i=0;i<nb_src;i++)
         if (id==src_ref[i])
            return i;
      addSource(id);
      return nb_src-1;
   }

   void calculate(int output_id, int count, Buffer &out)
   {
      /* Microphone position */
      if (!isInit)
      {
         isInit = true;
         RCPtr<Matrix<float> > micValue = getInput(micInputID, count);

         const Matrix<float> &mat = *micValue;
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
            mic[i][0]=mat[i][0]-pos_mean[0];
            mic[i][1]=mat[i][1]-pos_mean[1];
            mic[i][2]=mat[i][2]-pos_mean[2];
         }
      }

      if (count < delay)
      {
         Map<int,ObjectRef> &output = *new Map<int,ObjectRef>;
         out[count] = &output;
         return;
      }

      /* Getting audio (in freq. domain) */
      RCPtr<Vector<ObjectRef> > inputValue = getInput(inputID, count-delay);
      const Vector<ObjectRef> &in = *inputValue;
      
      RCPtr<Vector<ObjectRef> > noiseValue = getInput(noiseID, count-delay);
      const Vector<ObjectRef> &noise_in = *noiseValue;

      int nb_channels = in.size();
      float chan_1 = 1.0/nb_channels;

#ifdef USE_REAL_SOURCE_POS
      /* Deal with new/updated sources */
      RCPtr<Vector<ObjectRef> > input2Value = getInput(sourcesID, count);
      const Vector<ObjectRef> &sources = *input2Value;
      int src_there[nb_src];
      int old_nb_src = nb_src;
      for (int i=0;i<nb_src;i++)
         src_there[i] = 0;
      for (int i=0;i<sources.size();i++)
      {
         const SourceInfo &src = object_cast<SourceInfo> (sources[i]);
         int id = rmap(src.source_id);
         if (id == -1)
         {
            id = addSource(src.source_id);
            std::cerr << "Created source " << src.source_id << " with id " << id << std::endl;
         } else {
            src_there[id] = 1;
         }
         float dist = 15000;
         float pos[3];
         pos[0] = dist*src.x[0];
         pos[1] = dist*src.x[1];
         pos[2] = dist*src.x[2];
         float d0 = sqrt(sqr(pos[0])+sqr(pos[1])+sqr(pos[2]));
         for (int j=0;j<nb_channels;j++)
         {
            src_deltas[id][j]=speedOfSound_1*(d0-sqrt(sqr(pos[0]-mic[j][0])+sqr(pos[1]-mic[j][1])+sqr(pos[2]-mic[j][2])));
         }
         src_count[i]++;
      }

      /* Remove sources that are no longer there (order is important) */
      for (int i=old_nb_src-1;i>=0;i--)
         if (!src_there[i])
         {
            std::cerr << "removing source " << i << " (id " << src_ref[i] << ")\n";
            removeSource(src_ref[i]);
         }
#else

#define HALL_COQUE 1001
#define LAB_COQUE 1002
#define LAB_CUBE 1003
#define HALL_CUBE 1004
#define SNR_COQUE 2001
#define SNR_CUBE 2002

#define ANGLE HALL_COQUE
      int nbSources = 3;
      static bool src_created = false;
      if (!src_created)
      {
         src_created = true;
#if ANGLE == 90
         float angles[3] = {89.8,0.1,-88.0};
         float elevat[3] = {-3.0, -3.5, -2.0};
#elif ANGLE == 60
         float angles[3] = {61.06f, 0.22f, -60.1f};
         float elevat[3] = {-4.f, -3.7, -2.7};
#elif ANGLE == 45
         float angles[3] = {41.4,0.1,-40.4};
         float elevat[3] = {-4.7, -3.6, -3.3};
#elif ANGLE == 30
         float angles[3] = {27.2,-0.1,-25.0};
         float elevat[3] = {-4.3, -3.3, -2.6};
#elif ANGLE == HALL_COQUE
         float angles[3] = {89.8,-0.5,-88.68};
         float elevat[3] = {-14.1, -12.8, -13.58};
#elif ANGLE == LAB_COQUE
         float angles[3] = {88.2,1.1,-89.2};
         float elevat[3] = {-12.2, -11.5, -12.2};
#elif ANGLE == LAB_CUBE
         float angles[3] = {-90.41,-179.1,91.5};
         float elevat[3] = {-24.0, -22.0, -23.5};
#elif ANGLE == HALL_CUBE
         float angles[3] = {-89.2,178.8,93.5};
         float elevat[3] = {-26.8, -23.1, -26.0};
#elif ANGLE == SNR_COQUE
         float angles[3] = {-91.4,0.5,134.6};
         float elevat[3] = {-13.3, -19.7, 3.2};
#elif ANGLE == SNR_CUBE
         float angles[3] = {-124.1,178.6,93.8};
         float elevat[3] = {-5.1, -30.1, -20.2};
#elif ANGLE == TEST
         float angles[3] = {-91.4,0.5,134.6};
         float elevat[3] = {-13.3, -19.7, 3.2};
#else
#error no ANGLE defined
#endif
         for (int i=0;i<nbSources;i++)
            addSource(i);
         for (int id=0;id<nbSources;id++)
         {
            float pos[3];
            pos[0] = 200*cos(elevat[id]*M_PI/180.)*cos(angles[id]*M_PI/180.);
            pos[1] = 200*cos(elevat[id]*M_PI/180.)*sin(angles[id]*M_PI/180.);
            pos[2] = -200*sin(elevat[id]*M_PI/180.);
            float d0 = sqrt(sqr(pos[0])+sqr(pos[1])+sqr(pos[2]));
            for (int i=0;i<nb_channels;i++)
            {
               src_deltas[id][i]=speedOfSound_1*(d0-sqrt(sqr(pos[0]-mic[i][0])+sqr(pos[1]-mic[i][1])+sqr(pos[2]-mic[i][2])));
            }
         }
      } 
#endif

      /* Start compute */
      Vector<Vector<std::complex<float> > *> x;
      x.resize(nb_channels);
      for (int i=0;i<nb_channels;i++)
         x[i] = &object_cast<Vector<std::complex<float> > > (in[i]);
      rt_assert (pslength == x[0]->size());

      Vector<Vector<float> *> noise_ptrs;
      noise_ptrs.resize(nb_channels);
      for (int i=0;i<nb_channels;i++)
         noise_ptrs[i] = &object_cast<Vector<float> > (noise_in[i]);


#if 0  //Hardcoded positions
#if 0
      float deltas[3][8] = {
         {34,  18,  36,  20, -20, -36, -16, -34},
         {42,  26, -26, -32,  38,  20, -30, -38},
         {-41.685, -40.280, 21.212, 13.700, -19.474, -15.899, 44.521, 37.906}};
#else
      float deltas[3][8] = {
         {-13.4036, -40.9243, 41.0992, 5.6753, -10.5627, -36.6207, 44.7855, 9.9512},
         {11.4288, -20.1366, -21.1408, -47.5487, 50.1822, 18.7733, 17.3695, -8.9276},
         {41.2451, 8.9306, 33.9998, 2.2823, -1.3331, -34.9172, -8.9643, -41.2433}};
#endif
#endif

      std::complex<float> Ah_base[nb_src][nb_channels];
      std::complex<float> Ah[nb_src][nb_channels];
      for (int i=0;i<nb_src;i++)
         for (int j=0;j<nb_channels;j++)
         {
            float theta = 2*M_PI*src_deltas[i][j]/length;
            Ah_base[i][j] = std::complex<float> (cos(theta), -sin(theta));
            Ah[i][j] = 1+src_deltas[i][j]/200.0;
         }

      float kalphaJ = 4*nb_channels;
      float kalphaC = 2./nb_channels;
      float zetaF[nb_src];
      for (int i=0;i<nb_src;i++)
         zetaF[i] = 0;
      const float leak_factor = 0.12f;
      const float over_cancel = 1.2f;
      const float noise_compens = 1.4f;
      int init[nb_src];
      for (int i=0;i<nb_src;i++)
      {
         init[i] = src_init[i];
         src_init[i] = 1;
      }

      float Salpha, Sbeta;
      float alpha_leak = leak_factor * over_cancel;
      Sbeta = -alpha_leak/(1-alpha_leak*alpha_leak+alpha_leak*(1-alpha_leak)*(nb_src-2));
      Salpha = 1 - (nb_src-1)*alpha_leak*Sbeta;
      //cerr << "Salpha = " << Salpha << " Sbeta = " << Sbeta << endl;

      //Salpha = 1; Sbeta = 0.00001;
      std::complex<float> W_last[nb_src][MAX_CHAN];

      float tot_noise_1[MAX_PS][MAX_SRC];
            
      /* Main frequency loop */
      for (int k=0;k<pslength;k++)
      {
         if (k>separ_length)
         {
            for (int i=0;i<nb_src;i++)
            {
               Y[k][i] = 0.f;
            }
            continue;
         }
         std::complex<float> (*W)[MAX_CHAN] = &Wk[k][0];
         std::complex<float> (*Rxx)[MAX_CHAN] = &Rxxk[k][0];

         for (int i=0;i<nb_src;i++)
         {
            if (!init[i])
            {
               for (int j=0;j<nb_channels;j++)
                  W[i][j] = .8f*Ah[i][j]*chan_1;
               back_noise[k][i] = 0;
               for (int j=0;j<nb_channels;j++)
                  back_noise[k][i] += (*noise_ptrs[j])[k];
               back_noise[k][i] *= noise_compens*chan_1*chan_1;
            }
         }
         
         std::complex<float> S[nb_src];

         std::complex<float> xx[nb_channels];
         for (int j=0;j<nb_channels;j++)
            xx[j] = (*x[j])[k];

/*         float noise[nb_channels];
         for (int j=0;j<nb_channels;j++)
            noise[j] = (*noise_ptrs[j])[k];*/
         
         int max_src=0;
         float max_ener=-1;
         for (int i=0;i<nb_src;i++)
         {
            S[i] = 0;
            for (int j=0;j<nb_channels;j++)
               S[i] += W[i][j]*xx[j];
            if (norm(S[i])>max_ener)
            {
               max_src = i;
               max_ener = norm(S[i]);
            }
         }


         {
            //Minimizing output correlation
            float alpha2;
            std::complex<float> Ryy[nb_src][nb_src];

            //Optimize based on single data (stochastic gradient type)
            float Rxx_norm=0;
            for (int i=0;i<nb_channels;i++)
               Rxx_norm += norm(xx[i]);
            Rxx_norm *= Rxx_norm;
            
            float Rxx_norm_1 = 1.f/(1.f+Rxx_norm);
            alpha2 = alpha*kalphaJ*Rxx_norm_1;
            for (int i=0;i<nb_src;i++)
            {
               for (int j=i;j<nb_src;j++)
               {
                  Ryy[i][j] = S[i]*conj(S[j]);
                  Ryy[j][i] = conj(Ryy[i][j]);
               }
            }
            //If we remove that, we have an MVDR beamformer (well, maybe?)
            for (int i=0;i<nb_src;i++)
               Ryy[i][i] = 0;
            std::complex<float> tmp[nb_src];
            for (int i=0;i<nb_src;i++)
            {
               tmp[i] = 0;
               for (int j=0;j<nb_src;j++)
               {
                  tmp[i] += Ryy[i][j]*S[j];
               }
            }
            //float sum2=0;
            for (int i=0;i<nb_src;i++)
            {
               for (int j=0;j<nb_channels;j++)
               {
                  W[i][j] -= alpha2*tmp[i]*conj(xx[j]);
                  //sum2 += norm(alpha2*tmp[i]*conj(xx[j]));
                  /*if (norm(W[i][j]) > .2)
                    cerr << norm(W[i][j]) << endl;*/
               }
            }
            //cout << sum2 << " " << Rxx_norm << endl;

#if 1
            //Computing C2 Constraint
            float alpha1=alpha*kalphaC;
            std::complex<float> C[nb_src][nb_src];
            for (int i=0;i<nb_src;i++)
               for (int j=0;j<nb_src;j++)
               {
                  C[i][j] = 0;
                  for (int m=0;m<nb_channels;m++)
                     C[i][j] += W[i][m]*conj(Ah[j][m]);
                  //if (i!=j) C[i][j] *= .5;
               }

            for (int i=0;i<nb_src;i++)
               C[i][i] = 1.f*(C[i][i]-1.f);

            for (int i=0;i<nb_src;i++)
            {
               for (int j=0;j<nb_channels;j++)
               {
                  std::complex<float> grad=0;
                  for (int m=0;m<nb_src;m++)
                     grad += C[i][m]*Ah[m][j];
                  W[i][j] -= alpha1*grad;
                  W[i][j] *= .995f;
               }
            }
#endif


#if 0   /* Enforces continuity in frequency */
            if (k>1 && k<pslength-2)
            {
               for (int i=0;i<nb_src;i++)
               {
                  for (int j=0;j<nb_channels;j++)
                  {
                     std::complex<float> diff = W[i][j] - .5f*(Wk[k+1][i][j]+W_last[i][j]);
                     W[i][j] -= .002f*diff;
                  }
               }
            }
            for (int i=0;i<nb_src;i++)
               for (int j=0;j<nb_channels;j++)
                  W_last[i][j] = W[i][j];
#endif
#if 0    /* Hard C1 constraint */
            for (int i=0;i<nb_src;i++)
            {
               std::complex<float> resp=0;
               for (int m=0;m<nb_channels;m++)
                  resp += W[i][m]*conj(Ah[i][m]);
               resp = 1.f/resp;
               for (int m=0;m<nb_channels;m++)
                  W[i][m] *= resp;
            }
#endif

         }


#if 1
         for (int i=0;i<nb_src;i++)
         {
            S[i] = 0;
            for (int j=0;j<nb_channels;j++)
               S[i] += W[i][j]*xx[j];
            Y[k][i] = S[i];
         }
#else /* LS-C2 */
         ls_beamformer(3, Ah, xx, S);
         for (int i=0;i<nb_src;i++)
            Y[k][i] = S[i];
#endif

         /*for (int i=0;i<nb_src;i++)
         {
            back_noise[k][i] = 0;
            for (int j=0;j<nb_channels;j++)
            {
               back_noise[k][i] += noise[j]*norm(W[i][j]);
            }
         }*/
         
#if 1
         float src_est_noback[nb_src];
         float src_est_nobias[nb_src];
         float src_est_instant[nb_src];
         
         for (int i=0;i<nb_src;i++)
         {
#if 0
            src_est_instant[i] = norm(Y[k][i]);
            float sumW=0, sumWA=0;
            for (int j=0;j<nb_channels;j++)
            {
               src_est_instant[i] -= norm(W[i][j])*norm(xx[j]);
               sumW+=norm(W[i][j]);
               sumWA+=norm(W[i][j]*Ah[i][j]);
            }
            src_est_instant[i] *= sumWA/(sumWA - sumW);
            if (src_est_instant[i]<1)
               src_est_instant[i] = 1;
            src_est[k][i] = .6*src_est[k][i] + .4*src_est_instant[i];
            src_est_noback[i] = src_est[k][i] - .0*back_noise[k][i];
#else
            //src_est[k][i] = .5*src_last[k][i] + .5*norm(Y[k][i]);
            src_est[k][i] = .3*src_est[k][i] + .7*norm(Y[k][i]);
            src_est_noback[i] = src_est[k][i] - .5*back_noise[k][i];
            if (src_est_noback[i] < 1.f)
               src_est_noback[i] = 1.f;
#endif
         }

#if 1
         /* Remove "self-contribution" effect from estimate */
         for (int i=0;i<nb_src;i++)
         {
            src_est_nobias[i] = Salpha * src_est_noback[i];
            for (int j=0;j<nb_src;j++)
               if (i!=j)
                  src_est_nobias[i] += Sbeta * src_est_noback[j];
            //src_est_nobias[i] = src_est_noback[i];
            if (src_est_nobias[i] < 1.f)
               src_est_nobias[i] = 1.f;
            
         }
#else
         for (int i=0;i<nb_src;i++)
         {
            src_est_nobias[i] = src_est_noback[i];
            for (int j=0;j<nb_src;j++)
               if (i!=j)
                  src_est_nobias[i] -= leak_factor*src_est[k][j];
            if (src_est_nobias[i] < 1.f)
               src_est_nobias[i] = 1.f;
         }
#endif
#else
         //if (count==100) 
         //{
            float leak_matrix[nb_src][nb_src];
            for (int i=0;i<nb_src;i++)
            {
               for (int j=0;j<nb_src;j++)
               {
                  complex<float> resp=0;
                  for (int m=0;m<nb_channels;m++)
                     resp += Ah[i][m]*conj(Ah[j][m]);
                  leak_matrix[i][j] = max(.5*leak_factor,norm(resp)*chan_1*chan_1);
                  //cerr << leak_matrix[i][j] << " ";
               }
               //cerr << endl;
            }
         //}
         float src_est_instant[nb_src];
         float src_est_nobias[nb_src];
         float M_1 = 2./(nb_channels*(nb_channels-1));
         for(int i=0;i<nb_src;i++)
         {
            float estimate = 0;
            std::complex<float> Xd[nb_channels];
            for(int j=0;j<nb_channels;j++)
               Xd[j] = Ah[i][j]*xx[j];
            for(int j1=0;j1<nb_channels;j1++)
               for(int j2=j1+1;j2<nb_channels;j2++)
                  estimate += real(Xd[j1])*real(Xd[j2]) + imag(Xd[j1])*imag(Xd[j2]);
            estimate *= M_1;
            src_est_instant[i] = estimate;
            if (src_est_instant[i]<1)
               src_est_instant[i] = 1;
            src_est[k][i] = .6*src_est[k][i] + .4*estimate;
            if (src_est[k][i]<1)
               src_est[k][i] = 1;
            src_est_nobias[i] = src_est[k][i];
         }
#if 0
         for (int i=0;i<nb_src;i++)
         {
            src_est_nobias[i] = (.5*Salpha + .5)*src_est[k][i];
            for (int j=0;j<nb_src;j++)
               if (i!=j)
                  src_est_nobias[i] += .5*Sbeta * src_est[k][j];
            if (src_est_nobias[i] < 1)
               src_est_nobias[i] = 1;
         }
#else
         for (int i=0;i<nb_src;i++)
         {
            src_est_nobias[i] = src_est[k][i];
            for (int j=0;j<nb_src;j++)
               if (i!=j)
                  src_est_nobias[i] -= leak_factor*src_est[k][j];
            if (src_est_nobias[i] < 1.f)
               src_est_nobias[i] = 1.f;
         }
#endif
#endif

#if 0
         for(int i=0;i<nb_src;i++)
         {
            src_est_nobias[i] = .5*src_est_nobias[i] + .5*src_last[k][i];
         }
#endif
         for (int i=0;i<nb_src;i++)
         {
            //float tot_noise=0;
            float tot_noise = 0;
            //float tot_noise_1;
            for (int j=0;j<nb_src;j++)
            {
               if (i!=j)
                  tot_noise += src_est_nobias[j];
            }
            if (tot_noise<1)
               tot_noise=1;
            tot_noise *= leak_factor*over_cancel;
            /*if (i==1)
            cout << tot_noise << " ";*/
            //ECHO
            for (int j=0;j<nb_src;j++)
               //if (i!=j)
                  tot_noise += src_echo[k][j];

            if (tot_noise<0)
               tot_noise=0;
#if 1 /* Re-enable that */
            tot_noise += back_noise[k][i];
#else
            tot_noise = back_noise[k][i];
#endif
            tot_noise_1[k][i] = 1.0f/(tot_noise+1);

            post[k][i] = norm(Y[k][i])*tot_noise_1[k][i]-1.f;
            //post[k][i] = src_est_instant[i]*tot_noise_1;
            
         }


         /* Update transfer function for next frequency */
         for (int i=0;i<nb_src;i++)
            for (int j=0;j<nb_channels;j++)
               Ah[i][j] *= Ah_base[i][j];

      }
      
      if (0)
      {
      //Recalculate post[k][i] based on a new estimation of interference
      for (int k=0;k<separ_length;k++)
      {
         float estimate[nb_src];
         float est_prior[nb_src];
         for (int i=0;i<nb_src;i++)
         {
            est_prior[i] = .07*max(post[k][i], 0.0f) + .93 * src_last[k][i]*tot_noise_1[k][i];
            float gain = est_prior[i]/(1+est_prior[i]);
            estimate[i] = norm(Y[k][i])*gain*gain;
         }
         for (int i=0;i<nb_src;i++)
         {
            float tot_noise = 0;
            for (int j=0;j<nb_src;j++)
            {
               if (i!=j)
                  tot_noise += estimate[j];
            }
            if (tot_noise<1)
               tot_noise=1;
            tot_noise *= leak_factor*over_cancel;
            for (int j=0;j<nb_src;j++)
               tot_noise += src_echo[k][j];

            if (tot_noise<0)
               tot_noise=0;
            tot_noise += back_noise[k][i];
            tot_noise_1[k][i] = 1.0f/(tot_noise+1);
            post[k][i] = norm(Y[k][i])*tot_noise_1[k][i]-1;
         }
        
      }
      }
         
      
      for (int k=0;k<separ_length;k++)
      {
         for (int i=0;i<nb_src;i++)
         {
            float gamma = .07;
            gamma = .05+.9*prior[k][i]*prior[k][i]/((1+prior[k][i])*(1+prior[k][i]));
            prior[k][i] = gamma * max(post[k][i], 0.0f)  +  (1-gamma) * (.8f*src_last[k][i]*tot_noise_1[k][i]+ .2f*prior[k][i]);
            //prior[k][i] = gamma * max(post[k][i], 0.0f)  +  (1-gamma) * src_last[k][i]*tot_noise_1[k][i];
            
            //prior[k][i] = gamma * src_est_instant[i]*tot_noise_1  +  (1-gamma) * src_last[k][i]*tot_noise_1;
            /*prior[k][i] = max(.5*prior[k][i] + .5*src_last[k][i]*tot_noise_1[k][i], .075f);
            float pr = (prior[k][i]/(1+prior[k][i]));
            prior[k][i] = pr * (1 + pr*post[k][i]);*/
                  
            if (prior[k][i]>100)
               prior[k][i] = 100;

            zeta[k][i] = .7f*zeta[k][i] + .3f*prior[k][i];

#ifdef FILTER_FOR_8KHZ
            if (k>7 && k<85)
               zetaF[i] += zeta[k][i];
#else
            if (k>7 && k<100)
               zetaF[i] += zeta[k][i];	    
#endif
            
         }
      }
#if 1
      /* MCRA background noise estimation */
      for (int i=0;i<nb_src;i++)
      {
         for (int k=1;k<separ_length-1;k++)
         {
            float Sf = .25*src_est[k-1][i]+.5*src_est[k][i]+.25*src_est[k+1][i];
            if (count%40!=0)
            {
               Smin[k][i]= min(Smin[k][i],Sf);
               Stmp[k][i]= min(Stmp[k][i],Sf);
            } else {
               Smin[k][i]= min(Stmp[k][i],Sf);
               Stmp[k][i]= Sf;
            }

            if (Sf < 5*Smin[k][i] || Sf < back_noise[k][i])
            {
               float noise=0;
               for (int j=0;j<nb_src;j++)
                  if (i!=j)
                     noise += norm(Y[k][j]);
               noise *= 1.5f*leak_factor;

               //back_noise[k][i] = .95*back_noise[k][i] + .05*noise_compens*max(norm(Y[k][i])-noise,0.1);
               back_noise[k][i] = .9f*back_noise[k][i] + .1*noise_compens*std::max(norm(Y[k][i]),0.1f);
            }
         }
      }
#else
      /* modified noise estimation */
      for (int i=0;i<nb_src;i++)
      {
         if (src_count[i]<45)
         {
            float adapt_rate = 1.f/(2+src_count[i]);
            std::cerr << adapt_rate << std::endl;
            for (int k=0;k<separ_length;k++)
            {
               back_noise[k][i] = (1-adapt_rate)*back_noise[k][i] + adapt_rate*noise_compens*std::max(norm(Y[k][i]),0.1f);
            }
         }
      }
#endif
      /* Frame probability of presence */
      float Pframe[nb_src];
      for (int i=0;i<nb_src;i++)
      {
#ifdef FILTER_FOR_8KHZ
	zetaF[i] /= 77;
#else
         zetaF[i] /= 92;
#endif
#if 0
         if (zetaF[i]<zmin)
         {
            Pframe[i] = 0;
         } else {
            if (zetaF[i] > 1.5*zlast[i])
            {
               Pframe[i] = 1;
               zpeak[i] = zetaF[i];
               if (zpeak[i] > 10)
                  zpeak[i] = 10;
               if (zpeak[i] < 1)
                  zpeak[i] = 1;
            } else {
               if (zetaF[i] < zpeak[i]*zmin)
               {
                  Pframe[i] = 0;
               } else if (zetaF[i] > zpeak[i]*zmax)
               {
                  Pframe[i] = 1;
               } else {
                  Pframe[i] = log(zetaF[i]/(zpeak[i]*zmin)) / log(zmax/zmin);
               }
            }
         }
#else
         Pframe[i] = qcurve(zetaF[i], zmin, zmax);
#endif
         zlast[i] = zetaF[i];
      }


      for (int k=0;k<separ_length;k++)
      {
         for (int i=0;i<nb_src;i++)
         {

            float gain;

            float zetaL;
            if (k<2 || k>separ_length-3)
               zetaL = .25*zeta[max(0,k-1)][i] + .5*zeta[k][i] + .25*zeta[min(pslength-1,k+1)][i];
            else
               zetaL = .2*zeta[k-2][i] + .2*zeta[k-1][i] + .2*zeta[k][i] + .2*zeta[k+1][i] + .2*zeta[k+2][i];
            float zetaG = 0;
            for (int j=max(-14,-14);j<=min(14,14);j++)
            {
               zetaG += g_window[j+14]*zeta[max(0,min(separ_length-1,k+j))][i];
            }
            float p1,p2;
#if 0
            if (zetaL>zmax1)
               p1=1;
            else if (zetaL<zmin1)
               p1=0;
            else
               p1 = log(zetaL/zmin1)/log(zmax1/zmin1);
            if (zetaG>zmax2)
               p2=1;
            else if (zetaG<zmin2)
               p2=0;
            else
               p2 = log(zetaG/zmin2)/log(zmax2/zmin2);
#else
            p1 = qcurve(zetaL, zmin1, zmax1);
            p2 = qcurve(zetaG, zmin2, zmax2);
#endif
#if 1
            float q = 1 - (.01+.99*p1)*(.01+.99*p2)*(.2+.8*Pframe[i]);
#else
            float q = 1 - (.1+.9*p1)*(.1+.9*p2)*(.3+.7*Pframe[i]);
#endif 
            if (q>.9f)
               q=.9f;
            if (q<.02f)
               q = .02f;

            float prior_ratio = prior[k][i]/(1.f+prior[k][i]);
            float theta = (1.f+post[k][i])*prior_ratio;
            if (theta>20)
               theta=20;
            float MM;
            //if (theta<6)
               //MM = 0.74082*pow(theta+1,.61f)/sqrt(.0001+theta);
               MM = hypergeom_gain_log(theta);
               //MM = hypergeom_gain(theta);
               //MM = sqrt(1+1/(.0001f+theta));
               //MM = 1;
            /*else
               MM=1;*/
            gain = prior_ratio * MM;

            if (gain>1.f)
               gain = 1.f;

            src_last[k][i] = gain*gain*norm(Y[k][i]);
#if 0 // We now have a parameter for this
#if 0
            float reverb_decay = .62;
            float reverb_level = .08;
#else
            float reverb_decay = .82;
            float reverb_level = .03;
#endif
            //FIXME: REMOVE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //reverb_level = 0.0;
#endif            
            src_echo[k][i] = reverb_decay*src_echo[k][i] + reverb_decay*reverb_level*src_last[k][i];
            
            float p;
            p=1/(1 + (q/(1-q))*(1+prior[k][i])*exp(-theta));

            //gain *= p;
            //gain *= sqrt(p);
            //gain *= p*p;
            //gain = pow(gain, p) * pow(.15f,1.f-p);
            //gain = min(gain, pow(gain, p) * pow(.1f,1.f-p));
            gain = pow(gain, p) * pow(.1f,1.f-p);
            //gain = (sqrt(gain)*p + (1-p)*.316f)*(sqrt(gain)*p + (1-p)*.316f);
            //gain = min(gain, pow(gain, p) * pow(.1f,1.f-p));
            //gain *= pow(.5+asin(2*p-1)/M_PI,1.5f);
            /*if (gain < .5)
               gain = 0;
            else
            gain = 1;*/
            //cout << norm(Y[k][i]) << " ";
            /*if (gain < .1)
            gain = .1;*/

            //FIXME: Re-enable that
            Y[k][i] *= gain;
         
            /*for (int j=0;j<nb_src;j++)
               if (i!=j && norm(Y[k][i]) < norm(Y[k][j]))
            Y[k][i] = 0;*/
            
            /*if (i==1)
            cout << gain << " ";*/
            //if (i==0)
            //   cout << norm(Y[k][i]) << " ";
         }

         //X[k] = Y[k][rmap(0)];
      }
      //cout << endl;
         
      //Vector<pair<int,ObjectRef> > &output = *new Vector<pair<int,ObjectRef> > (nb_src);
      Map<int,ObjectRef> &output = *new Map<int,ObjectRef>;

      out[count] = &output;
      for (int i=0;i<nb_src;i++)
      {
         for (int j=0;j<pslength;j++)
         {
            if (j<4 || j>separ_length)
               Y[j][i]=0;
            complex_val[j][0] = Y[j][i].real();
            complex_val[j][1] = Y[j][i].imag();
         }
         fftwf_execute(plan_backward);

         /*Apply synthesis GAIN + window*/
         for (int j=0;j<length;j++)
            real_val[j] *= .003*window[j];

         Vector<float> &frameout = *Vector<float>::alloc(length_2);
         output[src_ref[i]] = ObjectRef(&frameout);
         //out[count] = &output;

         for (int k=0;k<length_2;k++)
            frameout[k] = real_val[k]+mem[i][k];

         for (int k=0;k<length_2;k++)
            mem[i][k] = real_val[k+length_2];
      }

   }

   IN_ORDER_NODE_SPEEDUP(SeparGSS)

};
