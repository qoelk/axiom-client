#ifndef MAP_H
#define MAP_H

typedef enum { R_TILE_WATER, R_TILE_LAND, R_TILE_DIRT, R_TILE_ROCK } RawTileKey;

typedef enum {
  TILE_WATER,
  TILE_LAND,
  TILE_DIRT,
  TILE_ROCK,

  TILE_WATER_LAND_TL,
  TILE_WATER_LAND_ML,
  TILE_WATER_LAND_BL,
  TILE_WATER_LAND_TM,
  TILE_WATER_LAND_BM,
  TILE_WATER_LAND_TR,
  TILE_WATER_LAND_MR,
  TILE_WATER_LAND_BR,

} TileKey;

typedef struct {
  int width;
  int height;
  RawTileKey *tiles;
} RawTileMap;

typedef struct {
  RawTileKey raw_key;
  TileKey key;
  int variation;
  int atlas_coords[2];
} TileType;

typedef struct {
  RawTileKey raw_key;
  TileKey key;
  int elevation;
  int texture_index_x;
  int texture_index_y;
} Tile;

typedef struct {
  int width;
  int height;
  Tile *tiles;
} TileMap;

Tile raw_to_tile(RawTileKey raw_key);
#endif
