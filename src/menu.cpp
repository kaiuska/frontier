#include "menu.h"

float menu_wid = 240;
glm::vec3 menu_bg_color(0.3, 0.1, 0.1);

MainMenu::MainMenu(){
    _highlight = 1;
    _border_wid = 10.0f;
}

MainMenu::MainMenu(glm::vec2 pos, glm::vec2 size)
{
    _border_wid = 10;
    init(pos, size);
}

void MainMenu::init(glm::vec2 pos, glm::vec2 size)
{
    std::cout << "main menu: pos:("<<pos.x <<"," << pos.y<<")"<<" size:("<<size.x<<","<<size.y<<")\n";
    Sprite::create(pos, size, textures[MAIN_MENU]);
    
    glm::vec2 button_pos(_pos.x + _border_wid, _pos.y - _border_wid);
    //_button.init("test_button", 0.5f, _pos, glm::vec2(_size.x - 10.0f, 10));
    _projection = glm::ortho(
                -(float)scr_wid/2, 
                (float)scr_wid/2, 
                -(float)scr_hei/2, 
                (float)scr_hei/2, 
                -20.0f, 10.0f); 
}

void MainMenu::draw(Shader& shader, Shader& text_shader)
{
    //if(_highlight >= 0 && _highlight < _buttons.size()){
    //    _buttons[_highlight].set_highlight(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    //}

    glBindTexture(GL_TEXTURE_2D, _textureID);
    glActiveTexture(GL_TEXTURE0);
    shader.use();

    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(_size.x, _size.y, 1.0f));
    model = glm::translate(model, glm::vec3(_pos.x/_size.x, _pos.y/_size.y, 0.0f));

    shader.setMat4("model", model);
    shader.setMat4("view", glm::mat4(1.0f));
    shader.setMat4("projection", _projection);
    shader.setVec4("highlight", glm::vec4(0.0f));
    //set_highlight(glm::vec4(0.0f));

    float t_wid = 1.0f;
    shader.setInt("subtex", _subtex);
    shader.setFloat("tex_wid", t_wid);

    glBindVertexArray(_vaoID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

    for(int i = 0; i < _buttons.size(); i++){
        _buttons[i].set_highlight(glm::vec4(2.0f, 0.0f, 0.0f, 0.0f));
        _buttons[i].draw(shader, text_shader);
    }
}


void MainMenu::resize(glm::vec2 pos, glm::vec2 size)
{
    //_pos = pos;
    //_size = size;
    set_position(pos);
    set_size(size);

    //_button.set_position(glm::vec2(_pos.x+10, _pos.y-10));
    //_button.set_size(glm::vec2(size.x-40, 40));
    //_button.set_size();
    for(int i = 0; i < _buttons.size(); i++){
        _buttons[i].set_position(glm::vec2(_pos.x + 10, _pos.y - 40 * i));
        //_buttons[i].(glm::vec2(_pos.x + 10, _pos.y - 40 * i));

    }

    _projection = glm::ortho(
                -(float)scr_wid/2, 
                (float)scr_wid/2, 
                -(float)scr_hei/2, 
                (float)scr_hei/2, 
                -20.0f, 10.0f); 
}


ActionType MainMenu::get_action(GLFWwindow *window)
{
    bool key_pressed = false;
    _action = NONE;
    do {

        glfwWaitEvents();

        for (int i = 0; i < _buttons.size(); i++) {

            double x, y;
            glfwGetCursorPos(window, &x, &y);
            mouse_pos = glm::vec2(x, y);
            std::cout << "mouse_pos: ("<<mouse_pos.x<<", "<<mouse_pos.y<<")"<<std::endl;

            if (_buttons[i].contains(mouse_pos)) {
                _buttons[i].set_highlight(glm::vec4(1.0f));
                if (mouse_clicked) {
                    _action = (ActionType)click(click_pos);
                    mouse_clicked = false;
                } 
            } else {
                _buttons[i].set_highlight(glm::vec4(0.0f));
            }

        }



        if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
            printf("Action: Chop\n");
            _action = CHOP;
            key_pressed = true;

        }if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
            printf("Action: Build\n");
            _action = BUILD;
            key_pressed = true;

        }if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
            printf("Action: Till\n");
            _action = TILL;
            key_pressed = true;

        }if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
            printf("Action: Plant\n");
            _action = PLANT;
            key_pressed = true;

        }if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            _action = QUIT;
            key_pressed = true;

        }if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            _action = QUIT;
            key_pressed = true;
        }



    } while(_action == NONE);


    ActionType action = _action;
    _action = NONE;
    clear_menu();
    return action;
}

int MainMenu::get_construction(GLFWwindow *window)
{
    bool selected = false;
    FeatureType feature = NO_FEATURE;
    show_action_prompt();

    while(!selected){
        glfwWaitEvents();

        int button_clicked = NO_FEATURE;
        if(mouse_clicked){
            selected = true;
            printf("mouse clicked\n");
            feature = (FeatureType)click(click_pos);
            mouse_clicked = false;
        }

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            _buttons[_highlight].set_selected(false);
            _highlight++;
            _buttons[_highlight].set_selected(true);

        }if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            _buttons[_highlight].set_selected(false);
            _highlight--;
            _buttons[_highlight].set_selected(true);
            
        }if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            feature = NO_FEATURE;
            selected = true;

        }if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
            feature = (FeatureType)_buttons[_highlight].get_action();
            selected = true;

        }if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            feature = NO_FEATURE;
            selected = true;
        }
    }

    //if(selected){
    clear_menu();
    return feature;
    //}
    //else return NO_FEATURE;

}

void MainMenu::show_action_prompt()
{
    printf("action prompt\n");
    _buttons.push_back(Button("Build", 
                0.5f, 
                glm::vec2(_pos.x + 10, _pos.y - 10.0f),
                glm::vec2(_size.x - 30.0f, 30),
                BUILD));
    _buttons.push_back(Button("Chop", 
                0.5f, 
                glm::vec2(_pos.x + 10, _pos.y - 40.0f),
                glm::vec2(_size.x - 30.0f, 30),
                CHOP));
    _buttons.push_back(Button("Till", 
                0.5f, 
                glm::vec2(_pos.x + 10, _pos.y - 70.0f),
                glm::vec2(_size.x - 30.0f, 30),
                TILL));
    _buttons.push_back(Button("Plant", 
                0.5f, 
                glm::vec2(_pos.x + 10, _pos.y - 100.0f),
                glm::vec2(_size.x - 30.0f, 30),
                PLANT));
    _buttons[_highlight].set_highlight(glm::vec4(1.0f));

}



void MainMenu::show_construction_prompt()
{

    printf("construction prompt\n");

    int i = 0;
    float button_hei = 40.0f;
    while(feature_list[i] != NO_FEATURE){
        i++;
    //for(int i = 0; feature_list[i] != TILLED_SOIL; i++){
        FeatureType feature = feature_list[i];

        if(feature_defs[feature].buildable == true){

            //printf("features: %s  ", feature_defs[feature].name.c_str()); 
            //printf("%d %d\n", textures[feature], feature_defs[feature].texID);

            _buttons.push_back(Button(
                        feature_defs[feature].name, 
                        0.5f, 
                        glm::vec2(_pos.x + _border_wid, _pos.y - _border_wid - button_hei*_buttons.size()),
                        glm::vec2(_size.x - 2*_border_wid, button_hei),
                        feature,
                        feature_defs[feature].texID,
                        feature_defs[feature].subtex
                        ));
        }
    }

}


void MainMenu::clear_menu()
{
    _buttons.clear();
}

void MainMenu::hide_action_prompt()
{
    printf("hide action prompt\n");
    _buttons.clear();

}

void MainMenu::add_button(std::string text, glm::vec2 size, ActionType action)
{
    int n_buttons = _buttons.size();

    _buttons.push_back(Button(text, 0.5f, glm::vec2(_pos.x + _border_wid, _pos.y - size.y*n_buttons - _border_wid), size, action));
}


void MainMenu::clear_buttons()
{
    _buttons.clear();
}



int MainMenu::click(glm::vec2 mouse)
{
    printf("menu clicked\n");
    int action = -1;

    for(int i = 0; i < _buttons.size(); i++){
        if(_buttons[i].contains(mouse)){
            _buttons[i].click();
            action = _buttons[i].get_action();
            printf("button %s clicked\n", _buttons[i].get_text().c_str());

        }
    }
    return action;
}




