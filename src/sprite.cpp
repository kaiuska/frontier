#include "sprite.h"


unsigned int sprite_vao;
unsigned int sprite_vbo;

Sprite::Sprite(){
    _vaoID = sprite_vao;
    _highlight = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}



Sprite::Sprite(glm::vec2 pos, glm::vec2 size, unsigned int texID)
{
    /*
    _vaoID = sprite_vao;
    set_textureID(texID);
    set_position(pos);
    set_size(size);
    */
    create(pos, size, texID, 0);
}


void Sprite::create(glm::vec2 pos, glm::vec2 size, unsigned int texID, int subtex)
{
    _subtex = subtex;
    _textureID = texID; 
    _vaoID = sprite_vao;

    _tex_dimensions = get_tex_dimensions();

    set_position(pos);
    set_size(size);

    //glm::mat4 model(1.0f);
    _model = glm::mat4(1.0f);
    _model = glm::scale(_model, glm::vec3(_size.x, _size.y, 1.0f));
    _model = glm::translate(_model, glm::vec3(_pos.x/_size.x, _pos.y/_size.y, 0.0f));

    //_projection = glm::ortho(
    //            -(float)scr_wid/2, 
    //            (float)scr_wid/2, 
    //            -(float)scr_hei/2, 
    //            (float)scr_hei/2, 
    //            -20.0f, 10.0f); 

}

void Sprite::create(glm::vec2 pos, glm::vec2 size, unsigned int texID)
{
    _subtex = 0;
    _textureID = texID; 
    _vaoID = sprite_vao;

    _tex_dimensions = get_tex_dimensions();

    set_position(pos);
    set_size(size);

    _model = glm::mat4(1.0f);
    _model = glm::scale(_model, glm::vec3(_size.x, _size.y, 1.0f));
    _model = glm::translate(_model, glm::vec3(_pos.x/_size.x, _pos.y/_size.y, 0.0f));
}

void Sprite::draw(Shader shader)
{
    if(!_textureID | !_vaoID){
        return;
    }

    //get_tex_dimensions();
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glActiveTexture(GL_TEXTURE0);

    shader.setVec4("highlight", _highlight);

    shader.setMat4("model", _model);

    float t_wid = (float)TILE_WID/_tex_dimensions.x;
    shader.setInt("subtex", _subtex);
    shader.setFloat("tex_wid", t_wid);
    //printf("t_wid: f\n", t_wid);



    glBindVertexArray(_vaoID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

}

void Sprite::set_subtex(int subtex)
{
    _subtex = subtex;

}

void Sprite::set_textureID(unsigned int texID)
{
    _textureID = texID;
    _tex_dimensions = get_tex_dimensions();
}


void Sprite::set_vaoID(unsigned int vaoID)
{
    _vaoID = vaoID;

}



glm::vec2 Sprite::get_position()
{
    return _pos;
}


glm::vec2 Sprite::get_size()
{
    return _size;
}

void Sprite::set_position(glm::vec2 pos)
{
    set_position(pos.x, pos.y);
}

void Sprite::set_position(float x, float y)
{
    _pos = glm::vec2(x, y); 
    update_model_matrix();
}




void Sprite::set_size(float x, float y)
{
    _tex_dimensions = get_tex_dimensions();
    float w = (x == SCALE_TO_TEX) ? _tex_dimensions.x : x;
    float h = (y == SCALE_TO_TEX) ? _tex_dimensions.y : y;

    _size = glm::vec2(w, h); 
    //_pos.y +=  _tex_dimensions.y - tile_surface_hei;
    update_model_matrix();
}


void Sprite::update_model_matrix()
{
    _model = glm::mat4(1.0f);
    _model = glm::scale(_model, glm::vec3(_size.x, _size.y, 1.0f));
    _model = glm::translate(_model, glm::vec3(_pos.x/_size.x, _pos.y/_size.y, 0.0f));
}


void Sprite::set_size(glm::vec2 size)
{
    _tex_dimensions = get_tex_dimensions();
    float w = (size.x == SCALE_TO_TEX) ? _tex_dimensions.x : size.x;
    float h = (size.y == SCALE_TO_TEX) ? _tex_dimensions.y : size.y;

    _size = glm::vec2(w, h); 
    //_pos.y +=  _tex_dimensions.y - tile_surface_hei;
}



glm::vec2 Sprite::get_tex_dimensions()
{
    int w, h;
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glActiveTexture(GL_TEXTURE0);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    //printf("feature %d texture size: %d, %d\n", _feature, w, h);
    
    return glm::vec2(w, h); 
}

bool Sprite::contains(glm::vec2 mouse)
{
    if(mouse.x > _pos.x && mouse.x < _pos.x+_size.x &&
            mouse.y > _pos.y && mouse.y < _pos.y+_size.y){
        return true;
    }
    return false;
}



/*
void Sprite::click()
{ 
}
*/
void Sprite::set_highlight(glm::vec4 highlight)
{
    _highlight = highlight;
}
