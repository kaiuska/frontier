#ifndef _UTILS_H_
#define _UTILS_H_


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "stb/stb_image.h"

#include <iostream>
#include <cstdio>
#include <map>
#include <random>
#include <chrono>
#include <algorithm>

#include "resources.h"
#include "shader.h"
#include "types.h"

// default sizes
extern const int CHUNK_SIZE;

extern const int TILESX;
extern const int TILESY;

extern const int DEFAULT_WID;
extern const int DEFAULT_HEI;

// turn length of 5 seconds
extern const float TURN_LENGTH;

extern short *rands;

extern bool mouse_clicked;
extern glm::vec2 click_pos;
extern glm::vec2 mouse_pos;


extern float scr_wid;
extern float scr_hei; 
extern float zoom;
extern const float ZOOM_INC;
extern const float MAX_ZOOM;
extern const float MIN_ZOOM;

extern float TILE_WID;
extern float TILE_HEI;
extern const float TILE_SURFACE_HEI; 
extern const float VOXEL_HEI; // verticlal step in world space per unit elevation

extern const float ELEVATION_OFFSET;
extern const int MAX_ELEVATION; 
extern const int MIN_ELEVATION;
extern const int WATER_LEVEL;



extern float vertices[8];
extern float texcoords[8];
extern unsigned int indices[6];


struct TileData {
    TileType type;
    FeatureType feature;

    glm::ivec2 world_pos;
    int num_items;
    ItemType* items;
    int elevation;
};

extern const float text_hei;

// Text characters
struct Character {
    unsigned int textureID;
    glm::ivec2 size;        // size
    glm::ivec2 bearing;     // position 
    long int advance;   // distance to next character
};


extern std::map<char, Character> characters;
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



short *init_rands(int seed_size);
glm::vec3 iso_to_cartesian(glm::vec3 iso);
glm::vec2 place_tile(glm::vec2 cart, float wid, float hei);
void render_text(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);
void init_textures();

Direction reverse_direction(Direction dir);

glm::vec2 grad(glm::vec2 p);
float fade(float t);
float noise(glm::vec2 p);









#endif
