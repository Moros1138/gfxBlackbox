# gfxBlackBox.h

## What is this?


This is compilation of maths/algorithms and utility functions that I have
grown tired of re-writing/re-implementing in new projects. This header
allows me to drop it in and start re-using the code in new projects!

## DISCLAIMER

Portions of the code have been adapted/rewritten and should NOT be considered
my original work! I take very little credit for adapting it for use with
the OneLoneCoder PixelGameEngine, however, aside from the drawing functions,
~~this header should be useful regardless of the framework you use to put your
pixels on the screen~~! (I lied, it relies on the 2d vector class provided by
PGE)

## CREDITS

**OneLoneCoder PixelGameEngine**:

https://github.com/OneLoneCoder/olcPixelGameEngine


**Efficient Polygon Triangulation**:

https://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml


## License (OLC-3)
```
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
```

## Thanks

I'd like to thank **Fyre** for pointing me in the direction of polygon
triangulation when I was trying to figure out how to fill in my polygons
without the use of a flood fill algorithm! Their advice sent me down the
rabbit whole which lead to the creation of this header file.

## Example Usage (Assumes usage of [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine))
```cpp
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
```