#include "Surface.h"
#include "SDL_Extensions.h"
#include <SDL_image.h>

#include <stdexcept>
#include <algorithm>

namespace Zik
{

	Surface::Surface()
		:pSurface_(nullptr)
	{}

	Surface::Surface(const Surface & that)
		:pSurface_(nullptr)
	{
		if(that.pSurface_ == nullptr)
			return;

		pSurface_ = SDL_Ex::CopySurface(that.pSurface_);
	}

	Surface::Surface(Surface && that)
		:pSurface_(that.pSurface_)
	{
		that.pSurface_ = nullptr;
	}

	Surface::Surface(const std::string & filename, bool use_color_key)
		:pSurface_(nullptr)
	{
		Load(filename, use_color_key);
	}

	Surface::Surface(int w, int h, const uint8_t * pixel_data, bool use_color_key)
		:pSurface_(nullptr)
	{
		Create(w,h,pixel_data,use_color_key);
	}

	// private constructor
	Surface::Surface(SDL_Surface * surf)
		:pSurface_(surf)
	{}

	void Surface::AttachSurface(SDL_Surface * surf)
	{
		if(pSurface_ != nullptr)
			SDL_FreeSurface(pSurface_);
		pSurface_ = surf;
	}

	SDL_Surface * Surface::DetachSurface()
	{
		auto ret = pSurface_;
		pSurface_ = nullptr;
		return ret;
	}

	Surface::~Surface()
	{
		if(pSurface_ != nullptr)
			SDL_FreeSurface(pSurface_);
	}

	Surface & Surface::operator=(Surface that)
	{
		swap(that);
		return *this;
	}

	void Surface::swap(Surface & that)
	{
		using std::swap;
		swap(pSurface_, that.pSurface_);
	}

	void Surface::Flip()
	{
		SDL_Flip(pSurface_);
	}

	void Surface::Fill(Uint32 color)
	{
		SDL_FillRect(pSurface_,nullptr,color);
	}

	void Surface::FillRect(int dx, int dy, int dw, int dh, Uint32 color)
	{
		SDL_Rect dst_rect;
		dst_rect.x = dx;
		dst_rect.y = dy;
		dst_rect.w = dw;
		dst_rect.h = dh;

		SDL_FillRect(pSurface_,&dst_rect,color);
	}

	uint32_t Surface::MakeColor(uint8_t r, uint8_t g, uint8_t b) const
	{
		return SDL_MapRGB(pSurface_->format, r, g, b);
	}

	void Surface::Create(int w, int h, bool use_color_key)
	{
		if(pSurface_ != nullptr)
		{
			SDL_FreeSurface(pSurface_);
			pSurface_ = nullptr;
		}

		pSurface_ = SDL_Ex::CreateSurface(0, w, h);
		if(use_color_key)
			SDL_SetColorKey(pSurface_, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(pSurface_->format, 255, 0, 255));
	}

	void Surface::Create(int w, int h, const uint8_t * pixel_data, bool use_color_key)
	{
		if(pSurface_ != nullptr)
		{
			SDL_FreeSurface(pSurface_);
			pSurface_ = nullptr;
		}

		pSurface_ = SDL_Ex::CreateSurfaceFrom(0, w, h, pixel_data);
		if(use_color_key)
			SDL_SetColorKey(pSurface_, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(pSurface_->format, 255, 0, 255));		
	}

	void Surface::Load(const std::string & filename, bool use_color_key)
	{
		if(pSurface_ != nullptr)
		{
			SDL_FreeSurface(pSurface_);
			pSurface_ = nullptr;
		}

		SDL_Surface * temp = IMG_Load(filename.c_str());
		if(temp == nullptr)
		{
			std::string what = "Surface::Load(): Failed to load \"";
			what += filename + "\" : " + SDL_GetError();
			throw std::runtime_error(what);
		}

		if(use_color_key)
			SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(temp->format, 255, 0, 255));

		pSurface_ = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
	}

	void Surface::TransferTo(Surface & that)
	{
		if(that.pSurface_ != nullptr)
			SDL_FreeSurface(that.pSurface_);

		that.pSurface_ = pSurface_;
		pSurface_ = nullptr;
	}

	void Surface::BlitTo(DrawDestination & dest) const
	{
		dest.BlitFrom(*this);
		// SDL_BlitSurface(pSurface_, nullptr, dest.pSurface_, nullptr);
	}

	void Surface::BlitTo(DrawDestination & dest, int dx, int dy) const
	{
		dest.BlitFrom(*this, dx, dy);
	}

	void Surface::BlitFrom(const Surface & src)
	{
		SDL_BlitSurface(src.pSurface_, nullptr, pSurface_, nullptr);
	}

	void Surface::BlitFrom(const Surface & src, int dx, int dy)
	{
		SDL_Rect dst_rect;
		dst_rect.x = dx;
		dst_rect.y = dy;

		SDL_BlitSurface(src.pSurface_, nullptr, pSurface_, &dst_rect);
	}

	int Surface::GetWidth() const
	{
		return pSurface_->w;
	}

	int Surface::GetHeight() const
	{
		return pSurface_->h;
	}

	void Surface::PutPixel( int dx, int dy, Uint32 color )
	{
		SDL_Ex::PutPixel24(pSurface_, dx, dy, color);
	}

	void Surface::Rect( int dx, int dy, int width, int height, Uint32 color )
	{
		SDL_Ex::DrawRect(pSurface_, dx, dy, width, height, color);
	}

	void Surface::Line( int x, int y, int x2, int y2, Uint32 color )
	{
		SDL_Ex::Line(pSurface_, x, y, x2, y2, color);
	}

	void Surface::Circle( int cx, int cy, int radius, Uint32 color )
	{
		SDL_Ex::Circle(pSurface_, cx, cy, radius, color);
	}

	void Surface::FillCircle( int cx, int cy, int radius, Uint32 color )
	{
		SDL_Ex::FillCircle(pSurface_,cx,cy,radius,color);
	}

}
