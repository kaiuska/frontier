#ifndef TYPES_H
#define TYPES_H


/* Enums with a cooresponding texture are given unique number codes */

enum GameState{
    NORMAL,
    AWAITING_ACTION,
    CRASH
};

enum ActionType{
    NONE = 0,
    WAITING,
    CHOP,
    BUILD,
    PLANT,
    TILL
};


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

// directions: 0 - 7   
enum Direction{
    NORTH = 0,
    NORTHEAST = 1,
    EAST = 2, 
    SOUTHEAST = 3,
    SOUTH = 4,
    SOUTHWEST = 5,
    WEST = 6,
    NORTHWEST = 7
};

// tiles: 100-199
enum TileType{
    GREEN_GRASS = 100,
    DEAD_GRASS,
    TILLED_SOIL,
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

    // buildings 250-299
    CABIN_WALL = 250,
    CABIN_WALL_NORTH_CORNER = 251,
    CABIN_WALL_NORTHEAST,
    CABIN_WALL_EAST_CORNER,
    CABIN_WALL_SOUTHEAST,
    CABIN_WALL_SOUTH_CORNER,
    CABIN_WALL_SOUTHWEST,
    CABIN_WALL_WEST_CORNER,
    CABIN_WALL_NORTHWEST,


    // agriculture: 3xx
    CORN = 300,
    CORN_STAGE_0 = 300,
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
