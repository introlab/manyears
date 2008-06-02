// Copyright (C) 2006 CSIRO 
// Author: Jean-Marc Valin

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include "SourceInfo.h"

using namespace FD;

class TrackingDecision3D;

DECLARE_NODE(TrackingDecision3D);
/*Node
 *
 * @name TrackingDecision3D
 * @category ManyEars:3D
 * @description Decide which source should be tracked
 *
 * @input_name INPUT
 * @input_type Vector<ObjectRef>
 * @input_description Potential sources
 *
 * @output_name OUTPUT
 * @output_type SourceInfo
 * @output_description Selected source for tracking
 *
END*/

class TrackingDecision3D : public BufferedNode {

   int inputID;
   int outputID;
   
public:
   TrackingDecision3D(std::string nodeName, ParameterSet params)
   : BufferedNode(nodeName, params)
   {
      inputID = addInput("INPUT");
      outputID = addOutput("OUTPUT");
      
      inOrder = true;
   }
   
   void calculate(int output_id, int count, Buffer &out)
   {
      RCPtr<Vector<ObjectRef> > inputValue = getInput(inputID, count);
      
      const Vector<ObjectRef> &in = *inputValue;
      
      if (!in.size())
      {
         out[count] = nilObject;
         return;
      }
      
      //Insert some better logic here
      
      out[count] = in[0];
   }
};

