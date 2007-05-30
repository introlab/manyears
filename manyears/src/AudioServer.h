/* Copyright (C) 2006 Dominic Létourneau

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

#ifndef _AUDIO_SERVER_CLIENT_H_
#define _AUDIO_SERVER_CLIENT_H_

#include <pthread.h>
#include <semaphore.h>
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
#include <list>
#include <map>
#include <vector>
#include "alsadevice.h"
#include <sched.h>
#include <sys/mman.h>
#include <bits/mman.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define STEREO_MODE
#define RATE 48000
#define NB_CHAN 8
#define NB_SPEAKERS 1
#define FRAME_SIZE 1024


#define TOT_SIZE (FRAME_SIZE*NB_CHAN)

#define LEAD_PLAY (60-1)
//#define LEAD_PLAY (480-1)
//#define LEAD_PLAY 0

#define REC_BUFFER_FRAMES 1000
#define PLAY_BUFFER_FRAMES 10

using namespace std;

class AudioServer;

  // AudioServerRecordClient ************************************************************
  class AudioServerRecordClient {

  public:

    friend void* rec_thread(void* data);
    
    AudioServerRecordClient(AudioServer *server, int socket_fd);    
    void terminate();
    bool write_frame();
    size_t frames_size();
    bool push_frame(short* frame, size_t size);
    bool running();
    bool post(int num = 1);
    bool wait();
    bool lock(bool blocking = true);
    bool unlock(bool blocking = true);
    int getSocket();
    ~AudioServerRecordClient();
        
  protected:
    pthread_mutex_t m_mutex;
    list<pair<short*, size_t> > m_frames;    
    pthread_t m_thread;
    AudioServer *m_server;
    sem_t m_semaphore;
    int m_socket;
    bool m_running;
    
  };

  // AudioServerPlaybackClient ************************************************************
  class AudioServerPlaybackClient {
  public:
    friend void* play_thread(void* data);
    AudioServerPlaybackClient(AudioServer *server, int client_fd);
    ~AudioServerPlaybackClient();
    short* getFrame(size_t &size);
    bool running();
    virtual void terminate();
    bool lock(bool blocking = true);
    bool unlock(bool blocking = true);
    bool post(int num = 1);
    bool wait();
    virtual bool read();
    
  protected:
        
    AudioServer *m_server;
    int m_socket;
    bool m_running;
    short m_gain;
    int localPlayPointer;
    pthread_mutex_t m_mutex;
    sem_t m_semaphore;
    pthread_t m_thread;
    list<pair<short*, size_t> > m_frames;
    short playBuffer[PLAY_BUFFER_FRAMES][FRAME_SIZE*NB_SPEAKERS*2];
  };
  
  class AudioServerStereoPlaybackClient : public AudioServerPlaybackClient
  {
	public:
	AudioServerStereoPlaybackClient(AudioServer* server, int client_fd);
	virtual void terminate();
	virtual bool read();
  };

// AudioServer ************************************************************
class AudioServer {

public:


  AudioServer(const char* devName);

  ~AudioServer();

  void notifyTerminate(AudioServerRecordClient *rClient);
  void notifyTerminate(AudioServerPlaybackClient *pClient);
  void notifyTerminate(AudioServerStereoPlaybackClient *pClient);
  void run();
  
protected:

  int listen_rec_client(); 
  int listen_play_client();
  int listen_stereo_play_client();
  void start_rec_thread(int socket);
  void start_play_thread(int socket);
  void start_stereo_play_thread(int socket);
  void init ();
  bool init_audio(const char* devName);
  void poll_audio();
  void lockPlayMutex();
  void unlockPlayMutex();
  void lockRecMutex();
  void unlockRecMutex();
  int getNumRecClients();
  int getNumPlayClients();


  AlsaDevice *m_alsaDevice;
  vector<struct pollfd> m_ufds;  
  pthread_mutex_t m_recMutex;
  pthread_mutex_t m_playMutex;
  short recPointer;
  //short playPointer;
  int m_recordPort;
  int m_playPort;
  int m_stereoPlayPort;
  struct sockaddr_in m_recAddr;
  struct sockaddr_in m_playAddr;
  struct sockaddr_in m_stereoPlayAddr;
  int m_recSocket;
  int m_playSocket;
  int m_stereoPlaySocket;
  list<AudioServerRecordClient*> m_recClients;
  list<AudioServerPlaybackClient*> m_playClients;
  list<AudioServerStereoPlaybackClient*> m_stereoPlayClients;
  int recTimeout;

  short recBuffer[REC_BUFFER_FRAMES][FRAME_SIZE*NB_CHAN];
};



#endif
