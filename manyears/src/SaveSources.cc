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

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include "SourceInfo.h"
#include "sphere.h"
#include "Map.h"
#include <sstream>

using namespace FD;

class SaveSources;

DECLARE_NODE(SaveSources)
/*Node
 *
 * @name SaveSources
 * @category ManyEars
 * @description Saves all sources to a different file
 *
 * @input_name SOURCES
 * @input_type Map<int,ObjectRef>
 * @input_description Source audio
 *
 * @output_name OUTPUT
 * @output_type Map<int,ObjectRef>
 * @output_description Same as input
 *
 * @parameter_name BASENAME
 * @parameter_type string
 * @parameter_description Base string for file names
 *
END*/


class SaveSources : public BufferedNode {
   
   int sourcesID;
   int outputID;
   std::string baseName;
   std::map<int, std::ofstream*> streams;
 public:
   SaveSources(std::string nodeName, ParameterSet params)
   : BufferedNode(nodeName, params)
   {
      sourcesID = addInput("SOURCES");
      outputID = addOutput("OUTPUT");
      baseName = object_cast<String> (parameters.get("BASENAME"));
      inOrder = true;
   }

   ~SaveSources()
   {
      closeFiles();
   }

   void reset()
   {
      closeFiles();
   }
   
   void closeFiles()
   {
      std::map<int,std::ofstream*>::iterator it = streams.begin();
      while (it != streams.end())
      {
         delete it->second;
         streams.erase(it);
         it = streams.begin();
      }
   }

   void calculate(int output_id, int count, Buffer &out)
   {
      ObjectRef input1Value = getInput(sourcesID, count);
      const Map<int,ObjectRef> &src = object_cast<Map<int,ObjectRef> > (input1Value);

      Map<int,ObjectRef>::const_iterator it = src.begin();
      while (it != src.end())
      {
         int id = it->first;
         if (streams.find(id) == streams.end())
         {
            ostringstream name_str;
            name_str << baseName << id << ".sw";
            streams[id] = new ofstream(name_str.str().c_str());
         }
         ofstream &str = *streams[id];
         const Vector<float> &aud = object_cast<Vector<float> > (it->second);
         short buff[aud.size()];
         for (int i=0;i<aud.size();i++)
            buff[i] = short(aud[i]);
         str.write((const char *)buff, sizeof(short)*aud.size());
         it++;
      }
      
      out[count] = input1Value;
   }

   IN_ORDER_NODE_SPEEDUP(SaveSources)
      
};
