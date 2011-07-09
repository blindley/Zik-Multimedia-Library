#ifndef ZIK_BLIT_SURFACE_902
#define ZIK_BLIT_SURFACE_902

#include <string>
#include <iterator>

#include "SDL_Extensions.h"
#include "Image.h"

struct SDL_Surface;
typedef unsigned int Uint32;
typedef unsigned char Uint8;

namespace Zik
{
	class Surface;

	class DrawDestination
	{
	public:
		virtual void BlitFrom(const Surface & src) = 0;
		virtual void BlitFrom(const Surface & src, int dx, int dy) = 0;

		virtual void Fill(Uint32 color) = 0;
		virtual void FillRect(int dx, int dy, int dw, int dh, Uint32 color) = 0;
		virtual void PutPixel(int dx, int dy, Uint32 color) = 0;
		virtual void Rect(int dx, int dy, int width, int height, Uint32 color) = 0;
		virtual void Line(int x, int y, int x2, int y2, Uint32 color) = 0;
		virtual void Circle(int cx, int cy, int radius, Uint32 color) = 0;
		virtual void FillCircle(int cx, int cy, int radius, Uint32 color) = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual uint32_t MakeColor(uint8_t r, uint8_t g, uint8_t b) const = 0;
	private:
	};

	class Surface : public DrawDestination
	{
	public:
		Surface();
		Surface(const Surface & that);
		Surface(Surface && that);
		Surface(const std::string & filename, bool use_color_key);
		Surface(int w, int h, const uint8_t * pixel_data, bool use_color_key);
		~Surface();

		Surface & operator=(Surface that);
		void swap(Surface & that);

		void Create(int w, int h, bool colorkey);
		void Create(int w, int h, const uint8_t * pixel_data, bool use_color_key);
		void Load(const std::string & filename, bool use_color_key);

		template<class OutputIterator>
		void ExtractFrames(OutputIterator it, int frame_width, int frame_height,
			int frames_per_row, int frames_per_column, bool padding) const;

		void Flip();

		void TransferTo(Surface & that);
		void Swap(Surface & that);

		void BlitTo(DrawDestination & dest) const;
		void BlitTo(DrawDestination & dest, int dx, int dy) const;

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
		Surface(SDL_Surface * surf);
		void AttachSurface(SDL_Surface * surf);
		SDL_Surface * DetachSurface();

		SDL_Surface * pSurface_;

		friend class Font;
		friend class Window;
	};

	template<typename OutputIterator>
	void Surface::ExtractFrames(OutputIterator it,
		int frame_width, int frame_height,
		int frames_per_row, int frames_per_column,
		bool padding) const
	{
		SDL_Surface ** temp_surf = SDL_Ex::ExtractFrames(pSurface_, frame_width, frame_height, frames_per_row, frames_per_column, padding);

		int surface_count = frames_per_row * frames_per_column;

		for(int i=0; i<surface_count; ++i)
		{			
			*it = Surface(temp_surf[i]);
			++it;
		}

		delete [] temp_surf;
	}

	Surface InitGraphics(int width, int height, int bpp);
}

#endif // #ifndef ZIK_BLIT_SURFACE_902