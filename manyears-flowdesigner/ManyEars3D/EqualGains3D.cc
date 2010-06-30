// Copyright (C) 2002-2005 Jean-Marc Valin/Universite de Sherbrooke
// All rights reserved

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"

class EqualGains3D;

using namespace FD;

DECLARE_NODE(EqualGains3D)
/*Node
 *
 * @name EqualGains3D
 * @category ManyEars:3D
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

class EqualGains3D : public BufferedNode {
   
   int inputID;
   int outputID;
   Vector<float> gains;
   int nb_channels;

public:
   EqualGains3D(std::string nodeName, ParameterSet params)
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

   NO_ORDER_NODE_SPEEDUP(EqualGains3D)
      
};
