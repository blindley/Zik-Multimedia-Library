#include <iterator>

#include "BitmapTileset.h"

namespace Zik
{

	BitmapTileset::BitmapTileset(const Surface & source, int tile_width, int tile_height, int columns, int rows, bool padding)
	{
		Create(source, tile_width, tile_height, columns, rows, padding);
	}

	BitmapTileset::BitmapTileset(const uint8_t * pixel_data, int tile_width, int tile_height, int tile_ct)
	{
		Create(pixel_data, tile_width, tile_height, tile_ct);
	}

	void BitmapTileset::Create(const Surface & source, int tile_width, int tile_height, int columns, int rows, bool padding)
	{
		tile_width_ = tile_width;
		tile_height_ = tile_height;
		surfaces_.reserve(tile_width * tile_height);
		source.ExtractFrames(std::back_inserter(surfaces_), tile_width, tile_height, columns, rows, padding);
	}

	void BitmapTileset::Create(const uint8_t * pixel_data, int tile_width, int tile_height, int tile_ct)
	{
		tile_width_ = tile_width;
		tile_height_ = tile_height;
		surfaces_.reserve(tile_ct);
		for(int i=0; i<tile_ct; ++i)
		{
			surfaces_.emplace_back(Surface(tile_width, tile_height, pixel_data, true));
			pixel_data += tile_width * tile_height * 3;
		}
	}

	void BitmapTileset::BlitTile(DrawDestination & dest, int dx, int dy, int tile_index) const
	{
		dest.BlitFrom(surfaces_[tile_index], dx, dy);
	}

}