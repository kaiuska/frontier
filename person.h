#ifndef _PERSON_H_ 
#define _PERSON_H_
#include "utils.h"
#include "sprite.h"




/*
class DirectionIndicator : public Sprite
{
}
*/

class Person : public Sprite
{
public:
    Person();
    Person(glm::ivec2 tile, glm::vec2 pos, PersonType type);
    void init(glm::ivec2 tile, glm::vec2 pos, PersonType type);
    void draw(Shader shader);
    void move(glm::ivec2 tile, glm::vec2 pos);
    void turn(Turn dir);
    glm::ivec2 get_tile();
    Direction facing();

protected:
    PersonType _type;
    glm::ivec2 _tile;
    Direction _facing;
    Sprite _direction_indicator;

};

#endif

