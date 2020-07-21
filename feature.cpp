#include "feature.h"



Feature::Feature(){}


Feature::Feature(glm::vec2 pos, FeatureType feature)
{
    _type = feature;
    _textureID = textures[_type];

    _vaoID = vertex_arrays[_type];
    set_position(pos);


    _elevation = 0;
    _tex_dimensions = get_tex_dimensions();
    set_size(glm::vec2(TILE_WID, _tex_dimensions.y));
}


void Feature::create(glm::vec2 pos, FeatureType feature)
{
    _type = feature;
    //printf("feature_position: %f %f %f\n", pos.x, pos.y, pos.z);
    _textureID = textures[_type];

    _tex_dimensions = get_tex_dimensions();
    set_position(glm::vec2(pos.x, pos.y + _tex_dimensions.y - TILE_SURFACE_HEI));

    _size = glm::vec2(TILE_WID, _tex_dimensions.y);
    Sprite::create(_pos, _size, _textureID);

    _elevation = 0;

}


void Feature::draw(Shader shader)
{
    Sprite::draw(shader);
}


void Feature::set_position(glm::vec2 pos)
{
    _pos = pos; 
    //set_elevation(_elevation);
}



void Feature::set_elevation(int elevation)
{
    _elevation = elevation;
    _pos.y += _elevation*VOXEL_HEI/3;
}




