#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static TileType TILE_MAPPINGS[] = {{.key = TILE_WATER,
                                    .raw_key = R_TILE_WATER,
                                    .variation = 1,
                                    .atlas_coords = {1, 1}},
                                   {
                                       .key = TILE_LAND,
                                       .raw_key = R_TILE_LAND,
                                       .variation = 1,
                                       .atlas_coords =

                                           {2, 2},
                                   },
                                   {
                                       .key = TILE_DIRT,
                                       .raw_key = R_TILE_DIRT,
                                       .variation = 2,
                                       .atlas_coords =

                                           {3, 3},
                                   },
                                   {
                                       .key = TILE_ROCK,
                                       .raw_key = R_TILE_ROCK,
                                       .variation = 2,
                                       .atlas_coords = {4, 4},
                                   }};

Tile raw_to_tile(RawTileKey raw_key) {
  Tile tile = {.elevation = 0,
               .texture_index_x = 0,
               .texture_index_y = 0,
               .raw_key = raw_key};

  for (size_t i = 0; i < 4; i++) {
    if (TILE_MAPPINGS[i].raw_key == raw_key) {
      tile.key = TILE_MAPPINGS[i].key;

      tile.texture_index_x = TILE_MAPPINGS[i].atlas_coords[0];
      tile.texture_index_y = TILE_MAPPINGS[i].atlas_coords[1];

      return tile;
    }
  }
  abort();
}
