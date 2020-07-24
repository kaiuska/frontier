#include "button.h"

glm::vec3 button_bg_color(0.35, 0.1, 0.1);

Button::Button(){}


Button::Button(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size)
{
    init(text, text_scale, pos, size);
}

void Button::init(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size)
{
    set_text(text);
    Sprite::create(pos, size, textures[BUTTON]);
    _text_scale = text_scale;

    _text_pos.y = pos.y - size.y/2 - text_hei/2 + 5;//(text_hei*text_scale)/2;
    _text_pos.x = pos.x + 20.0f;

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
    glm::mat4 projection = glm::ortho(
                -(float)scr_wid/2, 
                (float)scr_wid/2, 
                -(float)scr_hei/2, 
                (float)scr_hei/2, 
                -20.0f, 10.0f); 

    shader.setMat4("model", model);
    shader.setMat4("view", glm::mat4(1.0f));
    shader.setMat4("projection", projection);


    glBindVertexArray(_vaoID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 


    render_text(text_shader, _text, _text_pos.x ,_text_pos.y, _text_scale, text_color);
}

void Button::set_text(const std::string& text)
{
    _text = text;
}

glm::vec4 Button::get_rect()
{
    return glm::vec4(_pos, _pos.x + _size.x, _pos.y+_pos.y);
}


/*
bool Button::is_clicked(glm::vec2 mouse)
{
    if(mouse.x > _pos.x && mouse.x < _pos.x+_size.x &&
            mouse.y > _pos.y && mouse.y < _pos.y+_size.y){
        return true;
    }
    return false;
}
*/

void Button::click()
{
    printf("button clicked\n");
    printf("button: pos:  %f %f \n", _pos.x, _pos.y); 
    printf("        size: %f %f \n", _size.x, _size.y); 
    printf("        texID: %d  \n", _textureID); 
    printf("        vaoID: %d  \n", _vaoID); 
}



void Button::set_position(glm::vec2 pos)
{
    printf("moving button\n");
    _pos = pos; 
    //_text_pos

    _text_pos.x = pos.x + 20.0f;
    _text_pos.y = pos.y - _size.y/2 - text_hei/2 + 5;

}



