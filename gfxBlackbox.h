/*
    gfxBlackbox.h

	+-------------------------------------------------------------+
	|              Moros1138's GFX BlackBox v0.01                 |
	|        "I hate maths and maths hates me!" -Moros1138        |
	+-------------------------------------------------------------+    

	
    What is this?
	~~~~~~~~~~~~~
    This is compilation of maths/algorithms and utility functions that I have
    grown tired of re-writing/re-implementing in new projects. This header
    allows me to drop it in and start re-using the code in new projects!

    DISCLAIMER
	~~~~~~~~~~
    Portions of the code have been adapted/rewritten and should NOT be considered
    my original work! I take very little credit for adapting it for use with
    the OneLoneCoder PixelGameEngine, however, aside from the drawing functions,
    this header should be useful regardless of the framework you use to put your
    pixels on the screen!

    CREDITS
	~~~~~~~
    
    OneLoneCoder PixelGameEngine:
    https://github.com/OneLoneCoder/olcPixelGameEngine
    
    
    Efficient Polygon Triangulation:
    https://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml


	License (OLC-3)
	~~~~~~~~~~~~~~~

    Copyright 2021 Moros1138 <moros1138@gmail.com>

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	1. Redistributions or derivations of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce the above
	copyright notice. This list of conditions and the following	disclaimer must be
	reproduced in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may
	be used to endorse or promote products derived from this software without specific
	prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	"AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL,	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
	SUCH DAMAGE.    


	Thanks
	~~~~~~
    I'd like to thank Fyre for pointing me in the direction of polygon
    triangulation when I was trying to figure out how to fill in my polygons
    without the use of a flood fill algorithm! Their advice sent me down the
    rabbit whole which lead to the creation of this header file.

    Author: Moros1138 <moros1138@gmail.com>
*/

// O------------------------------------------------------------------------------O
// | Example Usage (assumes usage of olc::PixelGameEngine                         |
// O------------------------------------------------------------------------------O
/*
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define GFX_BLACKBOX_IMPLEMENTATION
#include "gfxBlackbox.h"

using namespace gfx_blackbox;

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
        // define a simple polygon as our model
        model = {
            { -0.5f,  0.0f },
            { -1.0f, -1.0f },
            {  2.0f,  0.0f },
            { -1.0f,  1.0f },
        };		

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        Clear(olc::BLACK);

        // transform the provided polygon based on the provided position, scale, and angle(in radians)
        std::vector<olc::vf2d> transformed = Polygon::Transform(
            model,    // the model (a std::vector<olc::vf2d> of points that make up the polygon)
            {50, 50}, // position
            10.0f,    // scale
            0.0f      // angle (in radians)
        );

        // fill the transformed polygon with the selected color
        Polygon::Fill(this, transformed, olc::GREY);
        
        // draw the triangles of the transformed polygon with the selected color
        Polygon::Wire(this, transformed, olc::DARK_GREY);
        
        // draw the outline of the transformed polygon with the selected color
        Polygon::Stroke(this, transformed, olc::WHITE);

        return true;
	}

    std::vector<olc::vf2d> model;
};

int main()
{
	Example demo;
	if (demo.Construct(320, 180, 4, 4))
		demo.Start();
	return 0;
}

*/

#ifndef GFX_BLACKBOX_H
#define GFX_BLACKBOX_H

#include "olcPixelGameEngine.h"

namespace gfx_blackbox {
    
    class Polygon
    {
    public: // MATHS AND ALGORITHMS
        
        // compute area of a polygon
        static float Area(const std::vector<olc::vf2d>& polygon);
        // determine if the provided points are an ear 
        static bool IsEar(const std::vector<olc::vf2d>& polygon, int u, int v, int w, int n, std::vector<int> V);
        // determine if point Px/Py is inside triangle defined by (Ax,Ay) (Bx,By) (Cx,Cy)
        static bool IsPointInTriangle(olc::vf2d A, olc::vf2d B, olc::vf2d C, olc::vf2d P);
        // transform the provided polygon with the provided: position, scale, and rotation
        static std::vector<olc::vf2d> Transform(const std::vector<olc::vf2d>& polygon, const olc::vf2d& position, const float& scale=1.0f, const float& rotation=0.0f);
        // perfroms an ear clipping triangulation of the provided polygon, places results in STL vector as series of triangles.
        static bool Triangulate(const std::vector<olc::vf2d>& polygon, std::vector<olc::vf2d>& triangles);
        
    public: // DRAWING ROUTINES
        
        // draw the specified polygon filled with the specified color
        static void Fill  (olc::PixelGameEngine* pge, const std::vector<olc::vf2d>& polygon, const olc::Pixel& color=olc::BLACK);
        // draw the specified polygon stroked with the specified color
        static void Stroke(olc::PixelGameEngine* pge, const std::vector<olc::vf2d>& polygon, const olc::Pixel& color=olc::WHITE);
        // draw the triangles of a specified polygon with the specified color
        static void Wire  (olc::PixelGameEngine* pge, const std::vector<olc::vf2d>& polygon, const olc::Pixel& color=olc::WHITE);

    };


} // gfx namespace

#ifdef GFX_BLACKBOX_IMPLEMENTATION
#undef GFX_BLACKBOX_IMPLEMENTATION

namespace gfx_blackbox {
    
    static const float EPSILON=0.0000000001f;

    // Polygon: MATHS AND ALGORITHMS
    
    // compute area of a polygon
    float Polygon::Area(const std::vector<olc::vf2d>& polygon)
    {
        float area = 0.0f;
        
        for(int i = 0; i < polygon.size(); i++)
            area += polygon[i].cross(polygon[(i+1) % polygon.size()]);
        
        return area * 0.5f;
    }

    // determine if the provided points are an ear 
    bool Polygon::IsEar(const std::vector<olc::vf2d>& polygon, int u, int v, int w, int n, std::vector<int> V)
    {
        olc::vf2d a, b, c;

        a = polygon[V[u]];
        b = polygon[V[v]];
        c = polygon[V[w]];

        // if (b) is not at a convex angle to (a) and (b), not an ear
        if(EPSILON > (((b.x-a.x)*(c.y-a.y)) - ((b.y-a.y)*(c.x-a.x))))
            return false;

        for(int i = 0; i < n; i++)
        {
            // if we're already testing this point, move on
            if(i == u || i == v || i == w)
                continue; 

            // if any of the remaining points are inside the current triangle
            // it's not a valid ear
            if(IsPointInTriangle(polygon[V[i]], a, b, c))
                return false;
            
        }
        
        // we have a valid ear, let's tell the world!
        return true;
    }
    
    // determine if point Px/Py is inside triangle defined by (Ax,Ay) (Bx,By) (Cx,Cy)
    bool Polygon::IsPointInTriangle(olc::vf2d point, olc::vf2d a, olc::vf2d b, olc::vf2d c)
    {
        olc::vf2d bc, ca, ac, ap, bp, cp;

        bc = c - b;
        ca = a - c;
        ac = c - a;

        ap = point - bc;
        bp = point - ca;
        cp = point - ac;

        return ((bc.cross(bp) >= 0.0f) && (ca.cross(cp) >= 0.0f) && (ac.cross(ap) >= 0.0f));
    }

    // transform the provided polygon with the provided: position, scale, and rotation
    std::vector<olc::vf2d> Polygon::Transform(const std::vector<olc::vf2d>& polygon, const olc::vf2d& position, const float& scale, const float& rotation)
    {
        // place to store the transformed points of the polygon
        std::vector<olc::vf2d> points;

        // for each point in polygon
        for(int i = 0; i < polygon.size(); i++)
        {
            olc::vf2d p;
            
            // rotate
            p.x = polygon[i].x *  cosf(rotation) - polygon[i].y * -sinf(rotation);
            p.y = polygon[i].x * -sinf(rotation) + polygon[i].y *  cosf(rotation);
            
            // scale
            p *= scale;

            // translate
            p += position;
            
            // add transformed point
            points.push_back(p);
        }
        
        return points;
    }

    // perfroms an ear clipping triangulation of the provided polygon, places results in STL vector as series of triangles.
    bool Polygon::Triangulate(const std::vector<olc::vf2d>& polygon, std::vector<olc::vf2d>& triangles)
    {
        /* allocate and initialize list of Vertices in polygon */
        int n = polygon.size();
        if(n < 3)
            return false;

        std::vector<int> V;
        V.reserve(n);

        /* we want a counter-clockwise polygon in V */
        if(0.0f < Area(polygon))
        {
            for (int v=0; v<n; v++)
                V.push_back(v);
        }
        else
        {
            for(int v=0; v<n; v++)
                V.push_back((n-1)-v);
        }
        
        int nv = n;

        /*  remove nv-2 Vertices, creating 1 triangle every time */
        int count = 2*nv;   /* error detection */

        for(int m=0, v=nv-1; nv>2; )
        {
            /* if we loop, it is probably a non-simple polygon */
            if (0 >= (count--))
            {
                //** Triangulate: ERROR - probable bad polygon!
                return false;
            }

            /* three consecutive vertices in current polygon, <u,v,w> */
            int u = v;
            
            /* previous */
            if(nv <= u) u = 0;
                v = u+1;
        
            /* new v    */
            if(nv <= v)
                v = 0;     
        
            int w = v+1;
        
            /* next     */
            if(nv <= w)
                w = 0;     

            if(IsEar(polygon,u,v,w,nv,V))
            {
                int a,b,c,s,t;

                /* true names of the vertices */
                a = V[u];
                b = V[v];
                c = V[w];

                /* output Triangle */
                triangles.push_back(polygon[a]);
                triangles.push_back(polygon[b]);
                triangles.push_back(polygon[c]);

                m++;

                /* remove v from remaining polygon */
                for(s=v,t=v+1;t < nv;s++,t++)
                    V[s] = V[t];
                    
                nv--;

                /* reset error detection counter */
                count = 2*nv;
            }
        }

        return true;
    }

    // Polygon: DRAWING ROUTINES
    
    void Polygon::Fill(olc::PixelGameEngine* pge, const std::vector<olc::vf2d>& polygon, const olc::Pixel& color)
    {
        if(polygon.size() < 3)
            return;
        
        std::vector<olc::vf2d> triangles;
        
        Triangulate(polygon, triangles);
        
        for(int i = 0; i < triangles.size(); i += 3)
            pge->FillTriangle(triangles[i+0], triangles[i+1], triangles[i+2], color);
    }

    void Polygon::Stroke(olc::PixelGameEngine* pge, const std::vector<olc::vf2d>& polygon, const olc::Pixel& color)
    {
        if(polygon.size() < 3)
            return;
        
        for(int i = 0; i < polygon.size(); i++)
            pge->DrawLine(polygon[i], polygon[(i+1) % polygon.size()], color);
    }

    void Polygon::Wire(olc::PixelGameEngine* pge, const std::vector<olc::vf2d>& polygon, const olc::Pixel& color)
    {
        if(polygon.size() < 3)
            return;
        
        std::vector<olc::vf2d> triangles;
        
        Triangulate(polygon, triangles);
        
        for(int i = 0; i < triangles.size(); i += 3)
            pge->DrawTriangle(triangles[i+0], triangles[i+1], triangles[i+2], color);
    }

} // gfx namespace

#endif // GFX_IMPLEMENTATION


#endif // GFX_BLACKBOX_H
