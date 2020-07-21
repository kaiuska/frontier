#ifndef _MAP_H_
#define _MAP_H_
#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "utils.h"
#include "sprite.h"
#include "tile.h"
#include "person.h"


class Map
{
public:
    Map(int tilesx, int tilesy);
    void init();

    void draw(Shader& shader);

    void turn_player(Turn dir);
    void move_player(MoveDirection direction);

    void zoom(float inc);
    glm::ivec2 neighboring_tile(glm::ivec2 start, Direction dir);

private:
    void generate_tile(int x, int y, int worldx, int worldy);

private:
    Tile*** _tiles;
    glm::ivec2 _center;
    glm::ivec2 _world_pos;
    glm::ivec2 _dimensions;
    Person _player;
    float _zoom;
    int _tiles_to_render;

    glm::vec3 _camera_pos;
    glm::vec3 _camera_front;
    glm::vec3 _camera_up;
};

































#endif
