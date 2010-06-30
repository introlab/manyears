// Copyright (C) 2006 CSIRO 
// Author: Jean-Marc Valin

#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include "SourceInfo.h"
#include "Matrix.h"
#include <sstream>

using namespace FD;

class SteeringVector3D;

DECLARE_NODE(SteeringVector3D);
/*Node
 *
 * @name SteeringVector3D
 * @category ManyEars:3D
 * @description Decide which source should be tracked
 *
 * @input_name INPUT
 * @input_type SourceInfo
 * @input_description Selected source for tracking
 *
 * @input_name MIC_POS
 * @input_type Matrix<float>
 * @input_description Microphone position
 *
 * @output_name OUTPUT
 * @output_type Vector<float>
 * @output_description Steering vector (delays) for selected source
 *
 * @output_name STRING
 * @output_type String
 * @output_description Steering vector (delays) for selected source (as string)
 *
END*/

class SteeringVector3D : public BufferedNode {

   int inputID;
   int micID;
   int outputID;
   int stringID;
   
public:
   SteeringVector3D(std::string nodeName, ParameterSet params)
   : BufferedNode(nodeName, params)
   {
      inputID = addInput("INPUT");
      micID = addInput("MIC_POS");
      outputID = addOutput("OUTPUT");
      stringID = addOutput("STRING");
      
      inOrder = true;
   }
   
   void calculate(int output_id, int count, Buffer &out)
   {
      ObjectRef inputValue = getInput(inputID, count);
      
      if (inputValue->isNil())
      {
         out[count] = nilObject;
         return;
      }
      const SourceInfo &src = object_cast<SourceInfo> (inputValue);
      
      FD::RCPtr<Matrix<float> > micValue = getInput(micID, count);
      const FD::Matrix<float> &mat = *micValue;

      int N = mat.nrows();
      FD::Vector<float > &vec = *FD::Vector<float>::alloc(N);
      (*(outputs[outputID].buffer))[count]=&vec;
      FD::String &str = *new String;
      (*(outputs[stringID].buffer))[count]=&str;
      
      std::ostringstream sstr;
      sstr << "begin ";
      for (int i=0;i<N;i++)
      {
         float d1=0, d2=0;
         for (int j=0;j<3;j++)
         {
            d1 += sqr(mat[i][j]-100*src.x[j]);
            d2 += sqr(100*src.x[j]);
         }
         d1 = sqrt(d1);
         d2 = sqrt(d2);
         vec[i] = (d2 - d1)*1.39;
         sstr << int(vec[i])+80 << " ";
      }
      sstr << "end" << std::endl;
      str = sstr.str();
   }
};

