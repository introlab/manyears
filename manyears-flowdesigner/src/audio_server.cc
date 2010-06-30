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
 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */

#include "alsadevice.h"
#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>

#include <sched.h>
#include <sys/mman.h>
#include <bits/mman.h>
#include <signal.h>
#include "alsadevice.cpp"
using namespace std;


#define STEREO_MODE
#if 0
#define RATE 8000
#define NB_CHAN 1
#define NB_SPEAKERS 1
#define FRAME_SIZE 160

#else

#define RATE 48000
#define NB_CHAN 8
#define NB_SPEAKERS 1
#define FRAME_SIZE 1024

#endif

//#define FRAME_SIZE 1024
//#define FRAME_SIZE 8192

#define TOT_SIZE (FRAME_SIZE*NB_CHAN)

#define LEAD_PLAY (60-1)
//#define LEAD_PLAY (480-1)
//#define LEAD_PLAY 0

#define REC_BUFFER_FRAMES 1000
#define PLAY_BUFFER_FRAMES 10

short recBuffer[REC_BUFFER_FRAMES][FRAME_SIZE*NB_CHAN];
short playBuffer[PLAY_BUFFER_FRAMES][FRAME_SIZE*NB_SPEAKERS*2];

bool recConnected=false;
bool playConnected=false;

short recPointer = 0;
short playPointer = 0;

int recProcessed[REC_BUFFER_FRAMES];
int playProcessed[PLAY_BUFFER_FRAMES];

pthread_mutex_t recMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t playMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t recCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t playCond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t playOwner = PTHREAD_MUTEX_INITIALIZER;
bool kick=false;

void *rec_thread(void *data)
{
   int port = 2313;
   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(struct sockaddr_in));
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(port);
   int sock = socket(PF_INET, SOCK_STREAM, 0);
   if (bind (sock, (struct sockaddr *)&addr, sizeof(addr)))
   {
      perror("bind failed");
      exit(1);
   }
   if (listen(sock, 1))
   {
      perror("listen: ");
      exit(1);
   }
   socklen_t len=sizeof(struct sockaddr_in);
  
   while (1)
   {
      cerr <<  "waiting for record client" << endl;
      int fd = accept (sock, (struct sockaddr *)&addr, &len);
      cerr << "record client connected" << endl;
      //accept(...);
      //int fd = 1;
      int localPointer;
      
      int localRecPointer;
      pthread_mutex_lock(&recMutex);
      localRecPointer = recPointer;
      for (int i=0;i<REC_BUFFER_FRAMES;i++)
         recProcessed[i] = true;
      recConnected = true;
      pthread_mutex_unlock(&recMutex);
      
      while (1)
      {
         pthread_mutex_lock(&recMutex);
         while (localRecPointer == recPointer)
         {
            pthread_cond_wait(&recCond, &recMutex);
         }
         pthread_mutex_unlock(&recMutex);
         //int err = write (fd, recBuffer[localRecPointer], FRAME_SIZE * NB_CHAN * sizeof(short));
         int err = send (fd, recBuffer[localRecPointer], FRAME_SIZE * NB_CHAN * sizeof(short), 0);
         pthread_mutex_lock(&recMutex);
         recProcessed[localRecPointer] = true;
         pthread_mutex_unlock(&recMutex);
         //cerr << err << endl;
         if (err <= 0) //if write fails
         {
            pthread_mutex_lock(&recMutex);
            recConnected = false;
            pthread_mutex_unlock(&recMutex);
            close (fd);
            cerr << "rec disconnect" << endl;
            break;
         }
         localRecPointer++;
         if (localRecPointer>=REC_BUFFER_FRAMES)
            localRecPointer=0;
      }
   }
}

void *play_thread(void *data)
{
   int port = 2314;
   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(struct sockaddr_in));
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(port);
   int sock = socket(PF_INET, SOCK_STREAM, 0);
   if (bind (sock, (struct sockaddr *)&addr, sizeof(addr)))
   {
      perror("bind failed");
      exit(1);
   }
   if (listen(sock, 1))
   {
      perror("listen: ");
      exit(1);
   }
   socklen_t len=sizeof(struct sockaddr_in);
   
   while (1)
   {
      cerr <<  "waiting for playback client" << endl;
      int fd = accept (sock, (struct sockaddr *)&addr, &len);
      kick=true;
      pthread_mutex_lock(&playOwner);
      cerr <<  "playback client connected" << endl;
      //accept();
      //int fd = 0;
      int localPlayPointer;
      
      pthread_mutex_lock(&playMutex);
      localPlayPointer = playPointer;
      for (int i=0;i<PLAY_BUFFER_FRAMES;i++)
         playProcessed[i] = true;   
      playConnected = true;
      pthread_mutex_unlock(&playMutex);
      
      while (1)
      {
         pthread_mutex_lock(&playMutex);
         while (localPlayPointer == playPointer)
         {
            pthread_cond_wait(&playCond, &playMutex);
         }
         pthread_mutex_unlock(&playMutex);
#ifdef STEREO_MODE_
         //int err = read (fd, playBuffer[localPlayPointer], FRAME_SIZE * NB_SPEAKERS * 2 * sizeof(short));
         int err = recv (fd, playBuffer[localPlayPointer], 2 * FRAME_SIZE * NB_SPEAKERS * sizeof(short), 0);
#else
         int err = recv (fd, playBuffer[localPlayPointer], FRAME_SIZE * NB_SPEAKERS * sizeof(short), 0);
#endif
         pthread_mutex_lock(&playMutex);
         playProcessed[localPlayPointer] = true;
         pthread_mutex_unlock(&playMutex);
         //cerr << err << endl;
         if (err <= 0) //if read fails
         {
            pthread_mutex_lock(&playMutex);
            playConnected = false;
            pthread_mutex_unlock(&playMutex);
            pthread_mutex_unlock(&playOwner);
            close(fd);
            kick=false;
            cerr << "play disconnect" << endl;
            break;
         }
         localPlayPointer++;
         if (localPlayPointer >= PLAY_BUFFER_FRAMES)
            localPlayPointer = 0;
      }
   }
}

void *music_thread(void *data)
{

   //SOCKET INIT
   int port = 2315;
   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(struct sockaddr_in));
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(port);
   int sock = socket(PF_INET, SOCK_STREAM, 0);
   if (bind (sock, (struct sockaddr *)&addr, sizeof(addr)))
   {
      perror("bind failed");
      exit(1);
   }
   if (listen(sock, 1))
   {
      perror("listen: ");
      exit(1);
   }
   socklen_t len=sizeof(struct sockaddr_in);
   
   while (1)
   {
      cerr <<  "waiting for music client" << endl;
      int fd = accept (sock, (struct sockaddr *)&addr, &len);
      pthread_mutex_lock(&playOwner);
      cerr <<  "music client connected" << endl;
      //accept();
      //int fd = 0;
      int localPlayPointer;
      
	
      //GET THE LOCAL PLAY POINTER
      pthread_mutex_lock(&playMutex);
      localPlayPointer = playPointer;
      for (int i=0;i<PLAY_BUFFER_FRAMES;i++)
         playProcessed[i] = true;   
      playConnected = true;
      pthread_mutex_unlock(&playMutex);
      
      while (1)
      {
         pthread_mutex_lock(&playMutex);
         while (localPlayPointer == playPointer)
         {
            pthread_cond_wait(&playCond, &playMutex);
         }
         pthread_mutex_unlock(&playMutex);
#ifdef STEREO_MODE_
         //int err = read (fd, playBuffer[localPlayPointer], FRAME_SIZE * NB_SPEAKERS * 2 * sizeof(short));
         int err = recv (fd, playBuffer[localPlayPointer], 2 * FRAME_SIZE * NB_SPEAKERS * sizeof(short), 0);
#else
         int err = recv (fd, playBuffer[localPlayPointer], FRAME_SIZE * NB_SPEAKERS * sizeof(short), 0);
#endif
         pthread_mutex_lock(&playMutex);
         playProcessed[localPlayPointer] = true;
         pthread_mutex_unlock(&playMutex);
         //cerr << err << endl;
         if (err <= 0 || kick) //if read fails
         {
            pthread_mutex_lock(&playMutex);
            playConnected = false;
            pthread_mutex_unlock(&playMutex);
            pthread_mutex_unlock(&playOwner);
            close(fd);
            cerr << "music disconnect" << endl;
            break;
         }
         localPlayPointer++;
         if (localPlayPointer >= PLAY_BUFFER_FRAMES)
            localPlayPointer = 0;
      }
   }
}



int main(int argc, char *argv[])
{   
   signal (SIGPIPE, SIG_IGN);
   AlsaDevice audio_dev(argv[1], NB_CHAN, RATE, FRAME_SIZE, 2);
   int lead_remain = LEAD_PLAY;
   int play_timeout=10;
   
   struct sched_param param;
   param.sched_priority = sched_get_priority_max(SCHED_FIFO);
   if (sched_setscheduler(0,SCHED_FIFO,&param))
      perror("sched_setscheduler");
         
   if (mlockall(MCL_FUTURE)!=0)
      perror("mlockall");
   
   cerr << "starting threads" << endl;
      
   pthread_t recThread;
   pthread_t playThread;
   pthread_t musicThread;
   pthread_create (&recThread, NULL, rec_thread, NULL);
   pthread_create (&playThread, NULL, play_thread, NULL);
   pthread_create (&musicThread, NULL, music_thread, NULL);

   vector<struct pollfd> ufds = audio_dev.getPollDevices();
   audio_dev.start();

   while (1)
   {
      poll(&ufds[0], ufds.size(), -1);
      if (audio_dev.playbackReady(&ufds[1], ufds.size()-1))
      {
         pthread_mutex_lock(&playMutex);
         short pcm[TOT_SIZE];
         for (int i=0;i<TOT_SIZE;i++)
            pcm[i] = 0;
         
         if (playConnected && playProcessed[playPointer])
         {
            for (int i=0;i<FRAME_SIZE;i++)
            {
#ifdef STEREO_MODE 
	    
               pcm[NB_CHAN*i] = playBuffer[playPointer][NB_SPEAKERS*i];
	       pcm[NB_CHAN*i+1] = playBuffer[playPointer][NB_SPEAKERS*i];
               for (int j=NB_SPEAKERS*2;j<NB_CHAN;j++)
                  pcm[NB_CHAN*i+j] = 0;
#else

	    
               pcm[NB_CHAN*i] = playBuffer[playPointer][NB_SPEAKERS*i];
               pcm[NB_CHAN*i+1] = playBuffer[playPointer][NB_SPEAKERS*i+1];
               pcm[NB_CHAN*i+2] = playBuffer[playPointer][NB_SPEAKERS*i+2];
               for (int j=NB_SPEAKERS;j<NB_CHAN;j++)
                  pcm[NB_CHAN*i+j] = 0;
#endif
            }
            play_timeout=0;
         } else {
            play_timeout++;
         }
         if (playConnected && !playProcessed[playPointer])
         {
            cerr << "socket underrun" << endl;
            //exit(1);
         }
         playProcessed[playPointer] = false;
         playPointer++;
         if (playPointer >= PLAY_BUFFER_FRAMES)
            playPointer = 0;
         if (audio_dev.write(pcm, FRAME_SIZE))
         {
            cerr << "soundcard underrun" << endl;
            //exit(1);
         }
         pthread_cond_broadcast(&playCond);
         pthread_mutex_unlock(&playMutex);
      }
      if (audio_dev.captureReady(&ufds[0], ufds.size()-1))
      {
         pthread_mutex_lock(&recMutex);
         if (audio_dev.read(recBuffer[recPointer], FRAME_SIZE))
         {
            cerr << "soundcard overrun" << endl;
            //exit(1);
         }
         if (play_timeout < 2)
         {
            for (int i=0;i<NB_CHAN*FRAME_SIZE;i++)
               recBuffer[recPointer][i] = 0;
         }
         if (recConnected && !recProcessed[recPointer])
         {
            cerr << "socket overrun" << endl;
            //exit(1);
         }
         recProcessed[recPointer] = false;
         recPointer++;
         if (recPointer >= REC_BUFFER_FRAMES)
            recPointer = 0;
         pthread_cond_broadcast(&recCond);
         pthread_mutex_unlock(&recMutex);
      }
   }


   return EXIT_SUCCESS;
}
