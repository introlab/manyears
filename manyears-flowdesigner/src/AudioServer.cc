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

#include "AudioServer.h"
#include <limits.h>
#include <algorithm>
// Record thread ******************************************************************************
void *rec_thread(void *data)
{
  cerr<<"rec_thread started with data : "<<data<<endl;
  if (data) {
    AudioServerRecordClient *rClient = reinterpret_cast<AudioServerRecordClient*>(data);
    if (rClient) {
    	while (rClient->running()) {
      		rClient->wait();
      		rClient->write_frame();
    	}    

    	rClient->terminate();
    }
  }
  cerr<<"rec_thread finished with data : "<<data<<endl;
  pthread_exit(NULL);
}

// Play thread ********************************************************************************
void *play_thread(void *data)
{
  cerr<<"play_thread started with data : "<<data<<endl;
  if (data)
  {
  	AudioServerPlaybackClient *pClient = reinterpret_cast<AudioServerPlaybackClient*>(data);
	if (pClient) {
		while(pClient->running()) {
			pClient->wait();
			if (!pClient->read()) break;
		}		
	}
	pClient->terminate();
  }
  cerr<<"play_thread finished with data : "<<data<<endl;
  pthread_exit(NULL);
}




  AudioServer::AudioServer(const char* devName)
    : m_alsaDevice(NULL), recPointer(0), recTimeout(0) {


    if(!init_audio(devName)) {
      perror("AudioServer::AudioServer unable to open audio device");
      exit(1);
    }

    pthread_mutex_init(&m_recMutex,NULL);
    pthread_mutex_init(&m_playMutex,NULL);
    init();
  }
  

  AudioServer::~AudioServer() {
    
    cerr<<"~AudioServer"<<endl;

  }

  void AudioServer::notifyTerminate(AudioServerRecordClient *rClient) {
    lockRecMutex();
    m_recClients.remove(rClient);
    unlockRecMutex();
    delete rClient;
  }

  void AudioServer::notifyTerminate(AudioServerPlaybackClient *pClient) {
    lockPlayMutex();
    m_playClients.remove(pClient);
    unlockPlayMutex();
    delete pClient;
  }

  void AudioServer::notifyTerminate(AudioServerStereoPlaybackClient *pClient) {
    lockPlayMutex();
    m_stereoPlayClients.remove(pClient);
    unlockPlayMutex();
    delete pClient;
  }

  void AudioServer::run() {
   cerr<<"AudioServer::run()"<<endl;
    while (1) {

      //read from sound card
      //read & write from clients
      poll_audio();

      //get new record clients
      int socket_rec = listen_rec_client();
      if (socket_rec > 0) {
	start_rec_thread(socket_rec);
      }

      //get new playback clients (mono)	
      int socket_play = listen_play_client();
      if (socket_play > 0) {
	start_play_thread(socket_play);
      }

      //get new playback clients (stereo)
      int socket_stereo_play = listen_stereo_play_client();
      if (socket_stereo_play > 0) {
	start_stereo_play_thread(socket_stereo_play);
      }
    }
  }
  

  int AudioServer::getNumRecClients()
  {
	lockRecMutex();
	int num = m_recClients.size();
        unlockRecMutex();
	return num;
  }
 
  int AudioServer::getNumPlayClients()
  {
	lockPlayMutex();
	int num = m_playClients.size() + m_stereoPlayClients.size();
        unlockPlayMutex();
	return num;
  }

 int AudioServer::listen_rec_client() {
    //cerr<<"AudioServer::listen_rec_client()"<<endl;
    socklen_t len=sizeof(struct sockaddr_in);
    struct sockaddr_in addr;
    int client_socket = accept (m_recSocket, (struct sockaddr *)&addr, &len); 

   if (client_socket > 0) {
	cerr <<"AudioServer::listen_rec_client() got socket :"<<client_socket<<endl;
   }
   
    return client_socket;
  }
  
 int AudioServer::listen_play_client() {
    //cerr<<"AudioServer::listen_rec_client()"<<endl;
    socklen_t len=sizeof(struct sockaddr_in);
    struct sockaddr_in addr;
    int client_socket = accept (m_playSocket, (struct sockaddr *)&addr, &len); 

   if (client_socket > 0) {
	cerr <<"AudioServer::listen_play_client() got socket :"<<client_socket<<endl;
   }
   
    return client_socket;
  }
 
  int AudioServer::listen_stereo_play_client() {
    //cerr<<"AudioServer::listen_rec_client()"<<endl;
    socklen_t len=sizeof(struct sockaddr_in);
    struct sockaddr_in addr;
    int client_socket = accept (m_stereoPlaySocket, (struct sockaddr *)&addr, &len); 

    if (client_socket > 0) {
	cerr <<"AudioServer::listen_stero_play_client() got socket :"<<client_socket<<endl;
    }
   
    return client_socket;
  }


  void AudioServer::start_rec_thread(int client_socket) {

    if (client_socket >= 0) 
    {
      cerr << "AudioServer::start_rec_thread ->record client connected" << endl;
      lockRecMutex();
      m_recClients.push_back(new AudioServerRecordClient(this,client_socket));
      unlockRecMutex();
    }
  }

  void AudioServer::start_play_thread(int client_socket) {

    if (client_socket >= 0) 
    {
      cerr << "AudioServer::start_play_thread ->playback (mono) client connected" << endl;
      lockPlayMutex();
      m_playClients.push_back(new AudioServerPlaybackClient(this,client_socket));
      unlockPlayMutex();
    }
  }

  void AudioServer::start_stereo_play_thread(int client_socket) {

    if (client_socket >= 0) 
    {
      cerr << "AudioServer::start_stereo_play_thread ->playback (stereo) client connected" << endl;
      lockPlayMutex();
      m_stereoPlayClients.push_back(new AudioServerStereoPlaybackClient(this,client_socket));
      unlockPlayMutex();
    }
  }

  void AudioServer::init () {
    cerr<<"AudioServer::init()"<<endl;

    //record socket ...
    m_recordPort = 2313;  
    memset(&m_recAddr, 0, sizeof(struct sockaddr_in));
    m_recAddr.sin_family = AF_INET;
    m_recAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_recAddr.sin_port = htons(m_recordPort);
    m_recSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (bind (m_recSocket, (struct sockaddr *)&m_recAddr, sizeof(m_recAddr)))
    {
	perror("AudioServer::init() -> bind failed for record socket");
	exit(1);
    }

    if (listen(m_recSocket,25))
    {
	perror("AudioServer::init() -> listen failed for record socket");
	exit(1);
    }

    //SET SOCKET TO NON BLOCKING
    long flags;
    if (-1 == (flags = fcntl(m_recSocket, F_GETFL, 0))) 
    {
	flags = 0;
    } 
    fcntl(m_recSocket, F_SETFL, flags | O_NONBLOCK);  

    //playback socket
    m_playPort = 2314;
    memset(&m_playAddr, 0, sizeof(struct sockaddr_in));
    m_playAddr.sin_family = AF_INET;
    m_playAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_playAddr.sin_port = htons(m_playPort);
    m_playSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (bind (m_playSocket, (struct sockaddr *)&m_playAddr, sizeof(m_playAddr)))
    {
	perror("AudioServer::init() -> bind failed for play socket");
	exit(1);
    }

    if (listen(m_playSocket,25))
    {
	perror("AudioServer::init() -> listen failed for play socket");
	exit(1);
    }

    //SET SOCKET TO NON BLOCKING
    //long flags;
    if (-1 == (flags = fcntl(m_playSocket, F_GETFL, 0))) 
    {
	flags = 0;
    } 
    fcntl(m_playSocket, F_SETFL, flags | O_NONBLOCK);

    //stereo playback socket
    m_stereoPlayPort = 2315;
    memset(&m_stereoPlayAddr, 0, sizeof(struct sockaddr_in));
    m_stereoPlayAddr.sin_family = AF_INET;
    m_stereoPlayAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_stereoPlayAddr.sin_port = htons(m_stereoPlayPort);
    m_stereoPlaySocket = socket(PF_INET, SOCK_STREAM, 0);
    if (bind (m_stereoPlaySocket, (struct sockaddr *)&m_stereoPlayAddr, sizeof(m_stereoPlayAddr)))
    {
	perror("AudioServer::init() -> bind failed for stereo play socket");
	exit(1);
    }

    if (listen(m_stereoPlaySocket,25))
    {
	perror("AudioServer::init() -> listen failed for stereo play socket");
	exit(1);
    }

    //SET SOCKET TO NON BLOCKING
    //long flags;
    if (-1 == (flags = fcntl(m_stereoPlaySocket, F_GETFL, 0))) 
    {
	flags = 0;
    } 
    fcntl(m_stereoPlaySocket, F_SETFL, flags | O_NONBLOCK);

  }


  bool AudioServer::init_audio(const char* devName) {
    cerr<<"AudioServer::init_audio : "<<devName<<endl;
    m_alsaDevice = new AlsaDevice(devName, NB_CHAN, RATE, FRAME_SIZE, 2,true);
    m_ufds = m_alsaDevice->getPollDevices();
    m_alsaDevice->start();
    return true;
  }

  void AudioServer::poll_audio() {

    //audio output data (NB_CHANNELS * FRAME_SIZE)
    static short pcm[TOT_SIZE];

    //poll timeout = infinite (-1)
    poll(&m_ufds[0], m_ufds.size(), -1);


    //playback ready?
    if (m_alsaDevice->playbackReady(&m_ufds[1], m_ufds.size()-1)) {

	    //zero playback data
	    memset(pcm,0,TOT_SIZE * sizeof(short));


	    lockPlayMutex();
	
	    vector<short*> mono_play_data;
	    vector<size_t> mono_play_size;	  
	    vector<short*> stereo_play_data;
	    vector<size_t> stereo_play_size;

	    if (m_playClients.empty() && m_stereoPlayClients.empty()) {
		 recTimeout = max(0,--recTimeout);
	    }
	    else {
	    	recTimeout = 2;
	    }
	    //get audio (mono)
	    for (list<AudioServerPlaybackClient*>::iterator iter = m_playClients.begin();
	       		iter != m_playClients.end(); iter++) {			
			//get audio
			size_t size = 0;
			short* data = (*iter)->getFrame(size);
			mono_play_data.push_back(data);
			mono_play_size.push_back(size);	
	    }

	    //get audio (stereo)
	    for (list<AudioServerStereoPlaybackClient*>::iterator iter = m_stereoPlayClients.begin();
	       		iter != m_stereoPlayClients.end(); iter++) {			
			//get audio
			size_t size = 0;
			short* data = (*iter)->getFrame(size);
			stereo_play_data.push_back(data);
			stereo_play_size.push_back(size);	
	    }

	    //mixing audio
            for (int i=0;i<FRAME_SIZE;i++)
            {
	       int left_value = 0;
	       int right_value = 0;
	       //iterate for all playback client data (mono)
	       for(int j = 0; j < mono_play_data.size(); j++) {
			
			if (mono_play_size[j] > i) {
				left_value += mono_play_data[j][i];
				right_value = left_value; 	
			}
		}
		//iterate for all playback client data (stereo)
		for(int j = 0; j < stereo_play_data.size(); j++) {
			
			if (stereo_play_size[j] > i * 2) {
				left_value += stereo_play_data[j][i*2];
				right_value += stereo_play_data[j][i*2 +1]; 	
			}
		}

		//apply saturation
		if (left_value < SHRT_MIN) {
			left_value = SHRT_MIN;
		}
		else if (left_value > SHRT_MAX) {
			left_value = SHRT_MAX;
		}
		if (right_value < SHRT_MIN) {
			right_value = SHRT_MIN;
		}
		else if (right_value > SHRT_MAX) {
			right_value = SHRT_MAX;
		}
		                          
               pcm[NB_CHAN*i] = (short)left_value; //playBuffer[playPointer][NB_SPEAKERS*i];
	       pcm[NB_CHAN*i+1] = (short)right_value; //playBuffer[playPointer][NB_SPEAKERS*i];		
            }

	   //post to get a new frame from clients
 	   for (list<AudioServerPlaybackClient*>::iterator iter = m_playClients.begin();
	       		iter != m_playClients.end(); iter++)
	   {  		
		(*iter)->post();
	   }
 	   for (list<AudioServerStereoPlaybackClient*>::iterator iter = m_stereoPlayClients.begin();
	       		iter != m_stereoPlayClients.end(); iter++)
	   {  		
		(*iter)->post();
	   }
	   unlockPlayMutex();

	 //write audio
         if (m_alsaDevice->write(pcm, FRAME_SIZE))
         {
            cerr << "AudioServer::poll_audio() -> soundcard underrun (write)" << endl;
         }
    }

    //capture ready?
    if (m_alsaDevice->captureReady(&m_ufds[0], m_ufds.size()-1)) {
      
      if (m_alsaDevice->read(recBuffer[recPointer], FRAME_SIZE)) {
	cerr << "AudioServer::poll_audio -> soundcard overrun (read)" << endl;
	//exit(1);
      }

      //if playback, zero input
      if (recTimeout > 0) {
	  memset(recBuffer[recPointer],0, FRAME_SIZE * NB_CHAN);
      }

      //send audio to clients
      lockRecMutex();
      for (list<AudioServerRecordClient*>::iterator iter = m_recClients.begin();
	   iter != m_recClients.end(); iter++) {
	(*iter)->push_frame(&recBuffer[recPointer][0],FRAME_SIZE * NB_CHAN);
      }
      unlockRecMutex();


      //increment rec pointer
      recPointer++;
      if (recPointer >= REC_BUFFER_FRAMES) {
            recPointer = 0;
      }

    }
  }

  void AudioServer::lockPlayMutex() {
    pthread_mutex_lock(&m_playMutex);
  }
  
  void AudioServer::unlockPlayMutex() {
    pthread_mutex_unlock(&m_playMutex);
  }
  
  void AudioServer::lockRecMutex() {
    pthread_mutex_lock(&m_recMutex);
  }
  
  void AudioServer::unlockRecMutex() {
    pthread_mutex_unlock(&m_recMutex);
    
  }
  

// AudioServerRecordClient *******************************************************************************
    
    AudioServerRecordClient::AudioServerRecordClient(AudioServer *server, int socket_fd)
      : m_server(server), m_socket(socket_fd), m_running(false) {

      //init semaphore
      if (sem_init (&m_semaphore, 0, 0) == -1) {
	cerr << "AudioServerRecordClient(AudioServer &server, int socket_fd) --> Init semaphore failed" << endl;
	exit(1);
      }

      //init mutex
      pthread_mutex_init(&m_mutex,NULL);

      //start thread
      m_running = true;
      pthread_create (&m_thread, NULL, rec_thread, this);
      
    }
    
    void AudioServerRecordClient::terminate() {
      cerr << "AudioServerRecordClient::terminate()"<<endl;
      m_server->notifyTerminate(this);
    }

    bool AudioServerRecordClient::write_frame() 
    {
	if (m_running) {
      
		lock();
		short *data = m_frames.front().first;
		size_t size = m_frames.front().second;
		m_frames.pop_front();
		unlock();

		//SEND FRAME TO SOCKET	
		int err = send(m_socket,(char*) data, size * sizeof(short),0);

		//write failed?
		if (err <= 0) {
			perror("AudioServerRecordClient::write_frames() : send error");
			//this will wake up the tread and end the thread			
			m_running = false;
			post();
			return false;
		}	
		return true;
      	}
      	else 
      	{
      		return false;
      	}      
    }
    
    size_t AudioServerRecordClient::frames_size() {
      
      size_t size;
      
      lock();
      size = m_frames.size();
      unlock();
      return size;
    }

    bool AudioServerRecordClient::push_frame(short* frame, size_t size) {

      lock();

      //todo : verify socket overflow
      if (find(m_frames.begin(), m_frames.end(), make_pair(frame,size)) == m_frames.end())
      {         
	    m_frames.push_back(make_pair(frame,size));
      }
      else
      {	
	    cerr<<"AudioServerRecordClient::push_frame(short* frame, size_t size) - socket underrun"<<endl;	
      }

      unlock();
      post();

      return true;
    }

    bool AudioServerRecordClient::running() {
      return m_running;
    }

    bool AudioServerRecordClient::post(int num)
    {
      sem_post(&m_semaphore);
      return true;
    }

    bool AudioServerRecordClient::wait()
    {
      sem_wait(&m_semaphore);
      return true;
    }

    bool AudioServerRecordClient::lock(bool blocking) {
      pthread_mutex_lock(&m_mutex);
      return true;
    }

    bool AudioServerRecordClient::unlock(bool blocking) {
      pthread_mutex_unlock(&m_mutex);       
      return true;
    }

    int AudioServerRecordClient::getSocket() {
      return m_socket;
    }

    AudioServerRecordClient::~AudioServerRecordClient()
    {
      cerr<<"~AudioServerRecordClient()"<<endl;
      m_running = false;
      post();
      void *arg;
      cerr<<"~AudioServerRecordClient() join thread"<<endl;
      pthread_join(m_thread,&arg);
      pthread_mutex_destroy(&m_mutex);
      sem_destroy(&m_semaphore);
      close(m_socket);
      cerr<<"~AudioServerRecordClient() done!"<<endl;
    }
// PLAYBACK THREAD *********************************************************************************
AudioServerPlaybackClient::AudioServerPlaybackClient(AudioServer *server, int client_fd)
	:m_server(server), m_socket(client_fd), m_running(false), m_gain(1), localPlayPointer(0)
{


      //init semaphore
      if (sem_init (&m_semaphore,0, PLAY_BUFFER_FRAMES) == -1) {
	cerr << "AudioServerRecordClient(AudioServer &server, int socket_fd) --> Init semaphore failed" << endl;
	exit(1);
      }

      //init mutex
      pthread_mutex_init(&m_mutex,NULL);

      //start thread
      m_running = true;
      pthread_create (&m_thread, NULL, play_thread, this);

}

AudioServerPlaybackClient::~AudioServerPlaybackClient()
{
      cerr<<"~AudioServerPlaybackClient()"<<endl;
      m_running = false; 
      post();    
      void *arg;
      cerr<<"~AudioServerPlaybackClient() join thread"<<endl;
      pthread_join(m_thread,&arg);
      pthread_mutex_destroy(&m_mutex);
      sem_destroy(&m_semaphore);
      close(m_socket);
      cerr<<"~AudioServerPlaybackClient() done!"<<endl;
}


short* AudioServerPlaybackClient::getFrame(size_t &size) {

	pair<short*,size_t> element = make_pair((short*)NULL,0);
	lock();
	if (m_frames.size() > 0) {
		element = m_frames.front();
		m_frames.pop_front();
        }
	unlock();
        //post should be done by server
	//post();
	//cerr<<"AudioServerPlaybackClient::getFrame(size_t &size)" << element.second << " " <<element.first<<endl;
 	size = element.second;
	return element.first;
}

bool AudioServerPlaybackClient::running() {
	return m_running;
}

void AudioServerPlaybackClient::terminate() {
	m_server->notifyTerminate(this);
}

    bool AudioServerPlaybackClient::lock(bool blocking) {
      pthread_mutex_lock(&m_mutex);
      return true;
    }

    bool AudioServerPlaybackClient::unlock(bool blocking) {
      pthread_mutex_unlock(&m_mutex);       
      return true;
    }
        
bool AudioServerPlaybackClient::read() {
	//get data (mono)
	int size = recv (m_socket, playBuffer[localPlayPointer], FRAME_SIZE * NB_SPEAKERS * sizeof(short), 0);
	
	//if read fails
	if (size <= 0) {
	    perror("AudioServerPlaybackClient::write_frames() : recv error");
	    //this will end the thread
	    post();
	    m_running = false;
	    return false;	
	}

	lock();
	//todo apply gain ?

	m_frames.push_back(make_pair(&playBuffer[localPlayPointer][0],size / sizeof(short)));
	unlock();

	localPlayPointer++;
        if (localPlayPointer >= PLAY_BUFFER_FRAMES)
	{
            localPlayPointer = 0;
	}
	return true;
}

bool AudioServerPlaybackClient::post(int num)
{
	sem_post(&m_semaphore);
	return true;
}

bool AudioServerPlaybackClient::wait()
{
	sem_wait(&m_semaphore);
	return true;
}

// AudioServerStereoPlaybackClient ****************************************************
AudioServerStereoPlaybackClient::AudioServerStereoPlaybackClient(AudioServer *server, int client_fd)
	: AudioServerPlaybackClient(server,client_fd)
{
	//NOTHING TO DO!
}

bool AudioServerStereoPlaybackClient::read()
{
	//get data (stereo)
	int size = recv (m_socket, playBuffer[localPlayPointer], 2 * FRAME_SIZE * NB_SPEAKERS * sizeof(short), 0);
	
	//if read fails
	if (size <= 0) {
	    perror("AudioServerPlaybackClient::write_frames() : recv error");
	    //this will end the thread
	    post();
	    m_running = false;
	    return false;	
	}

	lock();
	//todo apply gain ?

	m_frames.push_back(make_pair(&playBuffer[localPlayPointer][0],size / sizeof(short)));
	unlock();

	localPlayPointer++;
        if (localPlayPointer >= PLAY_BUFFER_FRAMES)
	{
            localPlayPointer = 0;
	}
	return true;
}

void AudioServerStereoPlaybackClient::terminate() {
	m_server->notifyTerminate(this);
}
