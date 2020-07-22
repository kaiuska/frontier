#ifndef _UTILS_H_
#define _UTILS_H_


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"
#include <GLFW/glfw3.h>


#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <cstdio>
#include <map>

#include "shader.h"
#include "stb/stb_image.h"
//#include "rands.h"
//
//


//default sizes
#define DEFAULT_HEI 800
#define DEFAULT_WID 600

#define TILESX 80
#define TILESY 80

// turn length of 5 seconds
extern const float TURN_LENGTH;

extern short rands[];


extern float scr_wid;
extern float scr_hei; 
extern float menu_wid;
extern float zoom;
extern const float ZOOM_INC;
extern const float MAX_ZOOM;
extern const float MIN_ZOOM;

extern float TILE_WID;
extern float TILE_HEI;
extern const float TILE_SURFACE_HEI; 
extern const float VOXEL_HEI; // verticlal step in world space per unit elevation


//extern int tiles_to_render;

extern const float ELEVATION_OFFSET;
extern const int MAX_ELEVATION; 
extern const int MIN_ELEVATION;

extern float game_time;




/* Enums with a cooresponding texture are given unique number codes */

// relative directions: 0-1
enum Turn{
    RIGHT = 0,
    LEFT = 1
};

enum MoveDirection{
    FORWARD = 0,
    BACKWARD = 1
};

// directions: 10-17    (directions use sprite sheet)
#define DIRECTION_INDICATOR 10


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
extern TileType tile_list[];


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
extern FeatureType feature_list[];

enum ItemType{
    NO_ITEM = 400
};
struct TileData {
    TileType _type;
    FeatureType _feature;
    glm::ivec2 _world_pos;
    int _num_items;
    ItemType* _items;
    int _elevation;
};

/*
struct TileChange {
    glm::ivec2 _world_pos;
};
*/


// controls: 500-599
enum ControlType {
    MAIN_MENU = 500,
    BUTTON,
    NO_CONTROL
};
extern ControlType control_list[];

//extern std::map<int, TileDef> tile_definitions;

// creatures: 900-999
enum PersonType{
    PLAYER = 900
};
extern PersonType person_list[];


extern const float text_hei;

struct Character {
    unsigned int textureID;
    glm::ivec2 size;        // size
    glm::ivec2 bearing;     // position 
    long int advance;   // distance to next character
};


extern std::map <char, Character> characters;
extern std::map<unsigned int, unsigned int> textures;
extern std::map<unsigned int, unsigned int> vertex_arrays;


// Menu
extern glm::vec3 menu_bg_color;
extern const glm::vec3 text_color;
extern glm::vec3 button_bg_color;

// Global vertex arrays
extern unsigned int text_vao;
extern unsigned int text_vbo;

extern unsigned int sprite_vao;
extern unsigned int sprite_vbo;



void load_font(const char* font, int hei);
unsigned int load_texture(const char *fname, int alpha);
glm::vec3 iso_to_cartesian(glm::vec3 iso);
glm::vec2 place_tile(glm::vec2 cart, float wid, float hei);
void render_text(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);

glm::vec2 grad(glm::vec2 p);
float fade(float t);
float noise(glm::vec2 p);
#endif
