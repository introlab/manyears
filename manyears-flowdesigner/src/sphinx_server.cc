/* Copyright (C) 2002-2004 Jean-Marc Valin

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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
extern "C" {
#include "sphinx2/fbs.h"
}
using namespace std;

class fd_streambuf : public streambuf {
  protected:
   virtual int overflow(int = EOF);
   virtual streamsize xsputn(const char *s, streamsize n);

   virtual int uflow();
   virtual int underflow();
   virtual streamsize xsgetn(char *s, streamsize n);
   virtual int pbackfail(int c);
  public:
   fd_streambuf(int _fd, bool _owner=true);
   ~fd_streambuf() {if (owner) close (fd);}
  protected:
   int fd;
   bool owner;
   bool takeFromBuf;
   char charBuf;
};

class fd_ostream : public ostream {
   fd_streambuf _streambuffer;
  public:
   fd_ostream(int _fd, bool _owner=true)
      : ostream(&_streambuffer)
      , _streambuffer (_fd, _owner)
      {clear();}
};


class fd_istream : public istream {
   fd_streambuf _streambuffer;
  public:
   fd_istream(int _fd, bool _owner=true)
      : istream(&_streambuffer)
      , _streambuffer (_fd, _owner)
      {clear();}
};

class fd_iostream : public iostream {
   fd_streambuf _streambuffer;
  public:
   fd_iostream(int _fd, bool _owner=true)
      : iostream(&_streambuffer)
      , _streambuffer (_fd, _owner)
      {clear();}
};




fd_streambuf::fd_streambuf(int _fd, bool _owner)
   : fd(_fd)
   , owner(_owner)
   , takeFromBuf(false)
{

}

int fd_streambuf::overflow(int c)
{
   unsigned char _c = c;
   //FIXME: How about EOF?
   write(fd, &_c, 1);
   return c;
}


streamsize fd_streambuf::xsputn(const char *s, streamsize n)
{
   return write(fd, s, n);
}

int fd_streambuf::uflow()
{
   if (takeFromBuf)
   {
      takeFromBuf = false;
      return charBuf;
   } else {
      if (read(fd, &charBuf, 1) > 0)
         return charBuf;
      else
         return EOF;
   }
}

int fd_streambuf::underflow()
{
   if (takeFromBuf)
   {
      return charBuf;
   } else
   {
      if (read(fd, &charBuf, 1) <= 0)
      {
         return EOF;
      }
      takeFromBuf = true;
      return charBuf;
   }
}


int fd_streambuf::pbackfail(int c)
{
   if (!takeFromBuf)
   {
      if (c != EOF)
         charBuf = c;

      takeFromBuf = true;
      return charBuf;
   } else {
      return EOF;
   }
}

streamsize fd_streambuf::xsgetn(char *s, streamsize n)
{
   int nbytes = read(fd, s, n);
   if (nbytes > 0)
      return nbytes;
   else
      return EOF;
}



int sock;

void main_process(fd_iostream &io, int fd, int argc, char **argv)
{
   vector<short> frame;
   vector<short> frame_buffer;
   
   fprintf (stderr, "child started\n");
   //fd_iostream io(fd, false);
   ofstream *fout = NULL;
   bool started = false;
   bool ended = false;
   float strength = 1.f;
   int end_pad = 0;
   while (1)
   {
      string command;
      io >> command;
      if (io.eof())
      {
         cerr << "Connection lost" << endl;
         break;
      } else if (command == "close")
      {
         //io << "Closing connection" << endl;
         break;
      } else if (command == "open_file")
      {
         if (fout)
         {
            cerr << "file already open" << endl;
         } else {
            string filename;
            io >> filename;
            fout = new ofstream(filename.c_str());
         }
      } else if (command == "result")
      {
         if (started && !ended)
         {
            ended = true;
            cerr << "got the \"result\" command" << endl;
            //io << "Nothing_recognized" << endl;
            uttproc_end_utt();
            int nb_frames;
            char *result;
            uttproc_result(&nb_frames, &result, 1);
            fprintf (stderr, "RESULT: %s\n", result);
            io << result;
            io.put(0);
            fbs_end();
         }
      } else if (command == "set_lm")
      {
         string lm_name;
         io >> lm_name;
         uttproc_set_lm(lm_name.c_str());
      } else if (command == "src_strength")
      {
         io >> strength;
      } else if (command == "put_frame")
      {
         if (!started)
         {
            started = true;
            fprintf (stderr, "utter start\n");
            uttproc_begin_utt(NULL);
            
            /*frame.resize(320,0);
            for (int i=0;i<320;i++)
               frame[i] = 0;
            uttproc_rawdata(&frame[0], 320, 1);
            uttproc_rawdata(&frame[0], 320, 1);
            uttproc_rawdata(&frame[0], 320, 1);*/
         }
         int frame_size;
         io >> frame_size;
         char ch;
         do 
         {
            io.get(ch);
         } while (ch != '|');
         frame.resize(frame_size);
         io.read((char*)&frame[0], frame_size*sizeof(short));
         /*if (fout)
            (*fout).write((char*)&frame[0], frame_size*sizeof(short));*/
         if (strength > .2)
         {
            end_pad = 0;
         } else {
            end_pad++;
         }
         
         if (end_pad<20)
         {
            uttproc_rawdata(&frame_buffer[0], frame_buffer.size(), 1);
            uttproc_rawdata(&frame[0], frame_size, 1);
            if (fout)
               (*fout).write((char*)&frame_buffer[0], frame_buffer.size()*sizeof(short));
            if (fout)
               (*fout).write((char*)&frame[0], frame_size*sizeof(short));
            frame_buffer.resize(0);
         } else {
            int sz = frame_buffer.size();
            frame_buffer.resize(sz+frame_size);
            for (int i=0;i<frame_size;i++)
               frame_buffer[i+sz] = frame[i];
            //uttproc_rawdata(&frame[0], frame_size, 1);
         }
      } else {
         cerr << "Unknown command: " << command << endl;
      }
   }
   cerr << "Shutting down socket" << endl;
   if (fout)
      delete fout;
   close(fd);
   close (sock);
   if (started && !ended)
   {
      cerr << "Result not reclaimed. Here it is anyway." << endl;
      uttproc_end_utt();
      int nb_frames;
      char *result;
      uttproc_result(&nb_frames, &result, 1);
      fprintf (stderr, "RESULT: %s\n", result);
      fprintf (stderr, "Discarded %d samples\n", frame_buffer.size());
      fbs_end();
   }
   fflush(stderr);
   fflush(stdout);
   exit(0);

}

void sig_chld(int i)
{
   while (1)
   {
      //cerr << "got sigchld\n";
      int rc = waitpid(-1,NULL,WNOHANG);
      if (rc <= 0)
         return;
   }
   
}

void sig_int(int i)
{
   close(sock);
   fbs_end();
   fprintf (stderr, "Shutting down\n");
   exit(0);
}

char *ARGV[] = {"sphinx", "-live",  "-verbose", "1", "-adcin", "TRUE", "-ctloffset", "0", "-ctlcount", "100000000", "-agcemax", "TRUE", "-langwt", "6.5", "-fwdflatlw", "8.5", "-rescorelw", "9.5", "-ugwt", "0.5", "-fillpen", "1e-10", "-silpen", "0.005", "-inspen", "0.65", "-top", "1", "-topsenfrm", "3", "-topsenthresh", "-70000", "-beam", "2e-06", "-npbeam", "2e-06", "-lpbeam", "2e-05", "-lponlybeam", "0.0005", "-nwbeam", "0.0005", "-fwdflat", "FALSE", "-fwdflatbeam", "1e-08", "-fwdflatnwbeam", "0.0003", "-bestpath", "TRUE", "-lmfn", "basecontext.lm", "-dictfn", "master.dic", "-noisedict", "/opt/sphinx/share/sphinx2/model/hmm/6k/noisedict", "-phnfn", "/opt/sphinx/share/sphinx2/model/hmm/6k/phone", "-mapfn", "/opt/sphinx/share/sphinx2/model/hmm/6k/map", "-hmmdir", "/opt/sphinx/share/sphinx2/model/hmm/6k", "-hmmdirlist", "/opt/sphinx/share/sphinx2/model/hmm/6k", "-8bsen", "TRUE", "-sendumpfn", "/opt/sphinx/share/sphinx2/model/hmm/6k/sendump", "-cbdir", "/opt/sphinx/share/sphinx2/model/hmm/6k"};


int main(int argc, char **argv)
{
   struct sockaddr_in addr;
   int port = 2312;

   memset(&addr, 0, sizeof(struct sockaddr));
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(port);


   sock = socket(PF_INET, SOCK_STREAM, 0);
   //int tmp = 1;
   //if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp)))
   //   perror("setsockopt SO_REUSEADDR failed\n");
   //struct linger ling = {1, 10};
   //  setsockopt (sock, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

   signal (SIGINT, sig_int);
   signal (SIGCHLD, sig_chld);
   if (bind (sock, (struct sockaddr *)&addr, sizeof(addr)))
   {
      perror("bind failed");
      return 0;
   }
   
   if (listen(sock, 1))
   {
      perror("listen: ");
      return 0;
   }

   
   fbs_init (sizeof(ARGV)/4, ARGV);
   lm_read ("basecontext.lm", "basecontext", 6.5, .5, .65);
   lm_read ("schmoozing.lm", "schmoozing", 6.5, .5, .65);
   lm_read ("yesno.lm", "yesno", 6.5, .5, .65);
   uttproc_set_lm("basecontext");

   while (1)
   {
      socklen_t len;

      int fd = accept (sock, (struct sockaddr *)&addr, &len);
      
      fd_iostream io(fd, false);
      
      while (1)
      {
         string command;
         io >> command;
         if (io.eof())
         {
            close(fd);
            break;
         }
         if (command == "fork")
         {
            pid_t pid = fork();
         
            if (pid==-1)
            {
               perror("fork failed");
            } else if (pid==0)
            {
               main_process(io, fd, argc, argv);
            }
            close(fd);
            break;
         } else if (command == "close")
         {
            close (fd);
            break;
         } else if (command == "set_lm")
         {
            string lm_name;
            io >> lm_name;
            uttproc_set_lm(lm_name.c_str());
            cerr << "setting lm \"" << lm_name << "\"" << endl;
         } else {
            cerr << "unknown command: " << command << endl;
         }
      }
   }

   close(sock);
}
