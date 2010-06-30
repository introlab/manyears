/* Copyright (C) 2006-2007 Eric Beaudry, Simon Briere, Dominic Letourneau

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
#ifndef _SOURCE_INFO_H_
#define _SOURCE_INFO_H_

#include "Object.h"
#include "ObjectParser.h"
#include <cmath>
#include "misc.h"
#include <string>
#include <string.h>

class SourceInfo : public FD::Object {
public:
   float delta[7];
   float x[3];
   float strength;
   int age;
   int interval;
   int eval_count;
   int start_time;
   int source_id;
   double remaining;
   
   bool useAudioStream;
   std::string host;
   int port;
   
   SourceInfo()
   {
	   memset(delta,0,sizeof(float) * 7);
	   memset(x,0,sizeof(float) * 3);
	   strength = 0;
	   age = 0;
	   interval = 0;
	   eval_count = 0;
	   start_time = 0;
	   source_id = -1;
	   remaining = 0;
	   useAudioStream = false;
	   host = "";
	   port = 0;
   }
   
   SourceInfo(const SourceInfo &cpy)
   {
	   memcpy(delta,cpy.delta,7*sizeof(float));
	   memcpy(x,cpy.x,3*sizeof(float));
	   strength = cpy.strength;
	   age = cpy.age;
	   interval = cpy.interval;
	   eval_count = cpy.eval_count;
	   start_time = cpy.start_time;
	   source_id = cpy.source_id;
	   remaining = cpy.remaining;
	   useAudioStream = cpy.useAudioStream;
	   host = cpy.host;
	   port = cpy.port;
   }
   
   float getX(unsigned int index) const
   {
	   if (index < 3)
		   return x[index];
	   else
		   return -1;
   }

   float angle(const float *y) const
   {
      float prod = x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
      if (prod < -1)
         prod=-1;
      if (prod>1)
         prod = 1;
      return std::acos(prod);
   }
   float angle(const SourceInfo &s) const {return std::acos(x[0]*s.x[0] + x[1]*s.x[1] + x[2]*s.x[2]);}
   void printOn(std::ostream &out=std::cout) const;

   FD::ObjectRef clone()
   {
      return FD::ObjectRef(new SourceInfo(*this));
   }
   
   void readFrom(std::istream &in);

   friend std::istream &operator >> (std::istream &in, SourceInfo &cell);
};

inline std::istream &operator >> (std::istream &in, SourceInfo &src)
{
   if (!FD::isValidType(in, "SourceInfo")) return in;
   src.readFrom(in);
   return in;
}

#endif

