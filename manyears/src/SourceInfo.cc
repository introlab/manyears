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

#include "SourceInfo.h"
#include "Vector.h"
#include "conversion.h"
#include "Exception.h"
#include "vmethod.h"
#include <sstream>

using namespace FD;

//conversion declaration
ObjectRef SourceInfoToStringConversion(ObjectRef in);
ObjectRef VectorObjectRefToString(ObjectRef in);

DECLARE_TYPE(SourceInfo);
DECLARE_TYPE(Vector<SourceInfo>);
REGISTER_CONVERSION(SourceInfo, String, SourceInfoToStringConversion);
REGISTER_CONVERSION(Vector<ObjectRef>, String, VectorObjectRefToString);
REGISTER_VTABLE0(toString, Vector<ObjectRef>, VectorObjectRefToString, 1);

//conversion to string...
ObjectRef SourceInfoToStringConversion(ObjectRef in)
{
    try {
        RCPtr<SourceInfo> info = in;
        std::stringstream temp;
        in->printOn(temp);
        return ObjectRef(new String(temp.str()));                        
    }
    catch(BaseException *e)
    {
        throw e->add(new GeneralException("Unable to cast into SourceInfo",__FILE__,__LINE__));   
    }        
}


ObjectRef VectorObjectRefToString(ObjectRef in)
{
    try {
        RCPtr<Vector<ObjectRef> > info = in;
        std::stringstream temp;
        in->printOn(temp);
        return ObjectRef(new String(temp.str()));                        
    }
    catch(BaseException *e)
    {
        throw e->add(new GeneralException("Unable to cast into SourceInfo",__FILE__,__LINE__));   
    }  
    
}


void SourceInfo::printOn(std::ostream &out) const
{
   out << "<SourceInfo " << std::endl;
   out << "<x " << x[0] << " " << x[1] << " " << x[2] << " >" << std::endl;
   out << "<strength " << strength << " >" << std::endl;
   out << "<age " << age << " >" << std::endl;
   out << "<source_id " << source_id << " >" << std::endl;
   if(useAudioStream) {
      out << "<host " << host.c_str() << " >" << std::endl;
      out << "<port " << port << " >" << std::endl;
   }
   out << ">" << std::endl;
}
