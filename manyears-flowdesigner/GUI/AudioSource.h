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
#include <QGraphicsEllipseItem>
#include <math.h>
#include <algorithm>

class AudioSource {
    public:

        AudioSource(int _id, float _theta, float _phi, float _strength, float _distance, QGraphicsEllipseItem *_item = NULL)
            : m_id(_id), m_theta(_theta), m_phi(_phi), m_strength (_strength), m_distance(_distance), m_port(-1), m_item(_item)
        {
                                          
        }
        
        AudioSource()
            : m_id(-1), m_theta(0), m_phi(0), m_strength (0), m_distance(0), m_item(NULL), m_port(-1)
        {

        }         

        AudioSource(const SourceInfo *source)
        {
            if (source)
            {
                m_id = source->source_id;
                
				//m_phi = -180.0 * atan2(source->x[2], source->x[1]) / M_PI;
				
				//This is a coordinate change for spartacus
				//float z2 = std::min(1.0f,source->x[2] * source->x[2]);
				m_phi = - 180.0 * atan2(source->x[2],sqrt(source->x[0] * source->x[0] + source->x[1] * source->x[1])) / M_PI;
                m_theta = 180.0 * atan2(source->x[1],source->x[0]) / M_PI;
                m_strength = source->strength;
                m_distance = source->x[0] * source->x[0] + source->x[1] * source->x[1] + source->x[2] * source->x[2];
                m_distance = sqrt(m_distance);
                m_host = source->host;
                m_port = source->port;
            }
            m_item = NULL;
        }
        
        AudioSource(const AudioSource& cpy)
        {
            m_id = cpy.m_id;
            m_theta = cpy.m_theta;
            m_phi = cpy.m_phi;
            m_strength = cpy.m_strength;
            m_item = cpy.m_item; 
            m_distance = cpy.m_distance; 
            m_host = cpy.m_host;
            m_port = cpy.m_port;
        }
        
        AudioSource& operator= (const AudioSource &cpy)
        {
            m_id = cpy.m_id;
            m_theta = cpy.m_theta;
            m_phi = cpy.m_phi;
            m_strength = cpy.m_strength;
            m_item = cpy.m_item;
            m_distance = cpy.m_distance;
            m_host = cpy.m_host;
            m_port = cpy.m_port;
            return *this;
        }
         
        bool operator== (const AudioSource &cpy)
        {
        	return (cpy.m_id == m_id) && (cpy.m_theta == m_theta) && (cpy.m_phi == m_phi) && (cpy.m_strength == m_strength) && (cpy.m_item == m_item) && (cpy.m_distance == m_distance);            
        }
        
        int m_id;
        float m_theta;
        float m_phi;
        float m_strength;
        float m_distance;
        QGraphicsEllipseItem *m_item;
        std::string m_host;
        int m_port;
    
};

#endif
