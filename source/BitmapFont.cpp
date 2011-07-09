#include <algorithm>
#include <iterator>
#include <numeric>
#include "BitmapFont.h"
#include "SDL_Extensions.h"
#include "Surface.h"

namespace Zik
{

	BitmapFont::~BitmapFont()
	{

	}

	void BitmapFont::Load(const std::string & filename, int c_width, int c_height)
	{
		surface_vec_.resize(96);
		Surface(filename, true).ExtractFrames(surface_vec_.begin(), c_width, c_height, 16, 6, true);
	}

	void BitmapFont::PrintString(DrawDestination & dest, int dx, int dy, const std::string & str) const
	{
		int cwidth = surface_vec_[0].GetWidth();

		std::for_each(str.begin(), str.end(), [&](char c) {		
			dest.BlitFrom(
				this->operator[](c),
				dx,dy);
			dx += cwidth;			
		});
	}

	int BitmapFont::StringWidth(const std::string & str) const
	{
		int width = 0;
		std::for_each(str.begin(), str.end(), [&](char c) {		
			width += this->operator[](c).GetWidth();			
		});
		return width;
	}

	const Surface & BitmapFont::operator[](int index) const
	{
		return surface_vec_[index - ' '];
	}
}