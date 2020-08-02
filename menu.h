#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_
#include "utils.h"
#include "shader.h"
#include "button.h"
#include "sprite.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector> 

extern float menu_wid;
extern glm::vec3 menu_bg_color;

class MainMenu : public Sprite
{
public:
    MainMenu();
    MainMenu(glm::vec2 pos, glm::vec2 size);
    void init(glm::vec2 pos, glm::vec2 size);
    void draw(Shader& shader, Shader& text_shader);
    void resize(glm::vec2 pos, glm::vec2 size);

    //bool is_clicked(glm::vec2 mouse);
    int click(glm::vec2 mouse);

    void clear_menu();
    void show_construction_prompt();
    void show_action_prompt();


    ActionType get_action(GLFWwindow *window);
    int get_construction(GLFWwindow *window);
    
    void add_button(std::string text, glm::vec2 size, ActionType action);
    void add_button(FeatureType feature);
    void clear_buttons();
private:
    float _border_wid;
    int _highlight;
    Button _button;
    std::vector<Button> _buttons;
    ActionType _action;
};


#endif
