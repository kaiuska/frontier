#ifndef _MAP_H_
#define _MAP_H_
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "shader.h"
#include "utils.h"
#include "sprite.h"
#include "tile.h"
#include "person.h"


class Map
{
public:
    Map(int worldx, int worldy, int tilesx, int tilesy);
    Map(){};
    ~Map();
    void create(int worldx, int worldy, int tilesx, int tilesy);
    void init();

    void draw(Shader& shader);

    void turn_player(TurnDirection dir);
    void move_player(MoveDirection direction);

    void zoom(float inc);
    glm::ivec2 adjacent_tile();

    FeatureType get_feature(glm::ivec2 tile);
    void change_feature(glm::ivec2 tile, FeatureType new_feature);

    void change_tile(glm::ivec2 tile, TileType new_tile);
    FeatureType get_type();



private:
    void generate_tile(int x, int y, int worldx, int worldy);
    bool save_changes();

private:
    std::string _name;

    Tile*** _tiles;
    glm::ivec2 _center;
    glm::ivec2 _world_pos;
    glm::ivec2 _chunk;
    glm::ivec2 _dimensions;
    Person _player;
    int _player_elevation;

    std::map<std::pair<int, int>, TileData> _changes;

    float _zoom;
    int _tiles_to_render;

    glm::vec3 _camera_pos;
    glm::vec3 _camera_front;
    glm::vec3 _camera_up;
};

































#endif
