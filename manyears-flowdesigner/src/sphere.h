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

#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

class Point {
public:
   float x[3];
   Point()
   {
      x[0]=0;
      x[1]=0;
      x[2]=0;
   }
   float dist(const Point &p)
   {
      return 1-(p.x[0]*x[0] + p.x[1]*x[1] + p.x[2]*x[2]);
   }
   void set(float x1, float x2, float x3)
   {
      x[0]=x1;
      x[1]=x2;
      x[2]=x3;
   }
   bool operator==(const Point &p)
   {
      for (int i=0;i<3;i++)
      {
         if (fabs(p.x[i]-x[i])>.0001)
            return false;
      }
      return true;
   } 
   Point(const Point &p1, const Point &p2)
   {
      int i;
      float norm=0;
      for (i=0;i<3;i++)
      {
         x[i] = p1.x[i] + p2.x[i];
         norm += x[i]*x[i];
      }
      norm = 1/sqrt(norm);
      for (i=0;i<3;i++)
      {
         x[i] *= norm;
         //cout << x[i] << " ";
      }
      //cout << endl;
   }
   Point(const Point &p1, const Point &p2, const Point &p3)
   {
      int i;
      float norm=0;
      for (i=0;i<3;i++)
      {
         x[i] = p1.x[i] + p2.x[i] + p3.x[i];
         norm += x[i]*x[i];
      }
      norm = 1/sqrt(norm);
      for (i=0;i<3;i++)
      {
         x[i] *= norm;
         //cout << x[i] << " ";
      }
      //cout << endl;
   }
};

class SphereQuant;

class Triangle {
   SphereQuant *sphere;
public:
   Point center;
   bool leaf;
   Point     points[3];
   int       point_id[3];
   Triangle *(children[4]);
public:
   //Triangle(SphereQuant *_sphere, const Point p[3]);
   Triangle(SphereQuant *_sphere, const Point &p0, const Point &p1, const Point &p2);
   void split(int levels);
   Triangle *search(const Point &p);
   friend class SphereQuant;
};

class Int6 {
public:
   int array[6];
   Int6() {for (int i=0;i<6;i++) array[i]=-1;};
   const int &operator[] (int i) const {return array[i];}
   int &operator[] (int i) {return array[i];}
};

class SphereQuant {
   Triangle *(children[20]);
   vector<Point> points;
   vector<Triangle*> triangles;
public:
   vector<Int6> neighbors;

   SphereQuant(int levels);
   int addPoint(const Point &p);
   void addTriangle(Triangle *tri) {triangles.push_back(tri);}
   Triangle *searchTriangle(const Point &p);
   int size() {return points.size();}
   const Point &operator[] (int i) {return points[i];}
   int operator() (const Point &p);

private:
   void addNeighbor(int id1, int id2);
};


extern SphereQuant sphere;
