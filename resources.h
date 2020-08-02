#ifndef RESOURCES_H
#define RESOURCES_H

#include "types.h"
#include "utils.h"
#include <map>
#include <string>


struct FeatureDef{
public:
    //FeatureDef(FeatureType type, unsigned int texID, unsigned int subtex, unsigned int _passable)

    FeatureType type;
    unsigned int texID;
    std::string name;
    unsigned int subtex;
    //glm::vec2 _tex_dimensions;
    unsigned int passable;
    bool buildable;

    // health  (-1 for unbreakable)
    // build time 
    // build resources
    // array of tile types which can contain feature 
    // movement speed modifier

};


extern std::map<FeatureType, FeatureDef> feature_defs;



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
