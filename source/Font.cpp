#include <string>
#include "Surface.h"
#include "Font.h"

namespace Zik
{

	Font::Font(const std::string & filename, int ptsize )
		:pFont_(NULL)
	{
		pFont_ = TTF_OpenFont(filename.c_str(), ptsize);
		if(pFont_ == NULL)
		{
			throw std::string("Font::Font(): Unable to open font: \"")
				+ std::string(filename) + std::string("\" -> ")
				+ std::string(TTF_GetError());
		}
	}


	void Font::Print(DrawDestination & dest, int x, int y, const std::string & str) const
	{
		static SDL_Color fg = { 255, 255, 255, 0 };
		Surface temp(TTF_RenderText_Solid(pFont_, str.c_str(), fg));

		dest.BlitFrom(temp, x, y);
	}

	Surface Font::MakeSurface(const std::string & str) const
	{
		static SDL_Color fg = { 255, 255, 255, 0 };
		return Surface(TTF_RenderText_Solid(pFont_, str.c_str(), fg));
	}

}