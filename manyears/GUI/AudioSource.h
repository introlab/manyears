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

#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

#include <map>
#include "SourceInfo.h"

class AudioSource {
    public:

        AudioSource(int _id, float _theta, float _phi, float _strength)
            : id(_id), theta(_theta), phi(_phi), strength (_strength)
        {
                                          
        }
        
        AudioSource()
            : id(-1), theta(0), phi(0), strength (0)
        {

        }         

        AudioSource(const SourceInfo *source)
        {
            if (source)
            {
                id = source->source_id;
                theta = atan2(source->x[1], source->x[0]) * 180.0 / M_PI;
                phi = atan2(source->x[2], source->x[1]) * -180.0 / M_PI;
                strength = source->strength;
            }
            
        }
        
        AudioSource(const AudioSource& cpy)
        {
            id = cpy.id;
            theta = cpy.theta;
            phi = cpy.phi;
            strength = cpy.strength;
            
        }
        
        AudioSource& operator= (const AudioSource &cpy)
        {
            id = cpy.id;
            theta = cpy.theta;
            phi = cpy.phi;
            strength = cpy.strength;
            return *this;
        }
                
        int id;
        float theta;
        float phi;
        float strength;
    
};

#endif
