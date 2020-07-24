#ifndef ASSET_MANAGEMENT_H
#define ASSET_MANAGEMENT_H

#include "types.h"
#include "utils.h"
extern TileType tile_list[];
extern FeatureType feature_list[];
extern ControlType control_list[];
extern PersonType person_list[];


void load_font(const char* font, int hei);
unsigned int load_texture(const char *fname, int alpha);
void init_vertex_arrays();

void init_textures();

#endif
