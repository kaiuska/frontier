#include "map.h"



Map::Map(int tilesx, int tilesy)
    : _dimensions(tilesx, tilesy), 
      _center(tilesx/2, tilesy/2),
      _world_pos(tilesx/2, tilesy/2),
      //_world_pos(0,0),
      _zoom(1.0f),
      _tiles_to_render(40)
{
    init();

    _camera_pos = glm::vec3(_tiles[(int)_center.x][(int)_center.y]->get_position(), 1.0f);

    _camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    _camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

    _player.init(_center, _camera_pos, PLAYER);

    //printf("player tile: %d %d\n", _player.get_tile().x, _player.get_tile().y);
    //printf("center %d %d\n", _center.x, _center.y);
}

void Map::create(int tilesx, int tilesy)
{
    _dimensions = glm::ivec2(tilesx, tilesy);
    _center = glm::ivec2(tilesx/2, tilesy/2);
    _world_pos = glm::ivec2(tilesx/2, tilesy/2);
    _zoom = 1.0f;
    _tiles_to_render = 40;

    init();
    _camera_pos = glm::vec3(_tiles[(int)_center.x][(int)_center.y]->get_position(), 1.0f);
    _camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    _camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

    _player.init(_center, _camera_pos, PLAYER);
}

void Map::draw(Shader& shader)
{
    
    glm::mat4 view(1.0f); 
    view = glm::lookAt(_camera_pos, _camera_pos + _camera_front, _camera_up);
    glm::mat4 projection = glm::ortho(
            _zoom*-(float)scr_wid/2, 
            _zoom*(float)scr_wid/2, 
            _zoom*-(float)scr_hei/2, 
            _zoom*(float)scr_hei/2, 
            -20.0f, 10.0f); 

    glm::mat4 model = glm::mat4(1.0f);

    shader.use();

    int beg_x = 0;
    int beg_y = 0;
    int end_x = _dimensions.x;
    int end_y = _dimensions.y;

    for(int x = beg_x; x < end_x; x++){
        for(int y = beg_y; y < end_y; y++){

            shader.setMat4("view", view);
            shader.setMat4("projection", projection);

            _tiles[x][y]->draw(shader);
            if(x == _dimensions.x/2 && y == _dimensions.y/2){
                _player.draw(shader);
            }
        }
    }
}


void Map::turn_player(TurnDirection dir)
{
   _player.turn(dir); 
}





void Map::init()
{
    srand(534);
    _tiles = new Tile**[_dimensions.x];

    for(int x = 0; x < _dimensions.x; x++){
        _tiles[x] = new Tile*[_dimensions.y];

        for(int y = 0; y < _dimensions.y; y++){

            _tiles[x][y] = new Tile();
            generate_tile(x, y, x, y);
        }
    }
}

void Map::generate_tile(int x, int y, int worldx, int worldy)
{
    //glm::vec2 v((float)(500+worldx)*0.15f, (float)(worldy)*0.15f);
    //glm::vec2 v((float)(500+worldx)*0.1f, (float)(worldy)*0.1f);
    glm::vec2 v((float)(500+worldx)*0.025f, (float)(worldy)*0.025f);

    float r =  
        noise(v * (1.0f/30)) * 3.0f + 
        noise(v * (1.0f/25)) * 0.8f +
        //noise(v * (1.0f/17)) * 1.3f +
        noise(v * (1.0f/13)) * 0.6f +
        noise(v * (1.0f/9)) * 0.4f +
        noise(v * (1.0f/4)) * 0.2f;


    r *= (2*r-1) * (MAX_ELEVATION);
    

    int elevation = round(r); 

    //printf("noise: %f\n", r);
    if(elevation > MAX_ELEVATION)
        elevation = MAX_ELEVATION;
    if(elevation < MIN_ELEVATION)
        elevation = MIN_ELEVATION;


    TileType tile = GREEN_GRASS;
    FeatureType feature = NO_FEATURE;

    if(elevation <= 4){
        tile = WATER;
        elevation = 4;
    }else{
        
        if(noise(v * (1.0f/20.0f)) * 20.0f > 0.5)
            tile = DEAD_GRASS;

        if(_changes.find(std::pair<int, int>(worldx,worldy)) != _changes.end()){
            //feature = it; 
            feature = _changes[std::pair<int, int>(x,y)];
            printf("loading tile\n");
        }else{

            glm::vec2 v1((float)(300+worldx)*0.25f, (float)(300+worldy)*0.25f);

            float feature_roll = 
                noise(v * 132.2324f) -
                noise(v * 140.2324f) +
                noise(v * 32.2324f) +
                noise(v * 20.2324f) +
                noise(v * 13.954f) +
                noise(v * 6.4399f) +
                noise(v * 4.2324f) +
                noise(v * 2.4435f) +
                noise(v * 1.8654f) +
                noise(v * 1.1435f) +
                noise(v * 0.0954f);

            if(feature_roll < 0.2f && feature_roll > 0.1f){
                feature = TREE;
            }else if(feature_roll < 0.25f && feature_roll > 0.2f){
                feature = OLD_TREE;
            }else if(feature_roll < 0.28f && feature_roll > 0.25f){
                feature = FERN;
            }else if(feature_roll < 0.35f && feature_roll > 0.3f){
                feature = LONG_GRASS;
            }

        }
    }


    //feature = NO_FEATURE;
    glm::vec2 pos = place_tile( glm::vec2( (float)-worldy, (float)-worldx), TILE_WID, TILE_SURFACE_HEI);


    _tiles[x][y]->create(pos, glm::vec2(TILE_WID, TILE_HEI), tile, feature);
    _tiles[x][y]->set_elevation(elevation);
    _tiles[x][y]->set_world_pos(worldx, worldy);

}

void Map::zoom(float inc)
{
    if(inc < 0.0f && _zoom + inc > MIN_ZOOM){
        _zoom += inc;
        //if(_tiles_to_render > 10)
            //_tiles_to_render -= 10;
    }else if(inc > 0.0f && _zoom + inc < MAX_ZOOM){
        _zoom += inc;
        //_tiles_to_render += 10;
    }
}

void Map::change_feature(glm::ivec2 tile, FeatureType new_feature)
{
    glm::ivec2 world_pos = _tiles[tile.x][tile.y]->get_world_pos();
    printf("changing feature %s at %d %d \n", feature_defs[new_feature].name.c_str(), world_pos.x, world_pos.y);

    _changes[std::pair<int, int>(world_pos.x, world_pos.y)] = new_feature;
    _tiles[tile.x][tile.y]->set_feature(new_feature);
}

glm::ivec2 Map::adjacent_tile()
{
    Direction dir = _player.facing();
    glm::ivec2 start = _player.get_tile();
    if(dir == NORTH){
        return glm::ivec2(start.x-1, start.y-1); 

    }else if(dir == NORTHEAST){
        return glm::ivec2(start.x, start.y-1); 

    }else if(dir == EAST){
        return glm::ivec2(start.x+1, start.y-1); 

    }else if(dir == SOUTHEAST){
        return glm::ivec2(start.x+1, start.y); 

    }else if(dir == SOUTH){
        return glm::ivec2(start.x+1, start.y+1); 

    }else if(dir == SOUTHWEST){
        return glm::ivec2(start.x, start.y+1); 

    }else if(dir == WEST){
        return glm::ivec2(start.x-1, start.y+1); 

    }else if(dir == NORTHWEST){
        return glm::ivec2(start.x-1, start.y); 
    }else{
        return start;
    }
}


bool save_changes()
{
//    std::fstream fout;
//    fout.open(_name, ios::out);
}

FeatureType Map::get_feature(glm::ivec2 tile)
{
    return _tiles[tile.x][tile.y]->get_feature_type();
}

void Map::move_player(MoveDirection direction)
{
    int offset = 1;
    Direction dir = _player.facing();

    if(direction == BACKWARD){
        dir = reverse_direction(dir);
    }
    

    // Scroll map northwest 
    if(dir == NORTHWEST || dir == NORTH || dir == WEST){
        _world_pos.x -= offset;

        for(int y = 0; y < _dimensions.y; y++){

            delete _tiles[_dimensions.x-1][y];

            for(int x = _dimensions.x-1; x > 0; x--){
                 _tiles[x][y] = _tiles[x-1][y];
            }

            // calculate the world index of the new tile
            int worldx = _world_pos.x - _dimensions.x/2;
            int worldy =  _world_pos.y - _dimensions.y/2 + y;

            _tiles[0][y] = new Tile();
            generate_tile(0, y, worldx, worldy);
        }
    }

    // Scroll map northeast
    if(dir == NORTHEAST || dir == NORTH || dir == EAST){
        _world_pos.y -= offset;

        for(int x = 0; x < _dimensions.x; x++){

            delete _tiles[x][_dimensions.y-1];

            for(int y = _dimensions.y-1; y > 0; y--){
                _tiles[x][y] = _tiles[x][y-1];
            }
            int worldx = _world_pos.x - _dimensions.x/2 + x;
            int worldy =  _world_pos.y - _dimensions.y/2;

            _tiles[x][0] = new Tile();
            generate_tile(x, 0, worldx, worldy);

        }
    }

    // Scroll map southwest
    if(dir == SOUTHWEST || dir == SOUTH || dir == WEST){
        _world_pos.y += offset;

        for(int x = 0; x < _dimensions.x; x++){
            delete _tiles[x][0];

            for(int y = 1; y < _dimensions.y; y++){
                _tiles[x][y-1] = _tiles[x][y];
            }

            int worldx = _world_pos.x - _dimensions.x/2 + x;
            int worldy =  _world_pos.y + _dimensions.y/2 -1;
            
            _tiles[x][_dimensions.y-1] = new Tile();
            generate_tile(x, _dimensions.y-1, worldx, worldy);
        }

    }
    
    // Scroll map southeast 
    if(dir == SOUTHEAST || dir == SOUTH || dir == EAST){
        _world_pos.x += offset;

        for(int y = 0; y < _dimensions.y; y++){
            delete _tiles[0][y];

            for(int x = 1; x < _dimensions.x; x++){
                _tiles[x-1][y] = _tiles[x][y];
            }

            int worldx = _world_pos.x + _dimensions.x/2 - 1;
            int worldy =  _world_pos.y - _dimensions.y/2 + y;
            
            _tiles[_dimensions.x-1][y] = new Tile();
            generate_tile(_dimensions.x-1, y, worldx, worldy);
        }
    }

    _player.move((glm::vec2)_center,  _tiles[_center.x][_center.y]->get_position() );
    _camera_pos.x = _tiles[(int)_center.x][(int)_center.y]->get_position().x;
    _camera_pos.y = _tiles[(int)_center.x][(int)_center.y]->get_position().y;

    printf("world_pos: %d %d\n", _world_pos.x, _world_pos.y);
}

