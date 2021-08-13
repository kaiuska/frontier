#include "button.h"


Button::Button(){}


Button::Button(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size, int action)
{
    init(text, text_scale, pos, size, action);
}

Button::Button(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size, int action, unsigned int texID, int subtex)
{
    init(text, text_scale, pos, size, action);
    set_image(texID, subtex);
}

void Button::init(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size, int action = NONE)
{
    _selected = false;
    _has_image = false;
    set_size(size);
    set_text(text);
    Sprite::create(pos, size, textures[BUTTON]);
    _text_scale = text_scale;
    _action = action;
    _subtex = 0;

    set_position(pos);
    //_projection = glm::ortho(
    //            -(float)scr_wid/2, 
    //            (float)scr_wid/2, 
    //            -(float)scr_hei/2, 
    //            (float)scr_hei/2, 
    //            -20.0f, 10.0f); 
    _projection = glm::ortho(
                0.0f,
                scr_wid,
                0.0f,
                scr_hei,
                -20.0f, 10.0f); 
}


void Button::set_image(unsigned int texID, int subtex)
{

    //_image.create(glm::vec2(_pos.x + _size.x - TILE_WID, _pos.y), glm::vec2(SCALE_TO_TEX, SCALE_TO_TEX), texID);
    _image.create(_pos, glm::vec2(SCALE_TO_TEX, SCALE_TO_TEX), texID);
    _image.set_subtex(subtex);


    glm::vec2 tex_size = _image.get_tex_dimensions();

    float scale = _size.y / tex_size.y;

    _image.set_size(TILE_WID * scale, tex_size.y * scale );
    _has_image = true;
}


void Button::draw(Shader& shader, Shader& text_shader)
{

   _vaoID = sprite_vao;
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glActiveTexture(GL_TEXTURE0);
    shader.use();

    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(_size.x, _size.y, 1.0f));
    model = glm::translate(model, glm::vec3(_pos.x/_size.x, _pos.y/_size.y, 0.0f));
    //model = glm::translate(model, glm::vec3(_pos.x, _pos.y, 0.0f));

    shader.setMat4("model", model);
    shader.setMat4("view", glm::mat4(1.0f));
    //shader.setMat4("projection", _projection);
    //set_highlight(_highlight);
    if(_selected)
        shader.setVec4("highlight", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));


    glBindVertexArray(_vaoID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

    if(_has_image)
        _image.draw(shader);

    render_text(text_shader, _text, _text_pos.x ,_text_pos.y, _text_scale, text_color);
    //shader.setVec4("highlight", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

void Button::set_text(const std::string& text)
{
    _text = text;
}

glm::vec4 Button::get_rect()
{
    return glm::vec4(_pos.x, _pos.y, _pos.x + _size.x, _pos.y-_pos.y);
}


void Button::click()
{
    printf("button \"%s\" clicked\n", _text.c_str());
}



void Button::set_position(glm::vec2 pos)
{
    _pos = pos; 
    //_text_pos

    
    _text_pos.x = pos.x + 20.0f;
    _text_pos.y = pos.y - _size.y + 5.0f;
}


int Button::get_action()
{
    return _action;
}

std::string Button::get_text()
{
    return _text;
}

void Button::set_selected(bool selected)
{
    _selected = selected; 
}
