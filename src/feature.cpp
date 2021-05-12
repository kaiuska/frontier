#include "feature.h"



Feature::Feature(){}


Feature::Feature(glm::vec2 pos, FeatureType feature)
{

    create(pos, feature);
}


void Feature::create(glm::vec2 pos, FeatureType feature)
{
    _type = feature;
    _vaoID = vertex_arrays[_type];

    _textureID = feature_defs[_type].texID;
    //_subtex = feature_defs[_type].subtex;
    //_textureID = feature_defs[_type].texID;



    _tex_dimensions = get_tex_dimensions();
    set_position(glm::vec2(pos.x, pos.y + _tex_dimensions.y - TILE_SURFACE_HEI));

    set_size(glm::vec2(TILE_WID, _tex_dimensions.y));
    Sprite::create(_pos, _size, _textureID, feature_defs[_type].subtex);
    //set_subtex(feature_defs[_type].subtex);

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




FeatureType Feature::get_type()
{
    return _type;
}
