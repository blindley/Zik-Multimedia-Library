#include "Label.h"

namespace Zik
{

	Label::Label(DrawDestination & dest, const Font & font)
		:dest_(dest), font_(font), dx_(0), dy_(0), justification_(LEFT)
	{
		surface_ = font_.MakeSurface(text_);
	}

	Label::Label(DrawDestination & dest, const Font & font, int dx, int dy)
		:dest_(dest), font_(font), dx_(dx), dy_(dy), justification_(LEFT)
	{
		surface_ = font_.MakeSurface(text_);
	}

	void Label::SetText(std::string text)
	{
		if(text == text_)
			return;
		text_ = text;
		surface_ = font_.MakeSurface(text_);
	}

	std::string Label::GetText() const
	{
		return text_;
	}

	void Label::Draw() const
	{
		int x, y;
		switch(justification_)
		{
		case LEFT: x = dx_; y = dy_; break;
		case CENTER: x = dx_ - surface_.GetWidth()/2; y = dy_; break;
		case RIGHT: x = dx_ - surface_.GetWidth(); y = dy_; break;
		}

		dest_.BlitFrom(surface_, x, y);
	}

	void Label::CenterJustify(int x, int y)
	{
		dx_ = x;
		dy_ = y;
		justification_ = CENTER;
	}

	void Label::LeftJustify(int x, int y)
	{
		dx_ = x;
		dy_ = y;
		justification_ = LEFT;
	}

	void Label::RightJustify(int x, int y)
	{
		dx_ = x;
		dy_ = y;
		justification_ = RIGHT;
	}

}