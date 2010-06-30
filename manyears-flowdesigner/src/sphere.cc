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

#include "sphere.h"
#include <iostream>

SphereQuant sphere(4);

Triangle::Triangle(SphereQuant *_sphere, const Point &p0, const Point &p1, const Point &p2)
   : sphere (_sphere)
     , center (p0, p1, p2)
     , leaf(true)
{
   points[0] = p0;
   points[1] = p1;
   points[2] = p2;
   for (int i=0;i<4;i++)
      children[i] = NULL;
}

/*Triangle::Triangle(SphereQuant *_sphere, const Point p[3])
   : sphere (_sphere)
{
   for (int i=0;i<3;i++)
      points[i] = p[i];
   for (int i=0;i<4;i++)
      children[i] = NULL;

      }*/

void Triangle::split(int levels)
{
   if (!levels)
   {
      sphere->addTriangle(this);
      for (int i=0;i<3;i++)
         point_id[i] = sphere->addPoint(points[i]);
      //sphere->addPoint(center);
      leaf = true;
      return;
   }

   leaf = false;
   Point p01(points[0], points[1]);
   Point p02(points[0], points[2]);
   Point p12(points[1], points[2]);

   children[0] = new Triangle(sphere, p01, p02, p12);
   children[1] = new Triangle(sphere, p01, p02, points[0]);
   children[2] = new Triangle(sphere, p01, p12, points[1]);
   children[3] = new Triangle(sphere, p02, p12, points[2]);

   for (int i=0;i<4;i++)
      children[i]->split(levels-1);
}

Triangle *Triangle::search(const Point &p)
{
   Triangle *tri = children[0];
   float best_dist = children[0]->center.dist(p);
   for (int i=1;i<4;i++)
   {
      float dist = children[i]->center.dist(p);
      if (dist < best_dist)
      {
         best_dist = dist;
         tri = children[i];
      }
   }
   if (tri->leaf)
      return tri;
   else 
      return tri->search(p);
}



SphereQuant::SphereQuant(int levels)
{
   Point p[12];
   float sint = 0.4472136f;
   float cost = 0.8944272f;
   p[0].set(0,0,1);
   p[11].set(0,0,-1);
   for (int i=0;i<5;i++)
   {
      float x,y,z;
      z = sint;
      y = cost*cos(2*M_PI*i/5);
      x = cost*sin(2*M_PI*i/5);
      p[i+1].set(x,y,z);
      p[i+6].set(-x,-y,-z);
   }
   
   children[0] = new Triangle(this, p[0], p[1], p[2]);
   children[1] = new Triangle(this, p[0], p[2], p[3]);
   children[2] = new Triangle(this, p[0], p[3], p[4]);
   children[3] = new Triangle(this, p[0], p[4], p[5]);
   children[4] = new Triangle(this, p[0], p[5], p[1]);

   children[5] = new Triangle(this, p[9], p[1], p[2]);
   children[6] = new Triangle(this, p[10], p[2], p[3]);
   children[7] = new Triangle(this, p[6], p[3], p[4]);
   children[8] = new Triangle(this, p[7], p[4], p[5]);
   children[9] = new Triangle(this, p[8], p[5], p[1]);

   children[10] = new Triangle(this, p[4], p[6], p[7]);
   children[11] = new Triangle(this, p[5], p[7], p[8]);
   children[12] = new Triangle(this, p[1], p[8], p[9]);
   children[13] = new Triangle(this, p[2], p[9], p[10]);
   children[14] = new Triangle(this, p[3], p[10], p[6]);

   children[15] = new Triangle(this, p[11], p[6], p[7]);
   children[16] = new Triangle(this, p[11], p[7], p[8]);
   children[17] = new Triangle(this, p[11], p[8], p[9]);
   children[18] = new Triangle(this, p[11], p[9], p[10]);
   children[19] = new Triangle(this, p[11], p[10], p[6]);

   for (int i=0;i<20;i++)
   {
      children[i]->split(levels);
   }
   

   /*for (int i=0;i<points.size();i++)
   {
      cout << points[i].x[0] << " " << points[i].x[1] << " " << points[i].x[2] << endl;
      }*/


   neighbors.resize(points.size());
   /*for (int i=0;i<points.size();i++)
   {
      for (int j=0;j<6;j++)
        neighbors[i][j]=-1;
   }*/
   for (int i=0;i<triangles.size();i++)
   {
      addNeighbor(triangles[i]->point_id[0], triangles[i]->point_id[1]);
      addNeighbor(triangles[i]->point_id[0], triangles[i]->point_id[2]);
      addNeighbor(triangles[i]->point_id[1], triangles[i]->point_id[2]);
   }
   cerr << "sphere: " << points.size() << " " << triangles.size() << endl;
}

void SphereQuant::addNeighbor(int id1, int id2)
{
   for (int j=0;j<6;j++)
   {
      if (neighbors[id1][j] == -1)
      {
         neighbors[id1][j]=id2;
         break;
      } else if (neighbors[id1][j] == id2)
         break;
   }

   for (int j=0;j<6;j++)
   {
      if (neighbors[id2][j] == -1)
      {
         neighbors[id2][j]=id1;
         break;
      } else if (neighbors[id2][j] == id1)
         break;
   }
}

int SphereQuant::addPoint(const Point &p)
{
   for (int i=0;i<points.size();i++)
   {
      if (points[i]==p)
         return i;
   }
   points.push_back(p);
   return points.size()-1;
}

Triangle *SphereQuant::searchTriangle(const Point &p)
{
   Triangle *tri = children[0];
   float best_dist = children[0]->center.dist(p);

   for (int i=1;i<20;i++)
   {
      float dist = children[i]->center.dist(p);
      if (dist < best_dist)
      {
         best_dist = dist;
         tri = children[i];
      }
   }
   return tri->search(p);
}

int SphereQuant::operator() (const Point &p)
{
   Triangle *tri = searchTriangle(p);
   
   int id = tri->point_id[0];
   float best_dist = tri->points[0].dist(p);
   
   float dist = tri->points[1].dist(p);
   if (dist < best_dist)
   {
      best_dist = dist;
      id = tri->point_id[1];
   }

   dist = tri->points[2].dist(p);
   if (dist < best_dist)
   {
      best_dist = dist;
      id = tri->point_id[2];
   }
   return id;
}
