/***************************************************************************
   Copyright (C) 2004 by Jean-Marc Valin
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   
   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   
   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   
   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   
****************************************************************************/

#include "alsadevice.h"
#include <alsa/asoundlib.h>

#include <iostream>
#include <assert.h>

using namespace std;

AlsaDevice::AlsaDevice(string device, int nb_chan, unsigned int rate, snd_pcm_uframes_t period_size, int nb_periods,  bool  poll)
{
   int dir;
   int err;
   snd_pcm_hw_params_t *hw_params;
   snd_pcm_sw_params_t *sw_params;
   //snd_pcm_uframes_t period_size = 1024;
   snd_pcm_uframes_t buffer_size = 2048;
   static snd_output_t *jcd_out;

   lead_in = period_size;
   channels = nb_chan;
   
   err = snd_output_stdio_attach(&jcd_out, stderr, 0);
   
   if ((err = snd_pcm_open (&capture_handle, device.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
      fprintf (stderr, "cannot open audio device %s (%s)\n",
               device.c_str(),
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
      fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
      fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
      fprintf (stderr, "cannot set access type (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
      fprintf (stderr, "cannot set sample format (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
      fprintf (stderr, "cannot set sample rate (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   fprintf (stderr, "rate = %d\n", rate);

   if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, nb_chan)) < 0) {
      fprintf (stderr, "cannot set channel count (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   
//   unsigned int period_time = 5000;
   //period_size = 1024;
   dir = 0;
   if ((err = snd_pcm_hw_params_set_period_size_near (capture_handle, hw_params, &period_size, &dir)) < 0) {
      fprintf (stderr, "cannot set period size (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   buffer_size = period_size * nb_periods;
   dir=0;
   if ((err = snd_pcm_hw_params_set_buffer_size_near (capture_handle, hw_params, &buffer_size)) < 0) {
      fprintf (stderr, "cannot set buffer time (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
      fprintf (stderr, "cannot set capture parameters (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   snd_pcm_dump_setup(capture_handle, jcd_out);
   snd_pcm_hw_params_free (hw_params);

   if ((err = snd_pcm_sw_params_malloc (&sw_params)) < 0) {
      fprintf (stderr, "cannot allocate software parameters structure (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   if ((err = snd_pcm_sw_params_current (capture_handle, sw_params)) < 0) {
      fprintf (stderr, "cannot initialize software parameters structure (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   if ((err = snd_pcm_sw_params_set_avail_min (capture_handle, sw_params, period_size)) < 0) {
      fprintf (stderr, "cannot set minimum available count (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   if ((err = snd_pcm_sw_params (capture_handle, sw_params)) < 0) {
      fprintf (stderr, "cannot set software parameters (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   
   if ((err = snd_pcm_prepare (capture_handle)) < 0) {
      fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   
   
   
   
   if ((err = snd_pcm_open (&playback_handle, device.c_str(), SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
      fprintf (stderr, "cannot open audio device %s (%s)\n",
               device.c_str(),
               snd_strerror (err));
      assert(0);
   }
   
   if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
      fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
      fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
      fprintf (stderr, "cannot set access type (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
      fprintf (stderr, "cannot set sample format (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &rate, 0)) < 0) {
      fprintf (stderr, "cannot set sample rate (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   fprintf (stderr, "rate = %d\n", rate);

   if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, nb_chan)) < 0) {
      fprintf (stderr, "cannot set channel count (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   
   //period_size = 1024;
   dir = 0;
   if ((err = snd_pcm_hw_params_set_period_size_near (playback_handle, hw_params, &period_size, &dir)) < 0) {
      fprintf (stderr, "cannot set period size (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   buffer_size = period_size * nb_periods;
   dir=0;
   if ((err = snd_pcm_hw_params_set_buffer_size_near (playback_handle, hw_params, &buffer_size)) < 0) {
      fprintf (stderr, "cannot set buffer time (%s)\n",
               snd_strerror (err));
      assert(0);
   }


   if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
      fprintf (stderr, "cannot set playback parameters (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   snd_pcm_dump_setup(playback_handle, jcd_out);
   snd_pcm_hw_params_free (hw_params);

   
   if ((err = snd_pcm_sw_params_malloc (&sw_params)) < 0) {
      fprintf (stderr, "cannot allocate software parameters structure (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   if ((err = snd_pcm_sw_params_current (playback_handle, sw_params)) < 0) {
      fprintf (stderr, "cannot initialize software parameters structure (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   if ((err = snd_pcm_sw_params_set_avail_min (playback_handle, sw_params, period_size)) < 0) {
      fprintf (stderr, "cannot set minimum available count (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   if ((err = snd_pcm_sw_params_set_start_threshold (playback_handle, sw_params, period_size)) < 0) {
      fprintf (stderr, "cannot set start mode (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   if ((err = snd_pcm_sw_params (playback_handle, sw_params)) < 0) {
      fprintf (stderr, "cannot set software parameters (%s)\n",
               snd_strerror (err));
      assert(0);
   }
  
  
   if ((err = snd_pcm_prepare (playback_handle)) < 0) {
      fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
               snd_strerror (err));
      assert(0);
   }

   int in_count = snd_pcm_poll_descriptors_count(capture_handle);
   int out_count = snd_pcm_poll_descriptors_count(playback_handle);
   int tot_count = in_count + out_count;
   ufds.resize(tot_count);
   if (snd_pcm_poll_descriptors(capture_handle, &ufds[0], in_count) != in_count)
   {
      cerr << "snd_pcm_poll_descriptors() doesn't match for capture" << endl;
   } 
   if (poll) {
   	for (int i=0;i<in_count;i++)
   	{
                cerr << "setting POLLIN event for desc "<<i<<endl;
      		ufds[i].events = POLLIN ;
   	}
   }
   if (snd_pcm_poll_descriptors(playback_handle, &ufds[in_count], out_count) != out_count)
   {
      cerr << "snd_pcm_poll_descriptors() doesn't match for playback" << endl;
   }
   if (poll)
   {
   	for (int i=0;i<out_count;i++)
   	{
		cerr << "setting POLLOUT event for desc "<<in_count + i<<endl;
      		ufds[in_count+i].events = POLLOUT;
   	}
   }
   cerr << "devices are " << ufds[0].fd << " and " << ufds[1].fd << ", total is " << tot_count << endl;
   /*
   if (poll) {
   ufds[0].revents = POLLIN;
   ufds[1].revents = POLLOUT;
   unsigned short revents=0;
   err = snd_pcm_poll_descriptors_revents(capture_handle, &ufds[0], 2, &revents);
   if (err)
   {
      fprintf (stderr, "cannot call snd_pcm_poll_descriptors_revents: (%s)\n",
               snd_strerror (err));
      assert(0);
   }
   cerr << "revents = "<< revents << endl;
   }
   */
}


AlsaDevice::~AlsaDevice()
{
}

bool AlsaDevice::read(short *pcm, int len)
{
   int err;
   if ((err = snd_pcm_readi (capture_handle, pcm, len)) != len)
   {
      if (err<0)
      {
         //fprintf(stderr, "error %d, EPIPE = %d\n", err, EPIPE);
         if (err == -EPIPE)
         {
            fprintf (stderr, "An overrun has occured, reseting capture\n");
         } else
         {
            fprintf (stderr, "read from audio interface failed (%s)\n",
                     snd_strerror (err));
         }
         if ((err = snd_pcm_prepare (capture_handle)) < 0)
         {
            fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
                     snd_strerror (err));
         }
         if ((err = snd_pcm_start (capture_handle)) < 0)
         {
            fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
                     snd_strerror (err));
         }
      } else {
         fprintf (stderr, "Couldn't read as many samples as I wanted (%d instead of %d)\n", err, len);
      }
      return true;
   }
   //cerr << "read " << err << endl;
   //cerr << "-";
   return false;
}

bool AlsaDevice::write(short *pcm, int len)
{
   int err;
   if ((err = snd_pcm_writei (playback_handle, pcm, len)) != len)
   {
      if (err<0)
      {
         //fprintf(stderr, "error %d, EPIPE = %d\n", err, EPIPE);
         if (err == -EPIPE)
         {
            fprintf (stderr, "An underrun has occured, reseting playback, len=%d\n", len);
         } else
         {
            fprintf (stderr, "write to audio interface failed (%s)\n",
                     snd_strerror (err));
         }
         if ((err = snd_pcm_prepare (playback_handle)) < 0)
         {
            fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
                     snd_strerror (err));
         }
      } else {
         fprintf (stderr, "Couldn't write as many samples as I wanted (%d instead of %d)\n", err, len);
      }
      return true;
   }
   //cerr << "wrote " << err << endl;
   //cerr << "+";
   return false;
}

bool AlsaDevice::captureReady(struct pollfd *pfds, unsigned int nfds)
{
   unsigned short revents=0;
   int err;
   if ((err = snd_pcm_poll_descriptors_revents(capture_handle, pfds, nfds, &revents)) < 0)
   {
      cerr << "error in snd_pcm_poll_descriptors_revents for capture: " << snd_strerror (err) << endl;
      //FIXME: This is a kludge
      return pfds[0].revents & POLLIN;
   }
   //cerr << (revents & POLLERR) << endl;
   return revents & POLLIN;
}

bool AlsaDevice::playbackReady(struct pollfd *pfds, unsigned int nfds)
{
   unsigned short revents=0;
   int err;
   if ((err = snd_pcm_poll_descriptors_revents(playback_handle, pfds, nfds, &revents)) < 0)
   {
      cerr << "error in snd_pcm_poll_descriptors_revents for playback: " << snd_strerror (err) << endl;
      //FIXME: This is a kludge
      return pfds[1].revents & POLLOUT;
   }
   //cerr << (revents & POLLERR) << endl;
   return revents & POLLOUT;
}

void AlsaDevice::start()
{

   cerr << "lead_in = " << lead_in << endl;
   short pcm[lead_in*channels];
   for (int i=0;i<lead_in*channels;i++)
      pcm[i] = 0;
   this->write(pcm, lead_in);
   //this->write(pcm, lead_in);
   snd_pcm_start(capture_handle);
   snd_pcm_start(playback_handle);
}

