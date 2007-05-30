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

#include "conj_window.h"
#include <math.h>

void conj_window(float *w, int len)
{
   for (int i=0;i<len;i++)
   {
      float x=4*((float)i)/len;
      int inv=0;
      if (x<1)
      {
      } else if (x<2)
      {
         x=2-x;
         inv=1;
      } else if (x<3)
      {
         x=x-2;
         inv=1;
      } else {
         x=4-x;
      }
      x*=1.9979;
      w[i]=(.5-.5*cos(x))*(.5-.5*cos(x));
      if (inv)
         w[i]=1-w[i];
      w[i]=sqrt(w[i]);
   }
}
