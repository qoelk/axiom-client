#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static TileType TILE_MAPPINGS[] = {
    // Basic terrain tiles
    {.key = TILE_WATER,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {0, 0}},
    {.key = TILE_LAND,
     .raw_key = R_TILE_LAND,
     .variation = 1,
     .atlas_coords = {5, 0}},
    {.key = TILE_DIRT,
     .raw_key = R_TILE_DIRT,
     .variation = 2,
     .atlas_coords = {3, 3}},
    {.key = TILE_ROCK,
     .raw_key = R_TILE_ROCK,
     .variation = 2,
     .atlas_coords = {4, 4}},

    // Water-to-land transition tiles (water center with land edges)
    {.key = TILE_WATER_LAND_TL,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {0, 1}}, // Top-left corner (water with land in TL)
    {.key = TILE_WATER_LAND_ML,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {0, 2}}, // Middle-left side (water with land on left)
    {.key = TILE_WATER_LAND_BL,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {0, 3}}, // Bottom-left corner (water with land in BL)
    {.key = TILE_WATER_LAND_TM,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {1, 1}}, // Top-middle side (water with land on top)
    {.key = TILE_WATER_LAND_BM,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {1, 3}}, // Bottom-middle side (water with land on bottom)
    {.key = TILE_WATER_LAND_TR,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {2, 1}}, // Top-right corner (water with land in TR)
    {.key = TILE_WATER_LAND_MR,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {2, 2}}, // Middle-right side (water with land on right)
    {.key = TILE_WATER_LAND_BR,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {2, 3}}, // Bottom-right corner (water with land in BR)

    // Land-to-water transition tiles (land center with water edges)
    {.key = TILE_LAND_WATER_TL,
     .raw_key = R_TILE_LAND,
     .variation = 1,
     .atlas_coords = {1, 4}}, // Top-left corner (land with water in TL)
    {.key = TILE_LAND_WATER_ML,
     .raw_key = R_TILE_LAND,
     .variation = 1,
     .atlas_coords = {0, 5}}, // Middle-left side (land with water on left)
    {.key = TILE_LAND_WATER_BL,
     .raw_key = R_TILE_LAND,
     .variation = 1,
     .atlas_coords = {0, 4}}, // Bottom-left corner (land with water in BL)
    {.key = TILE_LAND_WATER_TM,
     .raw_key = R_TILE_LAND,
     .variation = 1,
     .atlas_coords = {2, 4}}, // Top-middle side (land with water on top)
    {.key = TILE_LAND_WATER_BM,
     .raw_key = R_TILE_LAND,
     .variation = 1,
     .atlas_coords = {1, 5}}, // Bottom-middle side (land with water on bottom)
    {.key = TILE_LAND_WATER_TR,
     .raw_key = R_TILE_LAND,
     .variation = 1,
     .atlas_coords = {2, 5}}, // Top-right corner (land with water in TR)
    {.key = TILE_LAND_WATER_MR,
     .raw_key = R_TILE_LAND,
     .variation = 1,
     .atlas_coords = {3, 4}}, // Middle-right side (land with water on right)
    {.key = TILE_LAND_WATER_BR,
     .raw_key = R_TILE_LAND,
     .variation = 1,
     .atlas_coords = {3, 5}}, // Bottom-right corner (land with water in BR)
};
Tile raw_to_tile(RawTileKey raw_key) {
  Tile tile = {.elevation = 0,
               .texture_index_x = 0,
               .texture_index_y = 0,
               .raw_key = raw_key};

  for (size_t i = 0; i < 20; i++) {
    if (TILE_MAPPINGS[i].raw_key == raw_key) {
      tile.key = TILE_MAPPINGS[i].key;

      tile.texture_index_x = TILE_MAPPINGS[i].atlas_coords[0];
      tile.texture_index_y = TILE_MAPPINGS[i].atlas_coords[1];

      return tile;
    }
  }
  abort();
}
