#ifndef ZIK_FONT_929
#define ZIK_FONT_929

#include <SDL_ttf.h>

namespace Zik
{

	class Surface;

	class Font
	{
	public:
		Font(const std::string & filename, int ptsize);

		void Print(DrawDestination & dest, int x, int y, const std::string & str) const;
		Surface MakeSurface(const std::string & str) const;
	private:
		TTF_Font * pFont_;
	};

}



#endif // #ifndef ZIK_FONT_929