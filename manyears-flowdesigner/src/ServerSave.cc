/* Copyright (C) 2004 Jean-Marc Valin

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
#include <netdb.h>
#include <errno.h>
//#include <string.h>

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include "SourceInfo.h"
#include "sphere.h"
#include "Map.h"
#include <sstream>
#include "stream_wrap.h"

using namespace FD;

class ServerSave;

DECLARE_NODE(ServerSave)
/*Node
 *
 * @name ServerSave
 * @category ManyEars
 * @description Saves all sources to a different file
 *
 * @input_name SOURCES
 * @input_type Map<int,ObjectRef>
 * @input_description Source audio
 *
 * @input_name SRC_POS
 * @input_type Vector<ObjectRef>
 * @input_description Position of sources
 *
 * @output_name OUTPUT
 * @output_type Map<int,ObjectRef>
 * @output_description Same as input
 *
 * @parameter_name BASENAME
 * @parameter_type string
 * @parameter_description Base string for file names
 *
 * @parameter_name HOST
 * @parameter_type string
 * @parameter_description Host where server is running
 *
 * @parameter_name PORT
 * @parameter_type int
 * @parameter_description Port where server is running
 *
 * @parameter_name SAVE_TO_FILE
 * @parameter_type bool
 * @parameter_description Whether to save the separated sources to file
 *
END*/


class ServerSave : public BufferedNode {
   
   int sourcesID;
   int srcPosID;
   int outputID;
   std::string baseName;
   std::string host;
   int port;
   std::map<int, std::iostream*> streams;
   std::map<int, int> start_pos;
   bool saveToFile;
 public:
   ServerSave(std::string nodeName, ParameterSet params)
   : BufferedNode(nodeName, params)
   {
      sourcesID = addInput("SOURCES");
      srcPosID = addInput("SRC_POS");
      outputID = addOutput("OUTPUT");
      baseName = object_cast<String> (parameters.get("BASENAME"));
      host = object_cast<String> (parameters.get("HOST"));
      port = dereference_cast<int> (parameters.get("PORT"));
      if (parameters.exist("SAVE_TO_FILE"))
         saveToFile = dereference_cast<bool> (parameters.get("SAVE_TO_FILE"));
      else
         saveToFile = false;
      inOrder = true;
   }

   ~ServerSave()
   {
      closeFiles();
   }

   void reset()
   {
      closeFiles();
   }
   
   void closeFiles()
   {
     std:: map<int,iostream*>::iterator it = streams.begin();
      while (it != streams.end())
      {
         delete it->second;
         streams.erase(it);
         it = streams.begin();
      }
   }

   std::iostream *openStream()
   {
      struct sockaddr_in addr;
      int sock;
      struct hostent *entp;
      
      if ((sock = socket(PF_INET, SOCK_STREAM, 0))==-1)
      {
         perror("socket creation failed");
         return NULL;
      }

      memset(&addr, 0, sizeof(struct sockaddr));
      
      addr.sin_family = AF_INET;
      
      addr.sin_addr.s_addr = htonl(INADDR_ANY);
      addr.sin_port = htons(0);
      
      //cerr << "connecting to " << host << ":" << port << endl;
      if (bind (sock, (struct sockaddr *)&addr, sizeof(addr)))
      {
         cerr << "errno = " << errno << ", sock = " << sock << endl;
         perror("bind failed");
         return NULL;
      }

      if((entp = gethostbyname(host.c_str())) == NULL) {
         char message[256];
         fprintf(stderr, "Can't get host by name\n");
         return NULL;
      }
      
      memcpy(&addr.sin_addr, entp->h_addr_list[0], entp->h_length);
      
      addr.sin_port = htons(port);
      if (connect (sock, (struct sockaddr *)&addr, sizeof(addr)))
      {
         perror("connect: ");
         return NULL;
      }
      
      //cerr << "connected to server" << endl;
      return new fd_iostream(sock);
      //close(sock);
      
   }

   void calculate(int output_id, int count, Buffer &out)
   {
      ObjectRef input1Value = getInput(sourcesID, count);
      const Map<int,ObjectRef> &src = object_cast<Map<int,ObjectRef> > (input1Value);

      ObjectRef input2Value = getInput(srcPosID, count);
      const Vector<ObjectRef> &src_pos = object_cast<Vector<ObjectRef> > (input2Value);

      vector<int> all_ids;
      Map<int,ObjectRef>::const_iterator it = src.begin();
      while (it != src.end())
      {
         int id = it->first;
         all_ids.push_back(id);
         if (streams.find(id) == streams.end())
         {
            ostringstream name_str;
            name_str << baseName << id << ".sw";
            streams[id] = openStream();
            if (streams[id])
            {
               (*streams[id]) << "fork" << endl;
               if (saveToFile)
                  (*streams[id]) << "open_file " << name_str.str() << endl;
            }
            //streams[id] = new ofstream(name_str.str().c_str());
            start_pos[id] = 0;
         }
         if (streams[id])
         {
            iostream &str = *streams[id];
            const Vector<float> &aud = object_cast<Vector<float> > (it->second);
            short buff[aud.size()];
            int i, k;
            for (k=0,i=start_pos[id];i<aud.size();i+=3,k++)
               buff[k] = short(aud[i]);
            start_pos[id] = i-aud.size();
            int nb_samples = k;
            
            float strength = -1;
            for (int i=0;i<src_pos.size();i++)
            {
               RCPtr<SourceInfo> s_info = src_pos[i];
               if (s_info->source_id == id)
               {
                  strength = s_info->strength;
                  break;
               }
            }
            str << "src_strength " << strength << endl;
            str << "put_frame " << nb_samples << " |";
            /*for (i=0;i<nb_samples;i++)
               str << buff[i] << " ";*/
            str.write((const char *)buff, sizeof(short)*nb_samples);
            str << endl;
         }
         it++;
      }
     std:: map<int, std::iostream*>::iterator stream_it = streams.begin();
      while (stream_it != streams.end())
      {
         int id = stream_it->first;
         bool still_there = false;
         for (int i=0;i<all_ids.size();i++)
         {
            if (all_ids[i] == id)
            {
               still_there = true;
               break;
            }
         }
         if (still_there)
         {
            stream_it++;
         } else if (stream_it->second)
         {
            //cerr << "end of utterance" << endl;
            iostream &str = *stream_it->second;
            str << "result" << endl;
            string res;
            //str >> res;
            char ch;
            do {
               str.get(ch);
               if (ch!=0)
                  res += ch;
            } while (ch != 0);
            cerr << "results for source " << id << " = " << res << endl;
            std::map<int, std::iostream*>::iterator tmp = stream_it;
            tmp++;
            delete stream_it->second;
            streams.erase(stream_it);
            stream_it = tmp;
         }
      }
      
      out[count] = input1Value;
   }

   IN_ORDER_NODE_SPEEDUP(ServerSave)
      
};
