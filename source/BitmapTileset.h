#ifndef ZIK_BITMAP_TILESET_905
#define ZIK_BITMAP_TILESET_905

#include <vector>
#include <cstdint>

#include "Surface.h"

namespace Zik
{

	class BitmapTileset
	{
	public:
		BitmapTileset() {}
		BitmapTileset(const Surface & source, int tile_width, int tile_height, int columns, int rows, bool padding);
		BitmapTileset(const uint8_t * pixel_data, int tile_width, int tile_height, int tile_ct);
		void Create(const Surface & source, int tile_width, int tile_height, int columns, int rows, bool padding);
		void Create(const uint8_t * pixel_data, int tile_width, int tile_height, int tile_ct);
		void BlitTile(DrawDestination & dest, int dx, int dy, int tile) const;
		int GetNumTiles() const { return surfaces_.size(); }
		int TileWidth() const { return tile_width_; }
		int TileHeight() const { return tile_height_; }
	private:
		std::vector<Surface> surfaces_;

		int tile_width_;
		int tile_height_;
	};

}
#endif // #ifndef ZIK_BITMAP_TILESET_905