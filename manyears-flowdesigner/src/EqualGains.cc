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

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"

using namespace FD;
namespace FD {

class EqualGains;

DECLARE_NODE(EqualGains)
/*Node
 *
 * @name EqualGains
 * @category ManyEars
 * @description Applies a gain to a vector
 *
 * @input_name INPUT
 * @input_type Vector<float> 
 * @input_description Input vector
 *
 * @output_name OUTPUT
 * @output_type Vector<float>
 * @output_description Output vector (after gain)
 *
 * @parameter_name NB_CHANNELS
 * @parameter_type int
 * @parameter_description Number of channels
 *
 * @parameter_name GAINS
 * @parameter_type object
 * @parameter_description Value of the gains
 *
END*/

class EqualGains : public BufferedNode {
   
   int inputID;
   int outputID;
   Vector<float> gains;
   int nb_channels;

public:
   EqualGains(std::string nodeName, ParameterSet params)
   : BufferedNode(nodeName, params)
   {
      inputID = addInput("INPUT");
      outputID = addOutput("OUTPUT");
      gains = object_cast<Vector<float> > (parameters.get("GAINS"));
      nb_channels = dereference_cast<int> (parameters.get("NB_CHANNELS"));
   }

   void calculate(int output_id, int count, Buffer &out)
   {
      ObjectRef inputValue = getInput(inputID, count);

      const Vector<float> &in = object_cast<Vector<float> > (inputValue);
      int inputLength = in.size();

      Vector<float> &output = *Vector<float>::alloc(inputLength);
      out[count] = &output;

      for (int i=0;i<nb_channels;i++)
      {
         for (int j=i;j<inputLength;j+=nb_channels)
         {
            output[j] = in[j]*gains[i];
         }
      }
      //vec_mul_scal(gain, &in[0], &output[0], inputLength);
   }

   NO_ORDER_NODE_SPEEDUP(EqualGains)
      
};

}
