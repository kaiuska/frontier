#include "tile.h"


Tile::Tile(){}

Tile::Tile(glm::vec2 pos, glm::vec2 size, TileType type, FeatureType feature_type)
    : Sprite(pos, size, textures[type])
{
    if(_type == NO_TILE){
        return;
    }
    _type = type;


    _feature_type = feature_type;

    _elevation = 0;
    _feature.create(_pos, _feature_type);
}

void Tile::create(glm::vec2 pos, glm::vec2 size, TileType type, FeatureType feature_type)
{
    _type = type;
    _feature_type = feature_type;
    _elevation = 0;

    Sprite::create(pos, size, textures[type]);
    _feature.create(_pos, _feature_type);
}

void Tile::draw(Shader shader)
{
    Sprite::draw(shader);

    _feature.draw(shader);
}


void Tile::set_feature(FeatureType feature)
{
    _feature.create(_pos, feature);
}




FeatureType Tile::get_feature_type()
{
    return _feature_type;
}



TileType Tile::get_type()
{
    return _type;
}


void Tile::set_elevation(int elevation)
{
    _elevation = elevation <= MAX_ELEVATION ? elevation : MAX_ELEVATION;
    _elevation = elevation >= MIN_ELEVATION ? elevation : MIN_ELEVATION;
    _pos.y += _elevation*VOXEL_HEI/3;
    _feature.set_elevation(_elevation);
}

int Tile::get_elevation()
{
    return _elevation;
}
void Tile::set_world_pos(int x, int y)
{
    _world_pos = glm::ivec2(x, y);
}
glm::ivec2 Tile::get_world_pos()
{
    return _world_pos;
}
