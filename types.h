#ifndef TYPES_H
#define TYPES_H


/* Enums with a cooresponding texture are given unique number codes */

enum TurnDirection{
    RIGHT = 0,
    LEFT = 1
};

// relative directions: 0-1
enum MoveDirection{
    FORWARD = 0,
    BACKWARD = 1
};

#define DIRECTION_INDICATOR 10

// directions: 10-17    (directions use sprite sheet)
enum Direction{
    NORTH = 10,
    NORTHEAST = 11,
    EAST = 12, 
    SOUTHEAST = 13,
    SOUTH = 14,
    SOUTHWEST = 15,
    WEST = 16,
    NORTHWEST = 17
};

// tiles: 100-199
enum TileType{
    GREEN_GRASS = 100,
    DEAD_GRASS,
    WATER,
    NO_TILE
};

// features: 200-399
enum FeatureType{
    // wild: 2xx
    NO_FEATURE = 200,
    TREE,
    OLD_TREE,
    BUSH,
    FERN,
    LONG_GRASS,
    // agriculture: 3xx
    TILLED_SOIL = 300,
    CORN_STAGE_1,
    CORN_STAGE_2,
    CORN_STAGE_3,
    CORN_STAGE_4,
    CORN_STAGE_5
};

// items: 400-500
enum ItemType{
    NO_ITEM = 400
};

// controls: 500-599
enum ControlType {
    MAIN_MENU = 500,
    BUTTON,
    NO_CONTROL
};

// creatures: 900-999
enum PersonType{
    PLAYER = 900
};

#endif
