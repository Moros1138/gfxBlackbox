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