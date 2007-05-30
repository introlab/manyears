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

#include "Object.h"
#include "ObjectParser.h"
#include <cmath>
#include "misc.h"

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
