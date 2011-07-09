#ifndef ZIK_SDL_EXTENSIONS_903
#define ZIK_SDL_EXTENSIONS_903

#include <cstdint>

struct SDL_Surface;
typedef unsigned int Uint32;
typedef unsigned char Uint8;

namespace Zik
{
	namespace SDL_Ex
	{

		SDL_Surface * CreateSurface(Uint32 flags, int width, int height);
		SDL_Surface * CreateSurfaceFrom(Uint32 flags, int width, int height, const uint8_t * pixel_data);
		SDL_Surface * CopySurface(SDL_Surface * in_surf);
		SDL_Surface * ExtractRegion( int x, int y, int width, int height, SDL_Surface * src_surf, bool colorkey );
		SDL_Surface ** ExtractFrames(SDL_Surface * src_surf,
			int frame_width, int frame_height,
			int frames_per_row, int frames_per_column,
			bool padding);

		void SetColorKey(SDL_Surface * surface, bool set);

		void PutPixel32_nolock(SDL_Surface * surface, int x, int y, Uint32 color);
		void PutPixel24_nolock(SDL_Surface * surface, int x, int y, Uint32 color);
		void PutPixel16_nolock(SDL_Surface * surface, int x, int y, Uint32 color);
		void PutPixel8_nolock(SDL_Surface * surface, int x, int y, Uint32 color);

		void PutPixel32(SDL_Surface * surface, int x, int y, Uint32 color);
		void PutPixel24(SDL_Surface * surface, int x, int y, Uint32 color);
		void PutPixel16(SDL_Surface * surface, int x, int y, Uint32 color);
		void PutPixel8(SDL_Surface * surface, int x, int y, Uint32 color);

		void HLine(SDL_Surface * surface, int x, int y, int width, Uint32 color);
		void VLine(SDL_Surface * surface, int x, int y, int height, Uint32 color);
		void DrawRect(SDL_Surface * surface, int x, int y, int width, int height, Uint32 color);
		void Line(SDL_Surface * surface, int x, int y, int x2, int y2, Uint32 color);
		void Circle(SDL_Surface * surface, int cx, int cy, int radius, Uint32 color);
		void FillCircle(SDL_Surface * surface, int cx, int cy, int radius, Uint32 color);

		Uint32 TransColor(SDL_Surface * surface);
		Uint32 White(SDL_Surface * surface);
		Uint32 Grey(SDL_Surface * surface, Uint8 val = 127);
		Uint32 Red(SDL_Surface * surface, Uint8 val = 255);
		Uint32 Green(SDL_Surface * surface, Uint8 val = 255);
		Uint32 Blue(SDL_Surface * surface, Uint8 val = 255);
		Uint32 Cyan(SDL_Surface * surface, Uint8 val = 255);
		Uint32 Magenta(SDL_Surface * surface, Uint8 val = 254); // 254 because 255 would be DEATH MAGENTA, our transparent color
		Uint32 Yellow(SDL_Surface * surface, Uint8 val = 255);

	}
}

#endif // #ifndef ZIK_SDL_EXTENSIONS_903