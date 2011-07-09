#ifndef ZIK_BITMAP_FONT_901
#define ZIK_BITMAP_FONT_901

#include <vector>
#include "Surface.h"

namespace Zik
{

	class BitmapFont
	{
	public:
		~BitmapFont();

		void Load(const std::string & filename, int c_width, int c_height);
		void PrintString(DrawDestination & dest, int dx, int dy, const std::string & str) const;
		int StringWidth(const std::string & str) const;
	private:
		const Surface & operator[](int index) const;

		std::vector<Surface> surface_vec_;
	};

}

#endif // #ifndef ZIK_BITMAP_FONT_901