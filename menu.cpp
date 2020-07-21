#include "menu.h"

MainMenu::MainMenu(){}

MainMenu::MainMenu(glm::vec2 pos, glm::vec2 size)
{
    init(pos, size);
}

void MainMenu::init(glm::vec2 pos, glm::vec2 size)
{
    Sprite::create(pos, size, textures[MAIN_MENU]);
    
    glm::vec2 button_pos(_pos.x + 10, _pos.y - 10);
    _button.init("test_button", 0.5f, _pos, glm::vec2(_size.x -10.0f, 40));
}

void MainMenu::draw(Shader& shader, Shader& text_shader)
{
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

    float t_wid = 1.0f;
    shader.setInt("subtex", _subtex);
    shader.setFloat("tex_wid", t_wid);
    /*
    printf("main menu pos:  %f, %f\n", _pos.x, _pos.y);
    printf("          size: %f, %f\n", _size.x, _size.y);
    printf("          vaoID:  %d\n", _vaoID);
    printf("          texID:  %d\n", _textureID);
    */

    glBindVertexArray(_vaoID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 


    _button.draw(shader, text_shader);
}


void MainMenu::resize(glm::vec2 pos, glm::vec2 size)
{
    //_pos = pos;
    //_size = size;
    set_position(pos);
    set_size(size);

    _button.set_position(glm::vec2(_pos.x+10, _pos.y-10));
    _button.set_size(glm::vec2(size.x-40, 40));
    //_button.set_size();
}



void MainMenu::click(glm::vec2 mouse)
{
    printf("menu clicked\n");

    if(_button.is_clicked(mouse)){
        _button.click();
    }
    //sleep(10);
}






