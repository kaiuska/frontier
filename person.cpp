#include "person.h"

Person::Person(){};
Person::Person(glm::ivec2 tile, glm::vec2 pos, PersonType type)
{
    init(tile, pos, type);
}

void Person::init(glm::ivec2 tile, glm::vec2 pos, PersonType type)
{
    _facing = SOUTH;
    _tile = tile;

    Sprite::create(pos, glm::vec2(TILE_WID, SCALE_TO_TEX), textures[type]);

    glm::vec2 ind_pos(pos.x, pos.y+20);

    _direction_indicator.create(ind_pos, glm::vec2(TILE_WID, SCALE_TO_TEX), textures[DIRECTION_INDICATOR]);
    _direction_indicator.set_subtex(_facing - NORTH);

    _direction_indicator.set_position(ind_pos);
}


void Person::draw(Shader shader)
{
    //get_tex_dimensions();
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glActiveTexture(GL_TEXTURE0);


    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(TILE_WID, _tex_dimensions.y, 1.0f));

    //model = glm::translate(model, glm::vec3(_pos.x/feature_wid, _pos.y/_tex_dimensions.y, _pos.z));
    model = glm::translate(model, glm::vec3(_pos.x/TILE_WID, _pos.y/_tex_dimensions.y, 0.0f));

    float t_wid = TILE_WID/_tex_dimensions.x;

    shader.setInt("subtex", _subtex);
    shader.setFloat("tex_wid", t_wid);
    shader.setMat4("model", model);


    glBindVertexArray(_vaoID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 


    if(_facing != NORTH)
        _direction_indicator.draw(shader);
}

void Person::move(glm::ivec2 tile, glm::vec2 pos)
{
    glm::vec2 new_pos(pos.x, pos.y + _tex_dimensions.y-35);
    set_position(new_pos);
    //printf("move %f %f\n", new_pos.x, new_pos.y);
    _tile = tile;
    glm::vec2 ind_pos(pos.x, pos.y+20);
    _direction_indicator.set_position(ind_pos);
}

void Person::turn(TurnDirection dir)
{
    if(dir == RIGHT)
        _facing = (Direction)(_facing + 1);
    else
        _facing = (Direction)(_facing - 1);
    if(_facing < NORTH)
        _facing = NORTHWEST;
    if(_facing > NORTHWEST)
        _facing = NORTH;
    printf("Direction: %d\n", _facing);

    _direction_indicator.set_subtex(_facing - NORTH);
}

glm::ivec2 Person::get_tile()
{
    return _tile;
}

Direction Person::facing()
{
    return _facing;
}
