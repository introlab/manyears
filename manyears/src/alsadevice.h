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

#ifndef ALSADEVICE_H
#define ALSADEVICE_H

#include <vector>
#include <alsa/asoundlib.h>
#include <sys/poll.h>
#include <string>

/**
Represents an ALSA sound device

@author Jean-Marc Valin
*/
class AlsaDevice{
   snd_pcm_t *capture_handle;
   snd_pcm_t *playback_handle;
   int lead_in;
   int channels;
   std::vector<struct pollfd> ufds;
public:
    AlsaDevice(std::string device, int nb_chan, unsigned rate, snd_pcm_uframes_t period_size, int nb_periods, bool poll=false);

    ~AlsaDevice();

    bool read(short *pcm, int len);
    bool write(short *pcm, int len);
    std::vector<struct pollfd> &getPollDevices() {return ufds;}
    bool captureReady(struct pollfd *pfds, unsigned int nfds);
    bool playbackReady(struct pollfd *pfds, unsigned int nfds);
    void start();
};

#endif

