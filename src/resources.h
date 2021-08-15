#ifndef RESOURCES_H
#define RESOURCES_H

#include "types.h"
#include "utils.h"
#include <map>
#include <string>


class FeatureDef{
public:
    FeatureType type;
    unsigned int texID;
    std::string name;
    unsigned int subtex;
    unsigned int passable;
    bool buildable;
};


class ItemDef{
public:
    ItemType type;
    unsigned int texID;
    std::string name;
    unsigned int subtex;
    unsigned int passable;
    bool buildable;
};
extern std::map<FeatureType, FeatureDef> feature_defs;





extern std::map<ItemType, ItemDef> item_defs;



extern TileType tile_list[];
extern FeatureType feature_list[];
extern ControlType control_list[];
extern PersonType person_list[];



glm::vec2 get_tex_dimensions(unsigned int textureID);
void load_font(const char* font, int hei);
unsigned int load_texture(const char *fname, int alpha);
void init_vertex_arrays();

void init_textures();

#endif
