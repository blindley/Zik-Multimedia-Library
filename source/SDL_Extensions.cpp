#include "SDL_Extensions.h"
#include <utility>
#include <SDL.h>

namespace Zik
{

	namespace SDL_Ex
	{

		SDL_Surface * CreateSurface(Uint32 flags, int width, int height)
		{
			SDL_Surface *surface;
			Uint32 rmask, gmask, bmask, amask;

			/* SDL interprets each pixel as a 32-bit number, so our masks must depend
			on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x00000000;
#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0x00000000;
#endif

			surface = SDL_CreateRGBSurface(flags, width, height, 24,
				rmask, gmask, bmask, amask);
			return surface;
		}

		SDL_Surface * CreateSurfaceFrom(Uint32 flags, int width, int height, const uint8_t * pixel_data)
		{
			SDL_Surface *surface;
			Uint32 rmask, gmask, bmask, amask;

			/* SDL interprets each pixel as a 32-bit number, so our masks must depend
			on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x00000000;
#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0x00000000;
#endif

			surface = SDL_CreateRGBSurfaceFrom((void*)pixel_data, width, height, 24, width * 3,
				rmask, gmask, bmask, amask);			
			return surface;
		}

		SDL_Surface * CopySurface(SDL_Surface * in_surf)
		{
			SDL_Surface * surf_cpy;
			surf_cpy = SDL_ConvertSurface(in_surf, in_surf->format, SDL_HWSURFACE | SDL_HWACCEL);
			return surf_cpy;
		}

		SDL_Surface * ExtractRegion( int x, int y, int width, int height, SDL_Surface * src_surf,  bool colorkey ) 
		{
			SDL_Rect src_rect;
			src_rect.x = x;
			src_rect.y = y;
			src_rect.w = width;
			src_rect.h = height;

			SDL_Surface * region;
			region = CreateSurface(SDL_HWSURFACE | SDL_HWACCEL, width, height);

			/* Fill the surface with the transparent color, so that if the source surface uses
			a colorkey, the non-copied parts will already be set to the right color. */
			SDL_FillRect(region, NULL, TransColor(region));

			SDL_BlitSurface(src_surf, &src_rect, region, NULL);
			if(colorkey)
				SetColorKey(region, true);

			return region;
		}



		SDL_Surface ** ExtractFrames(SDL_Surface * src_surf,
			int frame_width, int frame_height,
			int frames_per_row, int frames_per_column,
			bool padding)
		{
			int frame_total = frames_per_row * frames_per_column;
			if(frame_total <= 0)
				return NULL;
			SDL_Surface ** surf_array = new SDL_Surface*[frame_total];

			bool colorkey = (src_surf->flags & SDL_SRCCOLORKEY) != 0;

			int x_start = (padding ? 1 : 0);
			int y_start = x_start;
			int x_inc = frame_width + (padding ? 1 : 0);
			int y_inc = frame_height + (padding ? 1 : 0);

			int frame_x = 0;
			int frame_y = 0;
			for(int i=0; i<frame_total; ++i)
			{
				int sx = x_start + (frame_x * x_inc);
				int sy = y_start + (frame_y * y_inc);

				surf_array[i] = ExtractRegion(sx, sy, frame_width, frame_height, src_surf, colorkey);

				++frame_x;
				if(frame_x == frames_per_row)
				{
					++frame_y;
					frame_x = 0;
				}		
			}

			return surf_array;
		}

		void SetColorKey(SDL_Surface * surface, bool set)
		{
			Uint32 flags = 0;
			Uint32 color = 0;
			if(set)
			{
				flags = SDL_SRCCOLORKEY | SDL_RLEACCEL;
				color = TransColor(surface);
			}

			SDL_SetColorKey(surface, flags, color);
		}

		void PutPixel32_nolock(SDL_Surface * surface, int x, int y, Uint32 color)
		{
			Uint8 * pixel = (Uint8*)surface->pixels;
			pixel += (y * surface->pitch) + (x * sizeof(Uint32));
			*((Uint32*)pixel) = color;
		}

		void PutPixel24_nolock(SDL_Surface * surface, int x, int y, Uint32 color)
		{
			Uint8 * pixel = (Uint8*)surface->pixels;
			pixel += (y * surface->pitch) + (x * sizeof(Uint8) * 3);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			pixel[0] = (color >> 24) & 0xFF;
			pixel[1] = (color >> 16) & 0xFF;
			pixel[2] = (color >> 8) & 0xFF;
#else
			pixel[0] = color & 0xFF;
			pixel[1] = (color >> 8) & 0xFF;
			pixel[2] = (color >> 16) & 0xFF;
#endif	
		}

		void PutPixel16_nolock(SDL_Surface * surface, int x, int y, Uint32 color)
		{
			Uint8 * pixel = (Uint8*)surface->pixels;
			pixel += (y * surface->pitch) + (x * sizeof(Uint16));
			*((Uint16*)pixel) = color & 0xFFFF;
		}

		void PutPixel8_nolock(SDL_Surface * surface, int x, int y, Uint32 color)
		{
			Uint8 * pixel = (Uint8*)surface->pixels;
			pixel += (y * surface->pitch) + (x * sizeof(Uint8));
			*pixel = color & 0xFF;
		}

		void PutPixel32(SDL_Surface * surface, int x, int y, Uint32 color)
		{
			if( SDL_MUSTLOCK(surface) )
				SDL_LockSurface(surface);
			PutPixel32_nolock(surface, x, y, color);
			if( SDL_MUSTLOCK(surface) )
				SDL_UnlockSurface(surface);
		}

		void PutPixel24(SDL_Surface * surface, int x, int y, Uint32 color)
		{
			if( SDL_MUSTLOCK(surface) )
				SDL_LockSurface(surface);
			PutPixel24_nolock(surface, x, y, color);
			if( SDL_MUSTLOCK(surface) )
				SDL_LockSurface(surface);
		}

		void PutPixel16(SDL_Surface * surface, int x, int y, Uint32 color)
		{
			if( SDL_MUSTLOCK(surface) )
				SDL_LockSurface(surface);
			PutPixel16_nolock(surface, x, y, color);
			if( SDL_MUSTLOCK(surface) )
				SDL_UnlockSurface(surface);
		}

		void PutPixel8(SDL_Surface * surface, int x, int y, Uint32 color)
		{
			if( SDL_MUSTLOCK(surface) )
				SDL_LockSurface(surface);
			PutPixel8_nolock(surface, x, y, color);
			if( SDL_MUSTLOCK(surface) )
				SDL_UnlockSurface(surface);
		}

		void HLine(SDL_Surface * surface, int x, int y, int width, Uint32 color)
		{
			if(width == 0)
				return;
			if(width < 0)
			{
				x += width + 1;
				width = -width;
			}
			SDL_Rect dst_rect;
			dst_rect.x = x;
			dst_rect.y = y;
			dst_rect.w = width;
			dst_rect.h = 1;

			SDL_FillRect(surface, &dst_rect, color);
		}

		void VLine(SDL_Surface * surface, int x, int y, int height, Uint32 color)
		{
			if(height == 0)
				return;
			if(height < 0)
			{
				y += height + 1;
				height = -height;
			}

			SDL_Rect dst_rect;
			dst_rect.x = x;
			dst_rect.y = y;
			dst_rect.w = 1;
			dst_rect.h = height;

			SDL_FillRect(surface, &dst_rect, color);
		}

		void Line(SDL_Surface * surface, int x, int y, int x2, int y2, Uint32 color)
		{
			if(y == y2)
				HLine(surface, x, y, x2-x+1, color);
			if(x == x2)
				VLine(surface, x, y, y2-y+1, color);

			int dx = x2 - x;
			int dy = y2 - y;

			void(*putpixel)(SDL_Surface *, int, int, Uint32);
			switch(surface->format->BitsPerPixel)
			{
			case 32: putpixel = PutPixel32_nolock; break;
			case 24: putpixel = PutPixel24_nolock; break;
			case 16: putpixel = PutPixel16_nolock; break;
			case 8: putpixel = PutPixel8_nolock; break;
			}

			if(SDL_MUSTLOCK(surface))
				SDL_LockSurface(surface);

			if(abs(dx) > abs(dy))
			{
				if(x2 < x)
				{
					std::swap(x,x2);
					std::swap(y,y2);
				}
				float yinc = static_cast<float>(dy)/dx;
				for(int ix=x, i=0; ix<=x2; ++ix, ++i)
				{
					int j = y + static_cast<int>(yinc * i);
					putpixel(surface,ix,j,color);
				}
			}
			else
			{
				if(y2 < y)
				{
					std::swap(x,x2);
					std::swap(y,y2);
				}
				float xinc = static_cast<float>(dx)/dy;
				for(int iy=y, i=0; iy<=y2; ++iy, ++i)
				{
					int j = x + static_cast<int>(xinc * i);
					putpixel(surface,j,iy,color);
				}
			}

			if(SDL_MUSTLOCK(surface))
				SDL_UnlockSurface(surface);
		}

		void DrawRect(SDL_Surface * surface, int x, int y, int width, int height, Uint32 color)
		{
			if(width == 0 || height == 0)
				return;

			if(width < 0) {	x += width + 1;	width = -width;	}
			if(height < 0) { y += height + 1; height = -height; }

			if(height == 1)
				return HLine(surface, x, y, width, color);

			if(width == 1)
				return VLine(surface, x, y, height, color);

			HLine(surface, x, y, width, color);
			HLine(surface, x, y+height-1, width, color);

			if(height == 2)
				return;

			VLine(surface, x, y+1, height-2, color);
			VLine(surface, x+width-1, y+1, height-2, color);
		}

		void Circle_helper_plot4points( SDL_Surface * surface, int cx, int cy, int x, int y, Uint32 color,
			void(*putpixel)(SDL_Surface *, int, int, Uint32)) 
		{
			putpixel(surface, cx + x, cy + y, color);
			if (x != 0) putpixel(surface, cx - x, cy + y, color);
			if (y != 0) putpixel(surface, cx + x, cy - y, color);
			if (x != 0 && y != 0) putpixel(surface, cx - x, cy - y, color);
		}

		void Circle_helper_plot8points( SDL_Surface * surface, int cx, int cy, int x, int y, Uint32 color,
			void(*putpixel)(SDL_Surface *, int, int, Uint32)) 
		{
			Circle_helper_plot4points(surface, cx, cy, x, y, color, putpixel);
			if (x != y)
				Circle_helper_plot4points(surface, cx, cy, y, x, color, putpixel);
		}

		void Circle(SDL_Surface * surface, int cx, int cy, int radius, Uint32 color)
		{
			void(*putpixel)(SDL_Surface *, int, int, Uint32);
			switch(surface->format->BitsPerPixel)
			{
			case 32: putpixel = PutPixel32_nolock; break;
			case 24: putpixel = PutPixel24_nolock; break;
			case 16: putpixel = PutPixel16_nolock; break;
			case 8: putpixel = PutPixel8_nolock; break;
			}

			int error = -radius;
			int x = radius;
			int y = 0;

			if(SDL_MUSTLOCK(surface))
				SDL_LockSurface(surface);

			while (x >= y)
			{
				Circle_helper_plot8points(surface, cx, cy, x, y, color, putpixel);

				error += y;
				++y;
				error += y;

				// The following test may be implemented in assembly language in
				// most machines by testing the carry flag after adding 'y' to
				// the value of 'error' in the previous step, since 'error'
				// nominally has a negative value.
				if (error >= 0)
				{
					--x;
					error -= x;
					error -= x;
				}
			}


			if(SDL_MUSTLOCK(surface))
				SDL_UnlockSurface(surface);
		}

		void FillCircle_helper_plot4points(SDL_Surface * surface, int cx, int cy, int x, int y, Uint32 color) 
		{
			HLine(surface, cx - x, cy - y, x * 2 + 1, color);
			if(y != 0) HLine(surface, cx - x, cy + y, x * 2 + 1, color);
		}

		void FillCircle_helper_plot8points( SDL_Surface * surface, int cx, int cy, int x, int y, Uint32 color) 
		{
			FillCircle_helper_plot4points(surface, cx, cy, x, y, color);
			if (x != y)
				FillCircle_helper_plot4points(surface, cx, cy, y, x, color);
		}

		void FillCircle(SDL_Surface * surface, int cx, int cy, int radius, Uint32 color)
		{
			int error = -radius;
			int x = radius;
			int y = 0;

			while (x >= y)
			{
				FillCircle_helper_plot8points(surface, cx, cy, x, y, color);

				error += y;
				++y;
				error += y;

				// The following test may be implemented in assembly language in
				// most machines by testing the carry flag after adding 'y' to
				// the value of 'error' in the previous step, since 'error'
				// nominally has a negative value.
				if (error >= 0)
				{
					--x;
					error -= x;
					error -= x;
				}
			}
		}


		Uint32 TransColor( SDL_Surface * surface ) { return SDL_MapRGB(surface->format, 255, 0, 255); }
		Uint32 White( SDL_Surface * surface ) { return SDL_MapRGB(surface->format, 255, 255, 255); }
		Uint32 Grey(SDL_Surface * surface, Uint8 val) { return SDL_MapRGB(surface->format, val, val, val); }
		Uint32 Red(SDL_Surface * surface, Uint8 val) { return SDL_MapRGB(surface->format, val, 0, 0); }
		Uint32 Green(SDL_Surface * surface, Uint8 val) { return SDL_MapRGB(surface->format, 0, val, 0); }
		Uint32 Blue(SDL_Surface * surface, Uint8 val) { return SDL_MapRGB(surface->format, 0, 0, val); }
		Uint32 Cyan(SDL_Surface * surface, Uint8 val) { return SDL_MapRGB(surface->format, 0, val, val); }
		Uint32 Magenta(SDL_Surface * surface, Uint8 val) { return SDL_MapRGB(surface->format, val, 0, val); }
		Uint32 Yellow(SDL_Surface * surface, Uint8 val) { return SDL_MapRGB(surface->format, val, val, 0); }

	}

}