#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Initialize random seed if not already done
static int rand_initialized = 0;

TileType TILE_MAPPINGS[] = {{.key = TILE_WATER,
                             .raw_key = R_TILE_WATER,
                             .variation = 1,
                             .atlas_coords = {{0, 0}}},
                            {.key = TILE_LAND,
                             .raw_key = R_TILE_LAND,
                             .variation = 1,
                             .atlas_coords =
                                 {
                                     {0, 6},
                                 }},
                            {.key = TILE_DIRT,
                             .raw_key = R_TILE_DIRT,
                             .variation = 2,
                             .atlas_coords = {{4, 3}, {4, 4}}},
                            {.key = TILE_ROCK,
                             .raw_key = R_TILE_ROCK,
                             .variation = 2,
                             .atlas_coords = {{5, 3}, {5, 4}}}};

Tile raw_to_tile(RawTileKey raw_key) {
  Tile tile = {0};
  tile.raw_key = raw_key;
  tile.elevation = 0;

  // Find the corresponding TileType in TILE_MAPPINGS
  for (size_t i = 0; i < sizeof(TILE_MAPPINGS) / sizeof(TILE_MAPPINGS[0]);
       i++) {
    if (TILE_MAPPINGS[i].raw_key == raw_key) {
      tile.type = TILE_MAPPINGS[i];
      tile.key = TILE_MAPPINGS[i].key;

      tile.texture_index_x = TILE_MAPPINGS[i].atlas_coords[0][0];
      tile.texture_index_y = TILE_MAPPINGS[i].atlas_coords[0][1];

      return tile;
    }
  }

  // Fallback if tile type not found
  tile.raw_key = raw_key;
  tile.key = TILE_LAND;
  tile.elevation = 0;

  // Use first grass tile as fallback
  tile.texture_index_x = 3;
  tile.texture_index_y = 3;

  // Try to copy a default type if possible
  if (sizeof(TILE_MAPPINGS) > 0) {
    tile.type = TILE_MAPPINGS[1]; // Use land type as fallback (index 1)
  }

  fprintf(
      stderr,
      "Warning: Raw tile key %d not found in TILE_MAPPINGS, using fallback\n",
      raw_key);
  return tile;
}
