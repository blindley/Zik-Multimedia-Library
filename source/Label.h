#ifndef ZIK_LABEL_907
#define ZIK_LABEL_907

#include <string>
#include <SDL.h>
#include "BitmapFont.h"
#include "Font.h"
#include "Surface.h"

namespace Zik
{

	class Label
	{
	public:
		Label(DrawDestination & dest, const Font & font);
		Label(DrawDestination & dest, const Font & font, int dx, int dy);

		void SetText(std::string);
		std::string GetText() const;

		void Draw() const;
		void CenterJustify(int x, int y);
		void LeftJustify(int x, int y);
		void RightJustify(int x, int y);
	private:
		enum { LEFT, CENTER, RIGHT} justification_;
		std::string text_;
		int dx_, dy_;

		Surface surface_;

		// unowned properties
		const Font & font_;
		DrawDestination & dest_;
	};

}

#endif // #ifndef ZIK_LABEL_907