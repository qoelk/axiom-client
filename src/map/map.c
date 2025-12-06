#include "map.h"
#include <stdbool.h>
#include <stddef.h>
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
    {.key = TILE_WATER_LAND_TL_L_T,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {2, 0}},
    {.key = TILE_WATER_LAND_TL,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {0, 1}},
    {.key = TILE_WATER_LAND_L,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {0, 1}}, // Middle-left side (water with land on left)
    {.key = TILE_WATER_LAND_BL_L_B,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {2, 2}}, // Bottom-left corner (water with land in BL)
    {.key = TILE_WATER_LAND_BL,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {0, 2}},
    {.key = TILE_WATER_LAND_T,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {3, 0}}, // Top-middle side (water with land on top)
    {.key = TILE_WATER_LAND_B,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {3, 2}}, // Bottom-middle side (water with land on bottom)
    {.key = TILE_WATER_LAND_TR_R_T,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {4, 0}}, // Top-right corner (water with land in TR)
    {.key = TILE_WATER_LAND_TR,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {1, 1}},
    {.key = TILE_WATER_LAND_R,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {4, 1}}, // Middle-right side (water with land on right)
    {.key = TILE_WATER_LAND_BR_R_B,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {4, 2}}, // Bottom-right corner (water with land in BR)
    {.key = TILE_WATER_LAND_BR,
     .raw_key = R_TILE_WATER,
     .variation = 1,
     .atlas_coords = {1, 2}},
};

Tile raw_to_tile(RawTileKey raw_key) {
  Tile tile = {.elevation = 0,
               .texture_index_x = 0,
               .texture_index_y = 0,
               .raw_key = raw_key};

  for (size_t i = 0; i < sizeof(TILE_MAPPINGS) / sizeof(TILE_MAPPINGS[0]);
       i++) {
    if (TILE_MAPPINGS[i].raw_key == raw_key) {
      tile.key = TILE_MAPPINGS[i].key;
    }
  }
  update_coordinates(&tile);
  return tile;
}

void update_coordinates(Tile *tile) {
  for (size_t i = 0; i < sizeof(TILE_MAPPINGS) / sizeof(TILE_MAPPINGS[0]);
       i++) {
    if (TILE_MAPPINGS[i].key == tile->key) {
      tile->texture_index_x = TILE_MAPPINGS[i].atlas_coords[0];
      tile->texture_index_y = TILE_MAPPINGS[i].atlas_coords[1];
      return;
    }
  }
  abort();
}

TileKey get_neighbor_at_offset(TileMap *map, int x, int y, int dx, int dy) {
  int nx = x + dx;
  int ny = y + dy;

  // Check bounds
  if (nx < 0 || nx >= map->width || ny < 0 || ny >= map->height) {
    return TILE_UNKNOWN;
  }

  int idx = ny * map->width + nx;
  return map->tiles[idx].key;
}

void preprocess_map(TileMap *map) {
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      Tile tile = map->tiles[y * map->width + x];
      if (tile.raw_key != R_TILE_WATER) {
        continue;
      }

      // Get neighbor tile keys with correct offsets
      TileKey tl = get_neighbor_at_offset(map, x, y, -1, -1); // top-left ✓
      TileKey t = get_neighbor_at_offset(map, x, y, 0, -1);   // top ✓
      TileKey tr =
          get_neighbor_at_offset(map, x, y, 1, -1); // top-right (was -1, 1)
      TileKey l = get_neighbor_at_offset(map, x, y, -1, 0); // left (was 0, -1)
      TileKey r = get_neighbor_at_offset(map, x, y, 1, 0);  // right ✓
      TileKey bl =
          get_neighbor_at_offset(map, x, y, -1, 1); // bottom-left (was 1, -1)
      TileKey b = get_neighbor_at_offset(map, x, y, 0, 1);  // bottom ✓
      TileKey br = get_neighbor_at_offset(map, x, y, 1, 1); // bottom-right ✓
      // Convert to booleans indicating if neighbor is land
      bool is_tl_land = (tl == TILE_LAND);
      bool is_t_land = (t == TILE_LAND);
      bool is_tr_land = (tr == TILE_LAND);
      bool is_l_land = (l == TILE_LAND);
      bool is_r_land = (r == TILE_LAND);
      bool is_bl_land = (bl == TILE_LAND);
      bool is_b_land = (b == TILE_LAND);
      bool is_br_land = (br == TILE_LAND);

      TileKey key = TILE_WATER;

      // Top-left corner logic
      if (is_tl_land) {
        key = (is_t_land && is_l_land) ? TILE_WATER_LAND_TL_L_T
                                       : TILE_WATER_LAND_TL;
      }

      // Top edge (only if no corner was set)
      if (is_t_land && key == tile.key) {
        key = TILE_WATER_LAND_T;
      }

      // Top-right corner (only if no previous corner was set)
      if (is_tr_land && key == tile.key) {
        key = (is_t_land && is_r_land) ? TILE_WATER_LAND_TR_R_T
                                       : TILE_WATER_LAND_TR;
      }

      // Left edge (only if no corner was set)
      if (is_l_land && key == tile.key) {
        key = TILE_WATER_LAND_L;
      }

      // Right edge (only if no corner was set)
      if (is_r_land && key == tile.key) {
        key = TILE_WATER_LAND_R;
      }

      // Bottom-left corner (only if no previous corner was set)
      if (is_bl_land && key == tile.key) {
        key = (is_b_land && is_l_land) ? TILE_WATER_LAND_BL_L_B
                                       : TILE_WATER_LAND_BL;
      }

      // Bottom edge (only if no corner was set)
      if (is_b_land && key == tile.key) {
        key = TILE_WATER_LAND_B;
      }

      // Bottom-right corner (only if no previous corner was set)
      if (is_br_land && key == tile.key) {
        key = (is_b_land && is_r_land) ? TILE_WATER_LAND_BR_R_B
                                       : TILE_WATER_LAND_BR;
      }

      map->tiles[y * map->width + x].key = key;
      update_coordinates(&map->tiles[y * map->width + x]);
    }
  }
}
