/* Copyright (C) 2006-2007 Jean-Marc Valin, Eric Beaudry, Simon Briere, Dominic Letourneau

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
#include "ObjectParser.h"
#include <string>
#include <iostream>

using namespace std;
using namespace FD;

DECLARE_TYPE(SourceInfo);
DECLARE_TYPE(Vector<SourceInfo>);

void SourceInfo::printOn(std::ostream &out) const
{
   out << "<SourceInfo " << std::endl;
   out << "<x " << x[0] << " " << x[1] << " " << x[2] << " >" << std::endl;
   out << "<strength " << strength << " >" << std::endl;
   out << "<age " << age << " >" << std::endl;
   out << "<source_id " << source_id << " >" << std::endl;
   out << ">" << std::endl;
}

void SourceInfo::readFrom(std::istream &in)
{
string tag;
  
  while (1) {
    char ch;
    in >> ch;
    if (ch == '>') break;
    
    else if (ch != '<') {
      throw new ParsingException ("SourceInfo::readFrom : Parse error: '<' expected");
    }
    in >> tag;
    
    if (tag == "x") {
      in >> x[0] >> x[1] >> x[2];
    }
    else if (tag == "strength") {
      in >> strength;
    }
    else if (tag == "age") {
      in >> age;
    }
    else if (tag == "source_id") {
      in >> source_id;
    }
    else {
      throw new ParsingException ("SourceInfo::readFrom : Unknown argument: " + tag);
    }
    
    if (!in) throw new ParsingException ("SourceInfo::readFrom : Parse error trying to build " + tag);
    
    in >> tag;
    if (tag != ">") 
      throw new ParsingException ("SourceInfo::readFrom : Parse error: '>' expected ");
  }
}
