#ifndef _TILE_H_
#define _TILE_H_
#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "feature.h"
#include "utils.h"
#include "sprite.h"




class Tile : public Sprite
{
public:
    Tile();
    Tile(glm::vec2 pos, glm::vec2 size, TileType type, FeatureType feature_type);
    void create(glm::vec2 pos, glm::vec2 size, TileType type, FeatureType feature_type);
    void draw(Shader shader);
    FeatureType get_feature_type();
    void set_feature(FeatureType feature);
    Feature _feature;
    TileType get_type();
    void set_elevation(int elevation);
    int get_elevation();
    void set_world_pos(int x, int y);
    glm::ivec2 get_world_pos();

    TileData* get_tile_data(); 

private:
    glm::ivec2 _world_pos;
    TileType _type;
    FeatureType _feature_type;
    int _elevation;

};



#endif
