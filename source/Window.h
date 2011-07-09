#ifndef ZIK_WINDOW_854
#define ZIK_WINDOW_854

#include "Surface.h"

namespace Zik
{
	class Window : public DrawDestination
	{
	public:
		Window(const std::string & window_title, int width, int height);
		~Window();

		void ProcessEvents();
		bool Closed() const { return closed_; }
		void Flip();

		void BlitFrom(const Surface & src);
		void BlitFrom(const Surface & src, int dx, int dy);

		void Fill(Uint32 color);
		void FillRect(int dx, int dy, int dw, int dh, Uint32 color);
		void PutPixel(int dx, int dy, Uint32 color);
		void Rect(int dx, int dy, int width, int height, Uint32 color);
		void Line(int x, int y, int x2, int y2, Uint32 color);
		void Circle(int cx, int cy, int radius, Uint32 color);
		void FillCircle(int cx, int cy, int radius, Uint32 color);

		int GetWidth() const;
		int GetHeight() const;

		uint32_t MakeColor(uint8_t r, uint8_t g, uint8_t b) const;
	private:
		Surface surface_;
		bool closed_;

		// private to disallow Copying
		Window(const Window &) {}
		void operator=(const Window &) {}
	};
}

#endif // #ifndef ZIK_WINDOW_854