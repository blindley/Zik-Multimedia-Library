#include "Window.h"
#include <SDL.h>

#include<stdexcept>

#define nullptr 0

namespace Zik
{
	Window::Window(const std::string & window_title, int width, int height)
		:closed_(true)
	{
		if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
		{
			std::string what = "Window::Wnidow(): Couldn't initialize SDL -> ";
			what += SDL_GetError();
			throw std::runtime_error(what);
		}

		// atexit(SDL_Quit);
		Uint32 flags = SDL_HWSURFACE | SDL_ANYFORMAT;
		SDL_Surface * pSurface = SDL_SetVideoMode(width, height, 32, flags);

		if(pSurface == nullptr)
		{
			std::string what = "Window::Window(): Failed to set video mode -> ";
			what += SDL_GetError();
			throw std::runtime_error(what);
		}

		SDL_WM_SetCaption(window_title.c_str(), window_title.c_str());

		surface_.AttachSurface(pSurface);
		closed_ = false;
	}

	Window::~Window()
	{
		auto pSurface = surface_.DetachSurface();
		if(pSurface)
			SDL_Quit();
	}

	void Window::ProcessEvents()
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			if( ev.type == SDL_QUIT )
			{
				closed_ = true;
			}
		}
	}

	void Window::Flip()
	{
		surface_.Flip();
	}

	void Window::Fill(Uint32 color)
	{
		surface_.Fill(color);
	}

	void Window::FillRect(int dx, int dy, int dw, int dh, Uint32 color)
	{
		surface_.FillRect(dx,dy,dw,dh,color);
	}

	uint32_t Window::MakeColor(uint8_t r, uint8_t g, uint8_t b) const
	{
		return surface_.MakeColor(r,g,b);
	}

	void Window::BlitFrom(const Surface & src)
	{
		surface_.BlitFrom(src);
	}

	void Window::BlitFrom(const Surface & src, int dx, int dy)
	{
		surface_.BlitFrom(src,dx,dy);
	}

	int Window::GetWidth() const
	{
		return surface_.GetWidth();
	}

	int Window::GetHeight() const
	{
		return surface_.GetHeight();
	}

	void Window::PutPixel( int dx, int dy, Uint32 color )
	{
		surface_.PutPixel(dx,dy,color);
	}

	void Window::Rect( int dx, int dy, int width, int height, Uint32 color )
	{
		surface_.Rect(dx,dy,width,height,color);
	}

	void Window::Line( int x, int y, int x2, int y2, Uint32 color )
	{
		surface_.Line(x,y,x2,y2,color);
	}

	void Window::Circle( int cx, int cy, int radius, Uint32 color )
	{
		surface_.Circle(cx,cy,radius,color);
	}

	void Window::FillCircle( int cx, int cy, int radius, Uint32 color )
	{
		surface_.FillCircle(cx,cy,radius,color);
	}
}
